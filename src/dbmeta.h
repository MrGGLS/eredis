/**
 * Database Metadata
 *
 * This is the metadata that will store in the file `dump.er`
 *
 * @author Zhang Jing
 */

#ifndef __DBMETA_H__
#define __DBMETA_H__

#include <cstdint>

#define DBNAME "EASY"
#define EASYREDIS_VERSION 1

struct DBMeta {
    int8_t magic[4]; // database name
    int32_t dbvers; // database version
    int32_t ervers; // easy-redis version
    int32_t size; // number of db
};

#endif
