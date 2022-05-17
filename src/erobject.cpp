#include "erobject.h"
#include <cassert>
#include <cstdarg>
#include <string>
#include <vector>

ERObject::ERObject(ObjectType type, void *value)
    : type(type)
{
    switch (type) {
    case ObjectType::ER_STRING: {
        std::string str = *(std::string *)value;
        this->value = ERString { str };
        break;
    }
    case ObjectType::ER_List: {
        break;
    }
    default:
        break;
    }
}

ObjectType ERObject::get_type()
{
    return type;
}

bool ERObject::is_list()
{
    return type == ObjectType::ER_List;
}

bool ERObject::is_string()
{
    return type == ObjectType::ER_STRING;
}

bool ERObject::is_unknown()
{
    return type == ObjectType::ER_UNKNOWN;
}

std::string ERObject::get_str()
{
    assert(type == ObjectType::ER_STRING);
    return std::get<ERString>(value).str;
}

std::vector<std::string> ERObject::get_list()
{
    assert(type == ObjectType::ER_List);
    return std::get<ERList>(value).list;
}
