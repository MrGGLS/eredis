#ifndef __EROBJECT_H__
#define __EROBJECT_H__

#include <string>
#include <variant>
#include <vector>

enum class ObjectType {
    EREDIS_UNKNOWN = 0,
    EREDIS_STRING,
    EREDIS_LIST,
    EREDIS_INT,
    EREDIS_DOUBLE
};

struct ERUnknown {
};

struct ERString {
    std::string str;
};

struct ERList {
    std::vector<std::string> list;
};

struct ERInt {
    int32_t num;
};

struct ERDouble {
    double num;
};
// ... 其他类型

/**
 * ERObject: 通用枚举类的实现
 *
 * @type: 枚举类型 type 表示对象的类型
 * @value: 联合体 value 表示对象所存储的数据
 *
 * @warn: 必须将数据包装成 private, 提供对应的操作函数，
 *        否则在使用时可能会导致类型和数据不匹配
 *
 */
class ERObject {
private:
    ObjectType type;
    std::variant<ERUnknown, ERString, ERList, ERInt, ERDouble> value;

public:
    explicit ERObject(ObjectType type, void *value);
    explicit ERObject();
    ObjectType get_type() const;
    bool is_unknown();
    bool is_string();
    bool is_list();
    bool is_int();
    bool is_double();
    std::string get_str() const;
    std::vector<std::string> get_list() const;
    int32_t get_int() const;
    double get_double() const;
    void set_string(std::string str);
    void set_list(std::vector<std::string> list);
    void set_int(int32_t i32);
    void set_double(double f64);
    void as_string();
    void as_list();
    void as_int();
    void as_double();
};

#endif //__EROBJECT_H__
