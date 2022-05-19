#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Parser.h"
#include "eredis.hpp"
#include <string>
class Controller {
private:
    Parser parser;
    ERedisServer server;
    ERedisClient client;

public:
    Controller();
    int run(std::string input);
};

#endif // __CONTROLLER_H__
