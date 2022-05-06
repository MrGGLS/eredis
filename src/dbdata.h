#ifndef __DBDATA_H__
#define __DBDATA_H__

#include "dbmeta.h"
#include <cstdint>
#include <string>
#include <unordered_map>

// datatype
// INT = 0
// FLOAT = 1
// STRING = 2

struct DBData {
    int32_t dbnum;
    int32_t size;
    std::unordered_map<std::string, std::pair<int32_t, std::string>> map;
};

struct EasyDB {
    DBMeta meta;
    DBData *data;
};

#endif
