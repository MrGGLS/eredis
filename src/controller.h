#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Parser.h"
#include "eredis.hpp"
#include <string>

/**
 * Controller
 *
 * @Author Zhang Jing
 *
 * connecter --> [parser --> controller --> server]
 *
 * controller controller the parser to get the syntax result,
 * and send it to server to execute the command
 */
struct Controller {
    Parser parser;
    ERedisServer server;
    ERedisClient *client;

public:
    Controller();
    std::string run(std::string input);

    /**
     * set_json
     *
     * @return json type like:
     * { "type": "$type", "message" = "$msg", "value"="*$value" }
     *
     * @warn if value == nullptr, json will not contain value
     *
     */
    std::string set_json(std::string msg, int type, int *value);
};

#endif // __CONTROLLER_H__
