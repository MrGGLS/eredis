//
// Created by Mr.GGLS on 2022/5/9.
//

#ifndef EASY_REDIS_ERDB_HPP
#define EASY_REDIS_ERDB_HPP
#include "eredis.hpp"
#include <fstream>
#include <iostream>
/* meta data */
#define DBNAME "EREDIS" /* 6 bytes for db name */
#define EREDIS_VERSION "0001" /* four bytes for db version representation */
#define DBNAME_LEN 6
#define VERSION_LEN 4
/* file name */
#define ERDB_FILENAME "dump.erdb"
/* identifiers */
#define EREDIS_EOF 0b10100110 /* use binary */
#define CHECK_SUM_MAX 0xffffffffffffffff
#define CHECK_SUM_LEN 8
#define EREDIS_INT_LEN 4
#define EREDIS_DOUBLE_LEN 8
#define CHECK_SUM 1
#define DICT 2
/* key expire time */
#define EREDIS_EXPIRE_MS 3
/* data type for file... */
#define EREDIS_ERDB_TYPE_INT 10
#define EREDIS_ERDB_TYPE_DOUBLE 11
#define EREDIS_ERDB_TYPE_STRING 12
#define EREDIS_ERDB_TYPE_LIST 13
#define EREDIS_ERDB_TYPE_UNKOWN 99

/* 爱用不用嘛 */
bool save_data(ERedisServer *server, std::string base_path = "./");

/* you should use it when init server */
bool load_data(ERedisServer *server, std::string base_path = "./");

/* checksum operations */
uint64_t cal_checksum(char *data, int len);
bool is_legal_data(uint64_t source_checksum, uint64_t cur_checksum);
#endif // EASY_REDIS_ERDB_HPP
