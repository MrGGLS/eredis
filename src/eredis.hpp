//
// Created by Mr.GGLS on 2022/5/9.
//

#ifndef EASY_REDIS_EREDIS_HPP
#define EASY_REDIS_EREDIS_HPP

#include "erobject.h"
#include <chrono>
#include <ctime>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
//#include "iostream"


/*error codes*/
#define REDIS_OK "OK"
#define REDIS_FAIL "ERROR"

/*default server config*/
#define EREDIS_SERVER_PORT 6379 // 服务器端口
#define EREDIS_MAX_BACKLOG 10 // TCP监听最大backlog
#define EREDIS_TIMEOUT 0 // default client timeout: infinite
#define EREDIS_DEFAULT_DB_ID 0
#define EREDIS_DEFAULT_DB_NUM 16 // default db num
#define EREDIS_DEFAULT_DEL_INTERVAL 1000 // default check keys validity interval
#define EREDIS_DEFAULT_DEL_CLIENT_INTERVAL 1000 // default check client's validity interval
#define EREDIS_DEFAULT_CLIENT_TIMEOUT (5*60) // client max idle time

struct ERedisDb {
    ERedisDb(int id);
    /*该数据库下的所有键值对*/
    /* Dict *dict; /1* The keyspace for this DB *1/ */
    std::unordered_map<std::string, ERObject> dict;

    /*有期限的键字典（key,value）= (键，过期时间)
     * 进行键操作时需先判断key是否已过期（lazy）*/
    /* Dict *expires; /1* Timeout of keys with a timeout set *1/ */
    std::unordered_map<std::string, std::time_t> expires;

    //发布订阅机制需要
    /* Dict *watched_keys; /1* WATCHED keys for MULTI/EXEC CAS *1/ */
    std::unordered_map<std::string, ERObject> watched_keys;

    int id; /* Database ID */
};

struct ERedisClient {
    int client_id; /* client unique identity */
    int db_id = EREDIS_DEFAULT_DB_ID; /* which db the client is using */

    time_t c_time;/* client created time */
    time_t last_interaction;/* last interaction with server, for clear use */

    std::string client_name;/* can be null */

    std::string hostname;
    int port;

    // TODO: 通信模块
    // all socket function
};

struct ERedisServer {
    /*security*/
    std::string server_auth; /* password */

    // databases
    /* RedisDb *db; */
    std::vector<ERedisDb *> db;

    /* Networking */
    std::string hostname; /* Hostname of server */
    // TCP 监听端口
    int port; /* TCP listening port */
    int tcp_backlog; /* TCP listen() backlog */

    // UNIX 套接字
    //    char *unixsocket; /* UNIX socket path */
    //    mode_t unixsocketperm; /* UNIX socket permission */

    /* RedisClient *current_client;  Current client  */
    //    std::vector<ERedisClient *> clients;
    std::unordered_map<int,ERedisClient*> clients;/* pair: (cliend_id,client) */
    /* Fields used only for stats */
    // 服务器启动时间
    time_t start_time; /* Server start time */
    // 上次save的时间
    time_t last_save; /* Unix time of last successful save */

    /* Configuration */
    int db_num; /* Total number of configured DBs */
    std::string erdb_filename; /* Name of ERDB file */
    int erdb_compression; /* Use compression in ERDB? */
    int erdb_checksum; /* Use ERDB checksum? */

    /* kv operations */
    std::string get_all_keys(int db_id);
    std::string exists_key(int db_id, std::string key);
    std::string get_key_type(int db_id, std::string key);
    std::string del_key(int db_id, std::string key);
    std::string get_dbsize(int db_id);
    std::string flushdb(int db_id);
    std::string flushall();
    std::string select_db(int db_id, int client_id); /* need to know who is using the db */
    std::string set_key(int db_id, std::string key, ERObject erObject);
    std::string get_key(int db_id, std::string key);
    std::string get_strlen(int db_id, std::string key);
    std::string append_value(int db_id, std::string key, std::string str);
    std::string getrange(int db_id, std::string key, int start, int end);

    ERedisServer(std::vector<ERedisDb *> _db, int db_num);
    ERedisServer(int db_num = EREDIS_DEFAULT_DB_NUM);
};

/* check key's validity, delete all invalid keys per interval milliseconds */
[[noreturn]] void clear_invalid_keys(ERedisServer *server);

/* check client's validity, delete all invalid client per interval milliseconds */
[[noreturn]] void clear_idle_clients(ERedisServer *server);

#endif // EASY_REDIS_EREDIS_HPP
