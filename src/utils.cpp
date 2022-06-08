/**
 * Utils: basic utils include string and log
 *
 * @authors Mr.GGLS
 *          Zhang Jing
 */

#include "utils.hpp"

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

/**
 * log_* will print a \n, please not send a str with \n in the end
 */
void log_err(std::string str)
{
    auto stime = format_time();
    std::cout
//#ifdef __APPLE__ || _WIN32
        << "\x1b[31m"
//#endif
        << "[ERR|" << stime << "] " << str
        //#ifdef __APPLE__ || _WIN32
        << "\x1b[0m"
//#endif
        << std::endl;
}

/**
 * log_* will print a \n, please not send a str with \n in the end
 */
void log_system(std::string str)
{
//    std::cout<<"fuck you from log"<<std::endl;
    auto stime = format_time();
    std::cout
        //#ifdef __APPLE__ || _WIN32
        << "\x1b[36m"
//#endif
        << "[SYS|" << stime << "] " << str
//#ifdef __APPLE__ || _WIN32
        << "\x1b[0m"
//#endif
        << std::endl;
}

/**
 * log_* will print a \n, please not send a str with \n in the end
 */
void log_warn(std::string str)
{
    auto stime = format_time();
    std::cout
//#ifdef __APPLE__ || _WIN32
        << "\x1b[33m"
//#endif
        << "[WARN|" << stime << "] " << str
//#ifdef __APPLE__ || _WIN32
        << "\x1b[0m"
//#endif
        << std::endl;
}
