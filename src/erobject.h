#ifndef __EROBJECT_H__
#define __EROBJECT_H__

#include <string>
#include <vector>

enum class ObjectType {
    ER_UNKNOWN = 0,
    ER_STRING,
    ER_List,
};

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

    union Value {
        constexpr Value()
            : unknown {}
        {
        }
        ~Value() {}

        struct {
        } unknown;

        struct ERSTRING {
            std::string str;
        } erstring;

        struct ERList {
            std::vector<std::string> list;
        } erlist;
        // ... 其他类型
    } value;

public:
    explicit ERObject(ObjectType type, void *value);
    ObjectType get_type();
    bool is_unknown();
    bool is_string();
    bool is_list();
    std::string get_str();
    std::vector<std::string> get_list();
};

#endif //__EROBJECT_H__
