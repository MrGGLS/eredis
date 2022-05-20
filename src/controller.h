#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Parser.h"
#include "eredis.hpp"
#include <string>
struct Controller {
    Parser parser;
    ERedisServer server;
    ERedisClient *client;

public:
    Controller();
    std::string run(std::string input);
};

#endif // __CONTROLLER_H__
