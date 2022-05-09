#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <string>
#include "parser.h"
class Controller {
private:

    parser parser;
    /* Parser parser; */
    /* DBOPS dbops; */
public:
    Controller();
    int run(std::string input);

};

#endif // __CONTROLLER_H__
