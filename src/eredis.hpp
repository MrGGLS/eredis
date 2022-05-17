//
// Created by Mr.GGLS on 2022/5/9.
//

#ifndef EASY_REDIS_EREDIS_HPP
#define EASY_REDIS_EREDIS_HPP

#include "erobject.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>

#define ERDB_FILENAME "MAIN.ERDB"
/*error codes*/
#define REDIS_OK "1"
#define REDIS_FAIL "0"

/*default server config*/
#define EREDIS_SERVER_PORT 6379 // 服务器端口
#define EREDIS_MAX_BACKLOG 10 // TCP监听最大backlog
#define EREDIS_TIMEOUT 0 // default client timeout: infinite
#define EREDIS_DEFAULT_DB_ID 0
#define EREDIS_DEFAULT_DB_NUM 16 // default db num

/* Objects encoding. Some kind of objects like Strings and Hashes can be
 * internally represented in multiple ways. The 'encoding' field of the object
 * is set to one of this fields for this object. */
#define EREDIS_ENCODING_RAW 0 /* Raw representation */
#define EREDIS_ENCODING_INT 1 /* Encoded as integer */
#define EREDIS_ENCODING_HT 2 /* Encoded as hash table */
#define EREDIS_ENCODING_LINKEDLIST 4 /* Encoded as regular linked list */
#define EREDIS_ENCODING_ZIPLIST 5 /* Encoded as ziplist */
#define EREDIS_ENCODING_INTSET 6 /* Encoded as intset */
#define EREDIS_ENCODING_SKIPLIST 7 /* Encoded as skiplist */
#define EREDIS_ENCODING_EMBSTR 8 /* Embedded sds string encoding */

/* Keyspace changes notification classes. Every class is associated with a
 * character for configuration purposes.
 * 如果要实现发布订阅机制*/
#define EREDIS_NOTIFY_KEYSPACE (1 << 0) /* K */
#define EREDIS_NOTIFY_KEYEVENT (1 << 1) /* E */
#define EREDIS_NOTIFY_GENERIC (1 << 2) /* g */
#define EREDIS_NOTIFY_STRING (1 << 3) /* $ */
#define EREDIS_NOTIFY_LIST (1 << 4) /* l */
#define EREDIS_NOTIFY_SET (1 << 5) /* s */
#define EREDIS_NOTIFY_HASH (1 << 6) /* h */
#define EREDIS_NOTIFY_ZSET (1 << 7) /* z */
#define EREDIS_NOTIFY_EXPIRED (1 << 8) /* x */
#define EREDIS_NOTIFY_EVICTED (1 << 9) /* e */
#define EREDIS_NOTIFY_ALL (EREDIS_NOTIFY_GENERIC | EREDIS_NOTIFY_STRING | EREDIS_NOTIFY_LIST | EREDIS_NOTIFY_SET | EREDIS_NOTIFY_HASH | EREDIS_NOTIFY_ZSET | EREDIS_NOTIFY_EXPIRED | EREDIS_NOTIFY_EVICTED) /* A */

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
    int client_id;/* client unique identity */
    int db_id=EREDIS_DEFAULT_DB_ID;/* which db the client is using */
};

struct ERedisServer {
    /*security*/
    std::string server_auth; /* password */

    // databases
    /* RedisDb *db; */
    std::vector<ERedisDb*> db;

    /* Networking */
    std::string hostname; /* Hostname of server */
    // TCP 监听端口
    int port; /* TCP listening port */
    int tcp_backlog; /* TCP listen() backlog */

    // UNIX 套接字
    //    char *unixsocket; /* UNIX socket path */
    //    mode_t unixsocketperm; /* UNIX socket permission */

    /* RedisClient *current_client;  Current client  */
    std::vector<ERedisClient*> cur_client;

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

    std::string get_all_keys(int db_id);
    std::string exists_key(int db_id, std::string key);
    std::string get_key_type(int db_id, std::string key);
    std::string del_key(int db_id, std::string key);
    std::string get_dbsize(int db_id);
    std::string flushdb(int db_id);
    std::string flushall();
    std::string select_db(int db_id, int client_id);/* need to know who is using the db */
    std::string set_key(int db_id, std::string key, ERObject erObject);
    std::string get_key(int db_id, std::string key);
    std::string get_strlen(int db_id, std::string key);
    std::string append_value(int db_id, std::string key, std::string str);
    std::string getrange(int db_id, std::string key, int start ,int end);
    ERedisServer(std::vector<ERedisDb*> _db, int db_num);
    ERedisServer(){};
};
ERedisServer getInstanceOfServer(int db_num=EREDIS_DEFAULT_DB_NUM);

#endif // EASY_REDIS_EREDIS_HPP
