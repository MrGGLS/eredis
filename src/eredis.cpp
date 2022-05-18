//
// Created by Mr.GGLS on 2022/5/17.
//
#include "eredis.hpp"
ERedisDb::ERedisDb(int id)
    : id(id)
{
}

ERedisServer::ERedisServer(std::vector<ERedisDb *> _db, int _db_num)
    : db(_db)
    , db_num(_db_num)
{
}

ERedisServer::ERedisServer(int db_num)
    : db_num(db_num)
{
    std::vector<ERedisDb *> _db;
    for (auto i = 0; i < db_num; i++) {
        _db.push_back(new ERedisDb(i));
    }
    this->db = _db;
}

std::string ERedisServer::get_all_keys(int db_id)
{
    std::string keys = "";
    ERedisDb *edb = this->db[db_id];
    for (const auto &item : edb->dict) {
        /* what if the search key has expired */
        if (edb->expires.count(item.first) && edb->expires[item.first] < time(0)) {
            del_key(db_id, item.first);
        }
    }
    if (edb->dict.size() < 1)
        keys = "(empty array)";
    else {
        int i = 0;
        for (const auto &item : edb->dict) {
            keys += (std::to_string(i)) + ")\"" + item.first + "\"\r\n";
            i++;
        }
    }
    return keys;
}

std::string ERedisServer::exists_key(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0)) {
        del_key(db_id, key);
        return "not exists";
    }
    if (edb->dict.count(key))
        return REDIS_OK;
    return "not exists";
}

std::string ERedisServer::get_key_type(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0)) {
        del_key(db_id, key);
        return "unkown type";
    }
    if (edb->dict.count(key)) {
        switch (edb->dict[key].get_type()) {
        case ObjectType::EREDIS_STRING: {
            return "eredis string";
        }
        case ObjectType::EREDIS_List: {
            return "eredis list";
        }
        default:
            break;
        }
    }
    return "unkown type";
}

std::string ERedisServer::del_key(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->dict.count(key)) {
        edb->dict.erase(key);
        edb->expires.erase(key);
        return REDIS_OK;
    } else {
        return REDIS_FAIL;
    }
}

std::string ERedisServer::get_dbsize(int db_id)
{
    ERedisDb *edb = this->db[db_id];
    for (const auto &item : edb->dict) {
        if (edb->expires.count(item.first) && edb->expires[item.first] < time(0)) {
            del_key(db_id, item.first);
        }
    }
    return std::to_string(edb->dict.size());
}

std::string ERedisServer::flushdb(int db_id)
{
    ERedisDb *edb = this->db[db_id];
    edb->dict.clear();
    edb->expires.clear();
    return REDIS_OK;
}

std::string ERedisServer::flushall()
{
    for (auto i = 0; i < this->db_num; i++)
        flushdb(i);
    return REDIS_OK;
}

std::string ERedisServer::select_db(int db_id, int client_id)
{
    ERedisClient *client = (this->cur_client[client_id]);
    client->db_id = db_id;
    return REDIS_OK;
}

std::string ERedisServer::set_key(int db_id, std::string key, ERObject erObject)
{
    ERedisDb *edb = this->db[db_id];
    edb->dict[key] = erObject;
    //    edb.dict[key]=erObject;
    return REDIS_OK;
}

std::string ERedisServer::get_key(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    if (edb->dict.count(key))
        return edb->dict[key].get_str();
    return REDIS_FAIL;
}

std::string ERedisServer::get_strlen(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    if (edb->dict.count(key)) {
        return std::to_string(edb->dict[key].get_str().length());
        //        return ""+edb->dict[key].get_str().length();
    }
    return REDIS_FAIL;
}

std::string ERedisServer::append_value(int db_id, std::string key, std::string str)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    if (edb->dict.count(key)) {
        edb->dict[key].set_str(edb->dict[key].get_str() + str);
        return edb->dict[key].get_str();
    }
    return REDIS_FAIL;
}
/* the interval includes start and end! */
std::string ERedisServer::getrange(int db_id, std::string key, int32_t start, int32_t end)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    if (edb->dict.count(key)) {
        return edb->dict[key].get_str().substr(start, end - start + 1);
    }
    return REDIS_FAIL;
}

[[noreturn]] void clear_invalid_keys(ERedisServer *server)
{
    while (true) {
        for (const auto &edb : server->db) {
            for (const auto &kv : edb->dict) {
                if (edb->expires.count(kv.first) && edb->expires[kv.first] < time(0)) {
                    edb->dict.erase(kv.first);
                    edb->expires.erase(kv.first);
                }
            }
        }
        //        std::cout<<"hehe................."<<std::endl;
        //        std::this_thread::sleep_for(std::chrono::milliseconds (100));
        std::this_thread::sleep_for(std::chrono::milliseconds(EREDIS_DEFAULT_DEL_INTERVAL));
    }
}