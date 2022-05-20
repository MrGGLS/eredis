//
// Created by Mr.GGLS on 2022/5/20.
//
#include "utils.hpp"
std::string to_lower(std::string str){
    std::string res;
    for (const auto &c : str)res+=tolower(c);
    return res;
}
std::string to_upper(std::string str){
    std::string res;
    for (const auto &c : str)res+=toupper(c);
    return res;
}
