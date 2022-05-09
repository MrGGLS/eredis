/**
 * Database Operation
 *
 * Set of DB operations, use Syntax Tree as input and provides DB ops.
 *
 * Return a return code which indicates the state of operation.
 *
 * @author Zhang Jing
 *
 */

#ifndef __DBOPS_H__
#define __DBOPS_H__

#include "dbdata.h"

/**
 * Check Metadata of Database
 *
 * check metadata to judge whether the database is corrupted
 *
 */
bool check_meta(DBMeta meta);

/**
 * init database:
 *
 * read the file `dump.er` from $path and load every data to memory
 *
 */
EasyDB *init(std::string path = "./");

/**
 * Save Database:
 *
 * save data to `dump.er` to the $path
 *
 * @return:
 *  0:  success
 *  -1: error
 *
 */
int save(std::string path = "./");

#endif
