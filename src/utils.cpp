/**
 * Utils: basic utils include string and log
 *
 * @authors Mr.GGLS
 *          Zhang Jing
 */

#include "utils.hpp"
#include <iostream>
#include <sstream>

std::string to_lower(std::string str)
{
    std::string res;
    for (const auto &c : str)
        res += tolower(c);
    return res;
}
std::string to_upper(std::string str)
{
    std::string res;
    for (const auto &c : str)
        res += toupper(c);
    return res;
}

std::string format_time()
{
    std::string stime;
    std::stringstream strtime;
    time_t currenttime = time(0);
    char tAll[255];
    strftime(tAll, sizeof(tAll), "%Y-%m-%d|%H:%M:%S", localtime(&currenttime));
    strtime << tAll;
    return strtime.str();
}

void log_err(std::string str)
{
    auto stime = format_time();
    std::cout
#ifdef __APPLE__
        << "\x1b[31m"
#endif
        << "[ERROR|" << stime << "] " << str
#ifdef __APPLE__
        << "\x1b[0m"
#endif
        ;
}

void log_system(std::string str)
{
    auto stime = format_time();
    std::cout
#ifdef __APPLE__
        << "\x1b[36m"
#endif
        << "[SYSTEM|" << stime << "] " << str
#ifdef __APPLE__
        << "\x1b[0m"
#endif
        ;
}

void log_warn(std::string str)
{
    auto stime = format_time();
    std::cout
#ifdef __APPLE__
        << "\x1b[33m"
#endif
        << "[WARN|" << stime << "] " << str
#ifdef __APPLE__
        << "\x1b[0m"
#endif
        ;
}
