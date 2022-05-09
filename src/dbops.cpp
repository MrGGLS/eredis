#include "dbdata.h"
#include "dbmeta.h"
#include <fstream>
#include <string>

bool check_meta(DBMeta meta)
{
    // check magic number
    for (int i = 0; i < 4; ++i) {
        if (meta.magic[i] != DBNAME[i])
            return false;
    }

    // check easy redis version
    if (meta.ervers != EASYREDIS_VERSION)
        return false;

    // check db size
    if (meta.size < 0)
        return false;

    return true;
}

EasyDB *init(std::string path)
{
    std::fstream fs;
    path += "dump.er";
    fs.open(path);
    if (!fs.is_open())
        return NULL;

    // load Metadata
    DBMeta meta;
    fs >> meta.magic >> meta.dbvers >> meta.ervers >> meta.size;
    check_meta(meta);

    // load database
    //
    // data store like:
    //
    // dbnum size
    // key type value
    // key type value
    // key type value
    // ...
    int size = meta.size;
    DBData *data = new DBData[size];
    for (int i = 0; i < size; ++i) {
        fs >> data[i].dbnum >> data[i].size;
        for (int j = 0; j < data[i].size; ++j) {
            std::string key;
            int32_t type;
            std::string value;
            fs >> key >> type >> value;
            std::pair<int32_t, std::string> pair(type, value);
            data[i].map.insert({ key, pair });
        }
    }

    fs.close();

    EasyDB *easydb = new EasyDB;
    easydb->meta = meta;
    easydb->data = data;

    return easydb;
}

// very first version, write all data into file rather than
// just write diff data
int save(std::string path, EasyDB *easydb)
{
    std::fstream fs;
    path += "dump.er";
    fs.open(path);
    if (!fs.is_open())
        return -1;

    // write metadata
    fs << easydb->meta.magic << " ";
    fs << easydb->meta.dbvers << " ";
    fs << easydb->meta.ervers << " ";
    fs << easydb->meta.size << " ";

    // write database data
    for (int i = 0; i < easydb->meta.size; ++i) {
        DBData dbdata = easydb->data[i];
        fs << dbdata.dbnum << " ";
        fs << dbdata.size << "\n";
        for (auto iter : dbdata.map) {
            fs << iter.first << " ";
            fs << iter.second.first << " ";
            fs << iter.second.second << "\n";
        }
    }

    fs.close();
    return 0;
}
