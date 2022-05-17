#include "erobject.h"
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>

ERObject::ERObject() {
    this->type=ObjectType::EREDIS_UNKNOWN;
    this->value=ERString{"hehe"};
}

ERObject::ERObject(ObjectType type, void *value)
    : type(type) {
    switch (type) {
    case ObjectType::EREDIS_STRING: {
        std::string str = *static_cast<std::string *>(value);
        this->value = ERString{str};
        break;
    }
    case ObjectType::EREDIS_List: {
        std::vector<std::string> list = *static_cast<std::vector<std::string> *>(value);;
        this->value = ERList{list};
        break;
    }
    default:
        break;
    }
}

ObjectType ERObject::get_type() const {
    return type;
}

bool ERObject::is_list() {
    return type == ObjectType::EREDIS_List;
}

bool ERObject::is_string() {
    return type == ObjectType::EREDIS_STRING;
}

bool ERObject::is_unknown() {
    return type == ObjectType::EREDIS_UNKNOWN;
}

std::string ERObject::get_str() const {
    assert(type == ObjectType::EREDIS_STRING);
    return std::get<ERString>(value).str;
}

std::vector<std::string> ERObject::get_list() const {
    assert(type == ObjectType::EREDIS_List);
    return std::get<ERList>(value).list;
}

void ERObject::set_str(std::string str) {
    this->value = ERString{str};
}

void ERObject::set_list(std::vector<std::string> list) {
    this->value = ERList{list};
}
