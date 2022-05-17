#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "parser.h"
#include "eredis.hpp"
#include <string>
class Controller {
private:
    parser parser;
    ERedisServer server;
    /* Parser parser; */
    /* DBOPS dbops; */
public:
    Controller();
    int run(std::string input);
};

#endif // __CONTROLLER_H__
