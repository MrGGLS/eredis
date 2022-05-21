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
    this->key_mtx = new std::mutex();
    this->cli_mtx = new std::mutex();
}

ERedisServer::ERedisServer(int db_num)
    : db_num(db_num)
{
    std::vector<ERedisDb *> _db;
    for (auto i = 0; i < db_num; i++) {
        _db.push_back(new ERedisDb(i));
    }
    this->db = _db;
    this->key_mtx = new std::mutex();
    this->cli_mtx = new std::mutex();
}

ERedisServer::~ERedisServer()
{
    delete key_mtx;
    delete cli_mtx;
}

std::string ERedisServer::get_all_keys(int db_id)
{
    std::string keys = "";
    ERedisDb *edb = this->db[db_id];
    std::vector<std::string> should_del_keys= get_all_expire_keys(db_id);
    for(auto del_k:should_del_keys){
        del_key(db_id,del_k);
    }
    std::lock_guard<std::mutex>lg(*key_mtx);
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
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key))
        return REDIS_OK;
    return "not exists";
}

std::string ERedisServer::get_key_type(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0)) {
        del_key(db_id, key);
        return "UNKOWN";
    }
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        switch (edb->dict[key].get_type()) {
        case ObjectType::EREDIS_INT: {
            return "EREDIS_INT";
        }
        case ObjectType::EREDIS_DOUBLE: {
            return "EREDIS_DOUBLE";
        }
        case ObjectType::EREDIS_STRING: {
            return "EREDIS_STRING";
        }
        case ObjectType::EREDIS_LIST: {
            return "EREDIS_LIST";
        }
        default:
            break;
        }
    }
    return "UNKOWN";
}

std::string ERedisServer::del_key(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    std::lock_guard<std::mutex> lg(*key_mtx);
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
    std::vector<std::string> should_del_keys= get_all_expire_keys(db_id);
    for(auto del_k:should_del_keys){
        del_key(db_id,del_k);
    }
    return std::to_string(edb->dict.size());
}

std::string ERedisServer::flushdb(int db_id)
{
    ERedisDb *edb = this->db[db_id];
    std::lock_guard<std::mutex> lg(*key_mtx);
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
    ERedisClient *client = (this->clients[client_id]);
    client->db_id = db_id;
    return REDIS_OK;
}

std::string ERedisServer::set_key(int db_id, std::string key, ERObject erObject)
{
    ERedisDb *edb = this->db[db_id];
    std::lock_guard<std::mutex> lg(*key_mtx);
    if(edb->dict.count(key)>0){
        return REDIS_FAIL;
    }
    edb->dict[key] = erObject;
    return REDIS_OK;
}

std::string ERedisServer::get_key(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        switch (edb->dict[key].get_type()) {
        case ObjectType::EREDIS_INT: {
            return std::to_string(edb->dict[key].get_int());
        }
        case ObjectType::EREDIS_DOUBLE: {
            return std::to_string(edb->dict[key].get_double());
        }
        case ObjectType::EREDIS_STRING: {
            return edb->dict[key].get_str();
        }
        default:
            break;
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::get_strlen(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        switch (edb->dict[key].get_type()) {
        case ObjectType::EREDIS_INT: {
            return std::to_string(std::to_string(edb->dict[key].get_int()).length());
        }
        case ObjectType::EREDIS_DOUBLE: {
            return std::to_string(std::to_string(edb->dict[key].get_double()).length());
        }
        case ObjectType::EREDIS_STRING: {
            return std::to_string(edb->dict[key].get_str().length());
        }
        default:
            break;
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::append_value(int db_id, std::string key, std::string str)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) && edb->dict[key].get_type() == ObjectType::EREDIS_STRING) {
        edb->dict[key].set_string(edb->dict[key].get_str() + str);
        return edb->dict[key].get_str();
    }
    return REDIS_FAIL;
}

/* the interval includes start and end! */
std::string ERedisServer::getrange(int db_id, std::string key, int32_t start, int32_t end)
{
    if(start<0||end< -1)
        return REDIS_FAIL;
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) && (edb->dict[key].get_type() == ObjectType::EREDIS_STRING)) {
        if(end==-1)
            end=edb->dict[key].get_str().length();
        return edb->dict[key].get_str().substr(start, end - start + 1);
    }
    return REDIS_FAIL;
}

