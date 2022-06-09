//
// Created by Mr.GGLS on 2022/5/20.
//

#ifndef EASY_REDIS_UTILS_HPP
#define EASY_REDIS_UTILS_HPP

#include <iostream>
#include <sstream>
#include <string>

std::string to_lower(std::string str);
std::string to_upper(std::string str);

void log_err(std::string);
void log_system(std::string);
void log_warn(std::string);
std::string format_time();

#endif // EASY_REDIS_UTILS_HPP
