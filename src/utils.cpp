//
// Created by Mr.GGLS on 2022/5/20.
//
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
    std::time_t currenttime = std::time(0);
    char tAll[255];
    std::strftime(tAll, sizeof(tAll), "%Y-%m-%d|%H:%M:%S", std::localtime(&currenttime));
    strtime << tAll;
    return strtime.str();
}

void log_err(std::string str)
{
    auto stime = format_time();
    std::cout << "\x1b[31m"
              << "[ERROR|" << stime << "] " << str << "\x1b[0m";
}

void log_system(std::string str)
{
    auto stime = format_time();
    std::cout << "\x1b[36m"
              << "[SYSTEM|" << stime << "] " << str << "\x1b[0m";
}

void log_warn(std::string str)
{
    auto stime = format_time();
    std::cout << "\x1b[33m"
              << "[WARN|" << stime << "] " << str << "\x1b[0m";
}
