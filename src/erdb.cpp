//
// Created by Mr.GGLS on 2022/5/18.
//
#include "erdb.hpp"
#include "iostream"

bool save_data(ERedisServer *server, std::string base_path)
{
    std::ofstream writer(base_path + ERDB_FILENAME, std::ios::out | std::ios::binary);
    if (writer.bad())
        return false;

    /* header of file */
    writer.write(DBNAME, 6);
    writer.write(EREDIS_VERSION, 4);
    //    writer.write((char*)&server->erdb_checksum,4);
    uint8_t db_num = server->db_num;
    writer.write((char *)&db_num, 1);

    /*content*/
    for (auto i = 0; i < server->db_num; i++) {
        uint8_t db_id = i;
        writer.write((char *)&db_id, 1);
        /* write kv pairs */
        auto edb = server->db[i];
        /* write db size */
        int32_t edb_size = edb->dict.size();
        writer.write((char *)&edb_size, sizeof(edb_size));

        time_t expire_ms;
        int32_t str_len;
        int32_t list_len;
        uint8_t data_type;
        /* write expire time */
        for (const auto &kv : edb->dict) {
            expire_ms = -1;
            /* we don't delete expired kv here */
            if (edb->expires.count(kv.first) > 0) {
                expire_ms = edb->expires[kv.first];
            }
            writer.write((char *)&expire_ms, sizeof(expire_ms));
            /* save key */
            str_len = kv.first.length();
            writer.write((char *)&str_len, sizeof(str_len));
            writer.write(kv.first.c_str(), str_len);
            /* save value */
            switch (kv.second.get_type()) {
            case ObjectType::EREDIS_STRING: {
                data_type = EREDIS_ERDB_TYPE_STRING;
                writer.write((char *)&data_type, sizeof(data_type));
                auto str = kv.second.get_str();
                str_len = str.length();
                writer.write((char *)&str_len, sizeof(str_len));
                writer.write(kv.second.get_str().c_str(), str_len);
                break;
            }
            case ObjectType::EREDIS_List: {
                data_type = EREDIS_ERDB_TYPE_LIST;
                writer.write((char *)&data_type, sizeof(data_type));
                auto list = kv.second.get_list();
                list_len = list.size();
                writer.write((char *)&list_len, sizeof(list_len));
                for (const auto &str : list) {
                    str_len = str.length();
                    writer.write((char *)&str_len, sizeof(str_len));
                    writer.write(kv.second.get_str().c_str(), str_len);
                }
                break;
            }
            default: { /* it should not be access forever... */
                //                    data_type = EREDIS_ERDB_TYPE_UNKOWN;
                //                    writer.write((char*)&data_type,1);
                //                    str_len=0;
                //                    writer.write((char*)&str_len,4);
                //                    writer.write(kv.second.get_str().c_str(),str_len);
                break;
            }
            }
        }
    }
    /* write eof */
    uint8_t eof = EOF;
    writer.write((char *)&eof, sizeof(eof));
    writer.close();
    /* count checksum */
    std::ifstream reader(base_path + ERDB_FILENAME, std::ios::in | std::ios::binary | std::ios::ate);
    if (reader.bad())
        return false;
    int len = reader.tellg();
    char buffer[len];
    reader.seekg(0);
    reader.read(buffer, len);
    auto check_sum = cal_checksum(buffer, len);
    reader.close();
    /* write checksum to erdb */
    writer.open(base_path + ERDB_FILENAME, std::ios::out | std::ios::binary | std::ios::app);
    if (writer.bad())
        return false;
    writer.write((char *)&check_sum, CHECK_SUM_LEN);
    /* end */
    writer.close();
    return true;
}