std::string ERedisServer::incr(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        if (edb->dict[key].get_type() == ObjectType::EREDIS_INT) {
            edb->dict[key].set_int(edb->dict[key].get_int() + 1);
            return std::to_string(edb->dict[key].get_int());
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::decr(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        if (edb->dict[key].get_type() == ObjectType::EREDIS_INT) {
            edb->dict[key].set_int(edb->dict[key].get_int() - 1);
            return std::to_string(edb->dict[key].get_int());
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::lpush(int db_id, std::string key, ERObject erObject)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (erObject.get_type() != ObjectType::EREDIS_LIST){
        return REDIS_FAIL;
    }
    if (edb->dict.count(key) > 0) {
        auto tmp = erObject.get_list();
        std::vector<std::string> new_list = edb->dict[key].get_list();
        for (int i = tmp.size() - 1; i >= 0; i--) {
            new_list.insert(new_list.begin(), tmp[i]);
        }
        edb->dict[key].set_list(new_list);
        return REDIS_OK;
    } else {
        edb->dict[key] = erObject;
        return REDIS_OK;
    }
    return REDIS_FAIL;
}
std::string ERedisServer::rpush(int db_id, std::string key, ERObject erObject)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (erObject.get_type() != ObjectType::EREDIS_LIST){
        return REDIS_FAIL;
    }
    if (edb->dict.count(key) > 0) {
        auto tmp = erObject.get_list();
        auto new_list = edb->dict[key].get_list();
        new_list.insert(new_list.end(), tmp.begin(), tmp.end());
        edb->dict[key].set_list(new_list);
        return REDIS_OK;
    } else {
        edb->dict[key] = erObject;
        return REDIS_OK;
    }
    return REDIS_FAIL;
}
std::string ERedisServer::lrange(int db_id, std::string key, int start, int end)
{
    if (start < 0 || end < -1)
        return REDIS_FAIL;
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        auto list = edb->dict[key].get_list();
        if (list.size() <= start)
            return "(empty)";
        if (end == -1)
            end = list.size();
        std::string res = "";
        end = end > list.size() ? list.size() : end;
        for (int i = start; i < end; i++) {
            res += std::to_string(i) + ")" + list[i] + "\r\n";
        }
        return res==""?"(empty)":res;
    }
    return REDIS_FAIL;
}

std::string ERedisServer::lpop(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) > 0) {
        if (edb->dict[key].get_list().empty()) {
            edb->dict.erase(key);
        } else {
            auto new_list = edb->dict[key].get_list();
            auto popped = new_list[0];
            new_list.erase(new_list.begin());
            edb->dict[key].set_list(new_list);
            return popped;
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::rpop(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) > 0) {
        if (edb->dict[key].get_list().empty()) {
            edb->dict.erase(key);
        } else {
            auto new_list = edb->dict[key].get_list();
            auto popped = *(new_list.end() - 1);
            new_list.pop_back();
            edb->dict[key].set_list(new_list);
            return popped;
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::lindex(int db_id, std::string key, int index)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) > 0) {
        auto list = edb->dict[key].get_list();
        if (list.size() <= index) {
            return REDIS_FAIL;
        } else {
            return list[index];
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::llen(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) > 0) {
        return std::to_string(edb->dict[key].get_list().size());
    }
    return REDIS_FAIL;
}

std::string ERedisServer::lset(int db_id, std::string key, int index, std::string value)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key) > 0) {
        auto list = edb->dict[key].get_list();
        if (list.size() <= index) {
            return REDIS_FAIL;
        } else {
            auto new_list = edb->dict[key].get_list();
            new_list[index] = std::move(value);
            edb->dict[key].set_list(new_list);
            return REDIS_OK;
        }
    }
    return REDIS_FAIL;
}

std::string ERedisServer::set_expire(int db_id, std::string key, int secs)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        edb->expires[key]=time(0)+secs;
//        edb->expires.insert({ key, time(0) + secs });
        return REDIS_OK;
    }
    return REDIS_FAIL;
}

std::string ERedisServer::ttl(int db_id, std::string key)
{
    ERedisDb *edb = this->db[db_id];
    if (edb->expires.count(key) && edb->expires[key] < time(0))
        del_key(db_id, key);
    std::lock_guard<std::mutex> lg(*key_mtx);
    if (edb->dict.count(key)) {
        if (edb->expires.count(key))
            return std::to_string(edb->expires[key] - time(0));
        else
            return "-1";
    }
    return REDIS_FAIL;
}

std::string ERedisServer::setex(int db_id, std::string key, int secs, ERObject erObject)
{
    ERedisDb *edb = this->db[db_id];
    std::lock_guard<std::mutex> lg(*key_mtx);
    if(edb->dict.count(key)>0){
        return REDIS_FAIL;
    }
    edb->dict.insert({ key, erObject });
    edb->expires.insert({ key, time(0) + secs });
    return REDIS_OK;
}
std::vector<std::string> ERedisServer::get_all_expire_keys(int db_id)
{
    ERedisDb *edb = this->db[db_id];
    std::vector<std::string> should_del_keys;
    std::lock_guard<std::mutex> lg(*key_mtx);
    for (const auto &item : edb->expires) {
        if (item.second < time(0)) {
            should_del_keys.push_back(item.first);
            continue;
        }
    }
    return should_del_keys;
}
std::vector<int> ERedisServer::get_all_idle_clients()
{
    std::vector<int> should_del_clients;
    std::lock_guard<std::mutex> lg(*cli_mtx);
    for (const auto &kv : clients) {
        if (time(0) - kv.second->last_interaction >= EREDIS_DEFAULT_CLIENT_TIMEOUT) {
            should_del_clients.push_back(kv.first);
        }
    }
    return should_del_clients;
}

[[noreturn]] void clear_invalid_keys(ERedisServer *server)
{
    while (true) {
        for (const auto &edb : server->db) {
            std::vector<std::string> should_del_keys = server->get_all_expire_keys(edb->id);
            std::lock_guard<std::mutex> lg(*(server->key_mtx));
            for(auto del_k:should_del_keys){
                server->del_key(edb->id,del_k);
            }
        }
        //        std::cout<<"hehe................."<<std::endl;
        //        std::this_thread::sleep_for(std::chrono::milliseconds (100));
        std::this_thread::sleep_for(std::chrono::milliseconds(EREDIS_DEFAULT_DEL_INTERVAL));
    }
}

[[noreturn]] void clear_idle_clients(ERedisServer *server)
{
    while (true) {
        std::vector<int> should_del_clients=server->get_all_idle_clients();
        std::lock_guard<std::mutex> lg(*(server->cli_mtx));
        for(auto idle_cli:should_del_clients){
            server->clients.erase(idle_cli);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(EREDIS_DEFAULT_DEL_CLIENT_INTERVAL));
    }
}