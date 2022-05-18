//
// Created by 86136 on 2022/5/9.
//

#ifndef EASY_REDIS_PARSER_H
#define EASY_REDIS_PARSER_H
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "erobject.h"
enum Parser_Token {
    //操作
    key_list_op = 1,
    exists_key_op = 2,
    type_key_op = 3,
    del_key_op = 4,
    dbsize_op = 5,
    flushdb_op = 6,
    flushall_op = 7,
    select_n_op = 8,
    set_key_value_op = 9,
    get_key_op = 10,
    strlen_key = 11,
    append_key_value_op = 12,
    getrange_key_start_end = 13,
    //语法错误
    syntax_error = -1,
    arguments_error = -2,
    no_vaild_error = -3
};
enum Parser_Token_len {

};
//操作根类
class op_result {
public:
    op_result(int optype, const std::string &result);

    int getOptype() const;

    const std::string &getResult() const;

    virtual ~op_result() = default;

    void setOptype(int optype);

    void setResult(const std::string &result);

private:
    int optype;
    std::string result;
};
//不同操作继承根操作类
// key *操作
class key_result : public op_result {
private:
public:
    key_result(int optype, const std::string &result);
};
// exist key操作
class exist_key_result : public op_result {
public:
    exist_key_result(int optype, const std::string &result, const std::string &keyN);

    const std::string &getKeyN() const;

    void setKeyN(const std::string &keyN);

private:
    std::string key_n;
};
class type_key_result : public op_result {
public:
    type_key_result(int optype, const std::string &result, const std::string &keyN);

    const std::string &getKeyN() const;

    void setKeyN(const std::string &keyN);

private:
    std::string key_n;
};
// del key
class del_key_result : public op_result {
public:
    del_key_result(int optype, const std::string &result, const std::string &keyN);

    const std::string &getKeyN() const;

    void setKeyN(const std::string &keyN);

private:
    std::string key_n;
};
// dbsize
class dbsize_result : public op_result {
public:
    dbsize_result(int optype, const std::string &result);
};
// flushdb
class flushdb_result : public op_result {
public:
    flushdb_result(int optype, const std::string &result);
};
// flushall
class flushall_result : public op_result {
public:
    flushall_result(int optype, const std::string &result);
};
// select db
class select_db_result : public op_result {
public:
    select_db_result(int optype, const std::string &result, const int &dbN);

    const int &getDbN() const;

    void setDbN(const int &dbN);

private:
    int db_n;
};
// set key
class set_key_value_result : public op_result {
public:
    set_key_value_result(int optype, const std::string &result, const std::string &key,const ERObject &value);

    const std::string &getKey() const;

    const ERObject &getValue() const;

    void setKey(const std::string &key);

    void setValue(const ERObject &value);

private:
    std::string key;
    ERObject value;
};
// get key
class get_key_result : public op_result {
public:
    get_key_result(int optype, const std::string &result, const std::string &key);

    void setKey(const std::string &key);

    const std::string &getKey() const;

private:
    std::string key;
};
// strlen
class strlen_result : public op_result {
public:
    strlen_result(int optype, const std::string &result, const std::string &key);

    const std::string &getKey() const;

    void setKey(const std::string &key);

private:
    std::string key;
};
// append key value
class append_key_value_result : public op_result {
public:
    append_key_value_result(int optype, const std::string &result, const std::string &key, const std::string &value);

    const std::string &getKey() const;

    const std::string &getValue() const;

    void setKey(const std::string &key);

    void setValue(const std::string &value);

private:
    std::string key;
    std::string value;
};
// getstrange
class getrange_result : public op_result {
public:
    getrange_result(int optype, const std::string &result, const std::string &key, const int32_t &start,
        const int32_t &anEnd);

    const std::string &getKey() const;

    int32_t getStart() const;

    int32_t getAnEnd() const;

    void setKey(const std::string &key);

    void setStart(int32_t start);

    void setAnEnd(int32_t anEnd);

private:
    std::string key;
    int32_t start;
    int32_t end;
};

// Parser_Token split(std::string &str,std::vector<std::string>&result);
bool string_to_int(std::string &str);
class parser {
public:
    parser();
    parser(const std::string &input);

    //    parser(std::string &input);
    Parser_Token split();

    virtual ~parser();

    std::unique_ptr<op_result> key_op();
    std::unique_ptr<op_result> exist_key_op();
    std::unique_ptr<op_result> type_key_op();
    std::unique_ptr<op_result> del_key_op();
    std::unique_ptr<op_result> dbsize_op();
    std::unique_ptr<op_result> flushdb_op();
    std::unique_ptr<op_result> flushall_op();
    std::unique_ptr<op_result> select_db_op();
    std::unique_ptr<op_result> set_key_value_op();
    std::unique_ptr<op_result> get_key_op();
    std::unique_ptr<op_result> strlen_op();
    std::unique_ptr<op_result> append_key_value_op();
    std::unique_ptr<op_result> getstrange_op();
    std::unique_ptr<op_result> syntax_error();
    std::unique_ptr<op_result> run();
    //测试函数
    void split_printf();
    void setInput(const std::string &input);
    void set_SplitResult_NULL();

private:
    std::string input;
    std::vector<std::string> split_result;
    std::unique_ptr<op_result> parser_result;
};
#endif // EASY_REDIS_PARSER_H