bool load_data(ERedisServer *server, std::string base_path)
{
    server->db.clear();
    /* check data integrity */
    std::ifstream reader(base_path + ERDB_FILENAME, std::ios::in | std::ios::binary | std::ios::ate);
    if (reader.bad())
        return false;
    int len = reader.tellg();
    len -= CHECK_SUM_LEN;
    char buffer[len];
    reader.seekg(0);
    reader.read(buffer, len);
    auto check_sum = cal_checksum(buffer, len);
    reader.seekg(len);
    uint64_t source_check_sum;
    reader.read((char *)&source_check_sum, CHECK_SUM_LEN);
    if (!is_legal_data(source_check_sum, check_sum)) {
        reader.close();
        return false;
    }

    /* check dbname */
    reader.seekg(0);
    char dbname[DBNAME_LEN] = { 0 };
    reader.read(dbname, DBNAME_LEN);
    auto eredis_dbname = DBNAME;
    for (int i = 0; i < DBNAME_LEN; ++i) {
        if (dbname[i] != eredis_dbname[i]) {
            reader.close();
            return false;
        }
    }

    /* check version */
    char version[VERSION_LEN] = { 0 };
    reader.read(version, VERSION_LEN);
    auto eredis_version = EREDIS_VERSION;
    for (int i = 0; i < VERSION_LEN; ++i) {
        if (version[i] != eredis_version[i]) {
            reader.close();
            return false;
        }
    }

    /* get other parts in header */
    reader.read((char *)&server->db_num, 1);
    /* content */
    uint8_t db_id;
    int edb_size;
    /* read db */
    for (int i = 0; i < server->db_num; i++) {
        /* db id */
        reader.read((char *)&db_id, 1);
        ERedisDb *edb = new ERedisDb(db_id);
        /* db size */
        reader.read((char *)&edb_size, 4);
        /* kv */
        time_t expire_ms;
        int str_len;
        int list_len;
        uint8_t data_type;
        for (int j = 0; j < edb_size; j++) {
            reader.read((char *)&expire_ms, sizeof(expire_ms));
            /* now we should consider key's validity */
            if (expire_ms != -1 && expire_ms < time(0)) {
                continue;
            }
            /* read key */
            reader.read((char *)&str_len, 4);
            char _key[str_len + 1];
            reader.read(_key, str_len);
            _key[str_len] = '\0';
            auto key = std::string(_key);
            //            std::cout<<"key: "<<key<<std::endl;
            /* read value type */
            reader.read((char *)&data_type, 1);
            switch (data_type) {
            case EREDIS_ERDB_TYPE_STRING: {
                reader.read((char *)&str_len, 4);
                char _value[str_len + 1];
                reader.read(_value, str_len);
                _value[str_len] = '\0';
                auto value = ERObject(ObjectType::EREDIS_STRING, new std::string(_value));
                edb->dict.insert({ key, value });
                break;
            }
            case EREDIS_ERDB_TYPE_LIST: {
                /* read list len */
                reader.read((char *)&list_len, 4);
                auto list = new std::vector<std::string>();
                /* read item */
                for (int k = 0; k < list_len; k++) {
                    reader.read((char *)&str_len, 4);
                    char tmp[str_len + 1];
                    reader.read(tmp, str_len);
                    tmp[str_len] = '\0';
                    list->push_back(std::string(tmp));
                }
                auto value = ERObject(ObjectType::EREDIS_List, &list);
                edb->dict.insert({ key, value });
                break;
            }
            default: {
                break;
            }
            }
        }
        server->db.push_back(edb);
    }
    /* end */
    reader.close();
    return true;
}

uint64_t cal_checksum(char *data, int len)
{
    uint64_t sum = 0;
    uint8_t item;
    for (int i = 0; i < len; i++) {
        item = (uint8_t)data[i];
        if (CHECK_SUM_MAX - sum < item) {
            sum += ~item;
            sum++;
        } else {
            sum += ~item;
        }
    }
    return ~sum;
}

bool is_legal_data(uint64_t source_checksum, uint64_t cur_checksum)
{
    //    return CHECK_SUM_MAX==(source_checksum+cur_checksum);
    //    return 0 == (source_checksum + cur_checksum + 1);
    return source_checksum == cur_checksum;
}