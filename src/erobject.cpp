#include "erobject.h"
#include <cassert>
#include <vector>

ERObject::ERObject()
{
    this->type = ObjectType::EREDIS_UNKNOWN;
    this->value = ERUnknown();
}

ERObject::ERObject(ObjectType type, void *value)
    : type(type)
{
    switch (type) {
    case ObjectType::EREDIS_INT: {
        int32_t i32 = *static_cast<int32_t *>(value);
        this->value = ERInt { i32 };
        break;
    }
    case ObjectType::EREDIS_DOUBLE: {
        double f64 = *static_cast<double *>(value);
        this->value = ERDouble { f64 };
        break;
    }
    case ObjectType::EREDIS_STRING: {
        std::string str = *static_cast<std::string *>(value);
        this->value = ERString { str };
        break;
    }
    case ObjectType::EREDIS_LIST: {
        std::vector<std::string> list = *static_cast<std::vector<std::string> *>(value);
        this->value = ERList { list };
        break;
    }
    default:
        break;
    }
}

ObjectType ERObject::get_type() const
{
    return type;
}

bool ERObject::is_list()
{
    return type == ObjectType::EREDIS_LIST;
}

bool ERObject::is_string()
{
    return type == ObjectType::EREDIS_STRING;
}

bool ERObject::is_unknown()
{
    return type == ObjectType::EREDIS_UNKNOWN;
}

std::string ERObject::get_str() const
{
    assert(type == ObjectType::EREDIS_STRING);
    return std::get<ERString>(value).str;
}

std::vector<std::string> ERObject::get_list() const
{
    assert(type == ObjectType::EREDIS_LIST);
    return std::get<ERList>(value).list;
}

void ERObject::set_str(std::string str)
{
    assert(type == ObjectType::EREDIS_STRING);
    this->value = ERString { str };
}

void ERObject::set_list(std::vector<std::string> list)
{
    assert(type == ObjectType::EREDIS_LIST);
    this->value = ERList { list };
}
bool ERObject::is_int()
{
    return type == ObjectType::EREDIS_INT;
}
bool ERObject::is_double()
{
    return type == ObjectType::EREDIS_DOUBLE;
}

int32_t ERObject::get_int() const
{
    assert(type == ObjectType::EREDIS_INT);
    return std::get<ERInt>(value).num;
}

double ERObject::get_double() const
{
    assert(type == ObjectType::EREDIS_DOUBLE);
    return std::get<ERDouble>(value).num;
}

void ERObject::set_double(double f64)
{
    assert(type == ObjectType::EREDIS_DOUBLE);
    this->value = ERDouble { f64 };
}

void ERObject::set_int(int32_t i32)
{
    assert(type == ObjectType::EREDIS_INT);
    this->value = ERInt { i32 };
}
