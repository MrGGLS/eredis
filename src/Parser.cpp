//
// Created by 86136 on 2022/5/9.
//
#include "Parser.h"
#include <unordered_map>
#include <sstream>
#define doule_e_support 0
/*
 * 目前存在未判定整数长度和浮点数长度是否合格bug
 * */
//string转换为 int double的模板函数
template <class Type>
Type stringToNum(const std::string& str)
{
    std::istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
//bool string_to_int(std::string &str)
//{
//    if (str.size() == 0) {
//        return false;
//    }
//    for (std::string::size_type i = 0; i < str.size(); ++i) {
//        if (!std::isdigit(str[i])) {
//            return false;
//        }
//    }
//    return true;
//}
//判断字符串是否为整数（仅判断格式，不考虑范围）
bool isInt(const char* str)
{
    bool isNum = false;

    int index = 0;
    for (; *str != '\0'; str++, index++)
    {
        switch (*str)
        {
            case '0':case'1':case'2':case'3':case'4':case'5':
            case'6':case'7':case'8':case'9':
                isNum = true;
                break;
            case '-':case '+':
                if (index != 0)
                {
                    return false;
                }
                break;
            default:
                return false;
        }
    }

    if (!isNum)
    {
        return false;
    }

    return true;
}
//判断字符串是否为浮点
//判断字符串是否为浮点数（仅判断格式，不考虑范围）
bool isDouble(const char * str)
{
    bool isE = false,
            isPoint = false,
            numBefore = false,
            numBehind = false;

    int index = 0;
    for (; *str != '\0'; str++, index++)
    {
        switch (*str)
        {
            case '0':case'1':case'2':case'3':case'4':case'5':
            case'6':case'7':case'8':case'9':
                if (isE)
                {
                    numBehind = true;
                }
                else
                {
                    numBefore = true;
                }
                break;
            case '+':case '-':
                if (index != 0)
                {
                    return false;
                }
                break;
#if doule_e_support
            case 'e':case 'E':
                if (isE || !numBefore)
                {
                    return false;
                }
                else
                {
                    isPoint = true;
                    index = -1;
                    isE = true;
                }
                break;
#endif
            case '.':
                if (isPoint)
                {
                    return false;
                }
                else
                {
                    isPoint = true;
                }
                break;
            default:
                return false;
        }
    }

    if (!numBefore)
    {
        return false;
    }
    else if (isE && !numBehind)
    {
        return false;
    }

    return true;
}

//Parser hashmap
static const std::unordered_map<std::string, Parser_Token> PARSER_MAP = {
    { "keys", Parser_Token::key_list_op },
    { "exists", Parser_Token::exists_key_op },
    { "type", Parser_Token::type_key_op },
    { "del", Parser_Token::del_key_op },
    { "dbsize", Parser_Token::dbsize_op },
    { "flushdb", Parser_Token::flushdb_op },
    { "flushall", Parser_Token::flushall_op },
    { "select", Parser_Token::select_n_op },
    { "set", Parser_Token::set_key_value_op },
    { "get", Parser_Token::get_key_op },
    { "strlen", Parser_Token::strlen_key },
    { "append", Parser_Token::append_key_value_op },
    { "getrange", Parser_Token::getrange_key_start_end },
    { "incr",Parser_Token::incr_key_op},
    { "decr",Parser_Token::decr_key_op},
    { "lpush",Parser_Token::lpush_key_list_op},
    { "rpush",Parser_Token::rpush_key_list_op},
    { "lrange",Parser_Token::lrange_key_op},
    { "lpop",Parser_Token::lpop_key_op},
    { "rpop",Parser_Token::rpop_key_op},
    { "lindex",Parser_Token::lindex_key_op},
    { "llen",Parser_Token::llen_key_op},
    { "lset",Parser_Token::lset_key_index_op},
    { "save",Parser_Token::save_op},
    { "expire",Parser_Token::expire_key_op},
    { "setex",Parser_Token::setex_key_op},
    { "ttl",Parser_Token::ttl_key_op}
};

//操作对象的返回函数
std::unique_ptr<op_result> Parser::key_op()
{
    std::unique_ptr<key_result> res = std::make_unique<key_result>(Parser_Token::key_list_op, "");
    if (split_result.size() == 2) {
        if (split_result[1] == "*") {
            return res;
        }
    }
    res.get()->setOptype(Parser_Token::arguments_error);
    return res;
}
std::unique_ptr<op_result> Parser::exist_key_op()
{
    std::unique_ptr<exist_key_result> res = std::make_unique<exist_key_result>(Parser_Token::exists_key_op, "", "");
    if (split_result.size() == 2) {
        res.get()->setKeyN(split_result[1]);
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::type_key_op()
{
    std::unique_ptr<type_key_result> res = std::make_unique<type_key_result>(Parser_Token::type_key_op, "", "");
    if (split_result.size() == 2) {
        res.get()->setKeyN(split_result[1]);
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::del_key_op()
{
    std::unique_ptr<del_key_result> res = std::make_unique<del_key_result>(Parser_Token::del_key_op, "", "");
    if (split_result.size() == 2) {
        res.get()->setKeyN(split_result[1]);
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::dbsize_op()
{
    std::unique_ptr<dbsize_result> res = std::make_unique<dbsize_result>(Parser_Token::dbsize_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::flushdb_op()
{
    std::unique_ptr<flushdb_result> res = std::make_unique<flushdb_result>(Parser_Token::flushdb_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::flushall_op()
{
    std::unique_ptr<flushall_result> res = std::make_unique<flushall_result>(Parser_Token::flushall_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::select_db_op()
{
    std::unique_ptr<select_db_result> res = std::make_unique<select_db_result>(Parser_Token::select_n_op, "", 0);

    if (split_result.size() == 2) {
        if (isInt(split_result[1].c_str())&&split_result[1].size()<3) {
            int N = std::stoi(split_result[1]);
            res.get()->setDbN(N);
            return res;
        } else {
            res.get()->setOptype(Parser_Token::no_vaild_error);
            return res;
        }
        //        char N=split_result[1][0];
        //        if(std::isdigit(N)){
        //            int dbN=N-'0';
        //            res.get()->setDbN(dbN);
        //            return res;
        //        }else{
        //            res.get()->setOptype(Parser_Token::arguments_error);
        //            return res;
        //        }

    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::get_key_op()
{
    std::unique_ptr<get_key_result> res = std::make_unique<get_key_result>(Parser_Token::get_key_op, "", "");
    if (split_result.size() == 2) {
        res.get()->setKey(split_result[1]);
        return res;
    } else {
        res.get()->setOptype(arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::strlen_op()
{
    std::unique_ptr<strlen_result> res = std::make_unique<strlen_result>(Parser_Token::strlen_key, "", "");
    if (split_result.size() == 2) {
        res.get()->setKey(split_result[1]);
        return res;
    } else {
        res.get()->setOptype(arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::append_key_value_op()
{
    std::unique_ptr<append_key_value_result> res = std::make_unique<append_key_value_result>(Parser_Token::append_key_value_op,
        "", "", "");
    if (3 == split_result.size()) {
        res.get()->setKey(split_result[1]);
        res.get()->setValue(split_result[2]);
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> Parser::getstrange_op()
{

    std::unique_ptr<getrange_result> res = std::make_unique<getrange_result>(Parser_Token::getrange_key_start_end, "",
        "", 0, 0);
    if (4 == split_result.size()) {
        try {
            if (isInt(split_result[2].c_str()) && isInt(split_result[3].c_str())) {
                res.get()->setKey(split_result[1]);
                res.get()->setStart(std::stoi(split_result[2]));
                res.get()->setAnEnd(std::stoi(split_result[3]));
                return res;
            } else {
                res.get()->setOptype(Parser_Token::no_vaild_error);
                return res;
            }
        }catch (std::exception){
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
        }

    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
    //    if (4==split_result.size()&&1==split_result[2].size()&&1==split_result[3].size()){
    //        char start=split_result[2][0];
    //        char end=split_result[3][0];
    //        if(std::isdigit(start)&& std::isdigit(end)){
    //            res.get()->setStart(start-'0');
    //            res.get()->setAnEnd(end-'0');
    //            return res;
    //        } else{
    //            res.get()->setOptype(Parser_Token::arguments_error);
    //            return res;
    //        }
    //    } else{
    //        res.get()->setOptype(Parser_Token::arguments_error);
    //        return res;
    //    }
}

//syntax error 函数
std::unique_ptr<op_result> Parser::syntax_error()
{
    return std::make_unique<op_result>(Parser_Token::syntax_error, "");
}
//set命令函数 保存erobject
std::unique_ptr<op_result> Parser::set_key_value_op()
{
    if (3 == split_result.size()) {
        try {
            if(isInt(split_result[2].c_str())){
//            ERObject object = ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]);
                int intvalue= std::stoi(split_result[2]);
                std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::set_key_value_op, "", split_result[1], ERObject(ObjectType::EREDIS_INT, (void *)&intvalue));
                return res;
            } else if (isDouble(split_result[2].c_str())){
                double doubleValue= std::stod(split_result[2]);
                std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::set_key_value_op, "", split_result[1], ERObject(ObjectType::EREDIS_DOUBLE, (void *)&doubleValue));
                return res;
            } else{
                std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::set_key_value_op, "", split_result[1], ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]));
                return res;
            }
        } catch (std::exception) {
            std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::set_key_value_op, "", split_result[1], ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]));
            return res;
        }

    } else {
//        auto start = split_result.begin() + 2;
//        auto end = split_result.end();
//        std::vector<std::string> list_value;
//        list_value.assign(start, end);
//        std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::set_key_value_op, "", split_result[1], ERObject(ObjectType::EREDIS_LIST, (void *)&list_value));
//        return res;
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");
    }
}


std::unique_ptr<op_result> Parser::incr_key_op() {
    if (split_result.size()==2){
        return std::make_unique<incr_result>(Parser_Token::incr_key_op,"",split_result[1]);
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");
    }
}

std::unique_ptr<op_result> Parser::decr_key_op() {
    if(split_result.size()==2){
        return std::make_unique<decr_result>(Parser_Token::decr_key_op,"",split_result[1]);

    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }
}

std::unique_ptr<op_result> Parser::lpush_key_op() {
        auto start = split_result.begin() + 2;
        auto end = split_result.end();
        std::vector<std::string> list_value;
        list_value.assign(start, end);
        std::unique_ptr<lpush_result> res = std::make_unique<lpush_result>(Parser_Token::lpush_key_list_op, "", split_result[1], ERObject(ObjectType::EREDIS_LIST, (void *)&list_value));
        return res;
}

std::unique_ptr<op_result> Parser::rpush_key_op() {
    auto start = split_result.begin() + 2;
    auto end = split_result.end();
    std::vector<std::string> list_value;
    list_value.assign(start, end);
    std::unique_ptr<rpush_result> res = std::make_unique<rpush_result>(Parser_Token::rpush_key_list_op, "", split_result[1], ERObject(ObjectType::EREDIS_LIST, (void *)&list_value));
    return res;
}

std::unique_ptr<op_result> Parser::lrange_key_op() {
    if (split_result.size()==4){
        if (isInt(split_result[2].c_str())& isInt(split_result[3].c_str())){
            try {
                int start=std::stoi(split_result[2]);
                int end=std::stoi(split_result[3]);
                return std::make_unique<lrange_result>(Parser_Token::lrange_key_op,"", split_result[1],start,end);
            } catch (std::exception) {
                return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
            }

        } else{
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");

        }
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }
}

std::unique_ptr<op_result> Parser::lpop_key_op() {
    if (split_result.size()==2){
        return std::make_unique<lpop_result>(Parser_Token::lpop_key_op,"",split_result[1]);
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }

}

std::unique_ptr<op_result> Parser::rpop_key_op() {
    if (split_result.size()==2){
        return std::make_unique<rpop_result>(Parser_Token::rpop_key_op,"",split_result[1]);
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }
}

std::unique_ptr<op_result> Parser::lindex_key_op() {
    if (split_result.size()==3){
        if (isInt(split_result[2].c_str())){
            try {
                int index=std::stoi(split_result[2]);
                return std::make_unique<lindex_result>(Parser_Token::lindex_key_op,"",split_result[1], index);
            } catch (std::exception) {
                return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
            }

        } else{
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
        }
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");
    }
}

std::unique_ptr<op_result> Parser::llen_key_op() {
    if (split_result.size()==2){
        return std::make_unique<llen_result>(Parser_Token::llen_key_op,"",split_result[1]);
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }
}

std::unique_ptr<op_result> Parser::lset_key_op() {
    if (split_result.size()==4){
        if (isInt(split_result[2].c_str())){
            try {
                int index=std::stoi(split_result[2]);
                return std::make_unique<lset_result>(Parser_Token::lset_key_index_op,"",split_result[1], index,split_result[3]);
            } catch (std::exception) {
                return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
            }

        } else{
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
        }
    } else{
        return std::make_unique<op_result>(Parser_Token::arguments_error,"");

    }
}

std::unique_ptr<op_result> Parser::save_op() {
    if(split_result.size()==1){
        return std::make_unique<save_result>(Parser_Token::save_op,"");
    } else{
        return std::make_unique<op_result>(Parser_Token::syntax_error,"");
    }
}

std::unique_ptr<op_result> Parser::expire_key_op() {
    if (split_result.size()==3){
        if (isInt(split_result[2].c_str())){
            try {
                int time = std::stoi(split_result[2]);
                return std::make_unique<expire_result>(Parser_Token::expire_key_op,"",split_result[1], time);
            } catch (std::exception) {
                return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
            }

        } else{
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
        }
    } else
        return std::make_unique<op_result>(Parser_Token::syntax_error,"");
}

std::unique_ptr<op_result> Parser::setex_key_op() {
    if (split_result.size()==4){
        if (isInt(split_result[2].c_str())){
            int time;
            try {
                time = std::stoi(split_result[2]);
            } catch (std::exception) {
                return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");
            }

            try {
                if(isInt(split_result[3].c_str())){
//            ERObject object = ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]);
                    int intvalue= std::stoi(split_result[3]);
                    std::unique_ptr<setex_result> res = std::make_unique<setex_result>(Parser_Token::setex_key_op, "", split_result[1],
                                                                                       time,ERObject(ObjectType::EREDIS_INT, (void *)&intvalue));
                    return res;
                } else if (isDouble(split_result[3].c_str())){
                    double doubleValue= std::stod(split_result[3]);
                    std::unique_ptr<setex_result> res = std::make_unique<setex_result>(Parser_Token::setex_key_op, "", split_result[1],
                                                                                       time, ERObject(ObjectType::EREDIS_DOUBLE, (void *)&doubleValue));
                    return res;
                } else{
                    std::unique_ptr<setex_result> res = std::make_unique<setex_result>(Parser_Token::setex_key_op, "", split_result[1],
                                                                                       time,ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]));
                    return res;
                }
            } catch (std::exception) {
                std::unique_ptr<setex_result> res = std::make_unique<setex_result>(Parser_Token::setex_key_op, "", split_result[1],
                                                                                   time,ERObject(ObjectType::EREDIS_STRING, (void *)&split_result[2]));
                return res;
            }
        } else{
            return std::make_unique<op_result>(Parser_Token::no_vaild_error,"");

        }
    } else{
        return std::make_unique<op_result>(Parser_Token::syntax_error,"");

    }
}

std::unique_ptr<op_result> Parser::ttl_key_op() {
    if (split_result.size()==2){
        return std::make_unique<ttl_key_result>(Parser_Token::ttl_key_op,"",split_result[1]);
    } else{
        return std::make_unique<op_result>(Parser_Token::syntax_error,"");

    }
}


/*op_result类的函数及其子类的函数
 *
*/
op_result::op_result(int optype, const std::string &result)
    : optype(optype)
    , result(result)
{
}

int op_result::getOptype() const
{
    return optype;
}

const std::string &op_result::getResult() const
{
    return result;
}

void op_result::setOptype(int optype)
{
    op_result::optype = optype;
}

void op_result::setResult(const std::string &result)
{
    op_result::result = result;
}

//
key_result::key_result(int optype, const std::string &result)
    : op_result(optype, result)
{
}
//
exist_key_result::exist_key_result(int optype, const std::string &result, const std::string &keyN)
    : op_result(optype,
        result)
    , key_n(keyN)
{
}

const std::string &exist_key_result::getKeyN() const
{
    return key_n;
}

void exist_key_result::setKeyN(const std::string &keyN)
{
    key_n = keyN;
}

type_key_result::type_key_result(int optype, const std::string &result, const std::string &keyN)
    : op_result(optype,
        result)
    , key_n(keyN)
{
}

const std::string &type_key_result::getKeyN() const
{
    return key_n;
}

void type_key_result::setKeyN(const std::string &keyN)
{
    key_n = keyN;
}

del_key_result::del_key_result(int optype, const std::string &result, const std::string &keyN)
    : op_result(optype,
        result)
    , key_n(keyN)
{
}

const std::string &del_key_result::getKeyN() const
{
    return key_n;
}

void del_key_result::setKeyN(const std::string &keyN)
{
    key_n = keyN;
}

dbsize_result::dbsize_result(int optype, const std::string &result)
    : op_result(optype, result)
{
}

flushdb_result::flushdb_result(int optype, const std::string &result)
    : op_result(optype, result)
{
}

flushall_result::flushall_result(int optype, const std::string &result)
    : op_result(optype, result)
{
}

select_db_result::select_db_result(int optype, const std::string &result, const int &dbN)
    : op_result(optype,
        result)
    , db_n(dbN)
{
}

const int &select_db_result::getDbN() const
{
    return db_n;
}

void select_db_result::setDbN(const int &dbN)
{
    db_n = dbN;
}

set_key_value_result::set_key_value_result(int optype, const std::string &result, const std::string &key, const ERObject &value)
    : op_result(optype, result)
    , key(key)
    , value(value)
{
}

const std::string &set_key_value_result::getKey() const
{
    return key;
}

const ERObject &set_key_value_result::getValue() const
{
    return value;
}

void set_key_value_result::setKey(const std::string &key)
{
    set_key_value_result::key = key;
}

void set_key_value_result::setValue(const ERObject &value)
{
    set_key_value_result::value = value;
}

get_key_result::get_key_result(int optype, const std::string &result, const std::string &key)
    : op_result(optype,
        result)
    , key(key)
{
}

const std::string &get_key_result::getKey() const
{
    return key;
}

void get_key_result::setKey(const std::string &key)
{
    get_key_result::key = key;
}

strlen_result::strlen_result(int optype, const std::string &result, const std::string &key)
    : op_result(optype, result)
    , key(key)
{
}

const std::string &strlen_result::getKey() const
{
    return key;
}

void strlen_result::setKey(const std::string &key)
{
    strlen_result::key = key;
}

append_key_value_result::append_key_value_result(int optype, const std::string &result, const std::string &key,
    const std::string &value)
    : op_result(optype, result)
    , key(key)
    , value(value)
{
}

const std::string &append_key_value_result::getKey() const
{
    return key;
}

const std::string &append_key_value_result::getValue() const
{
    return value;
}

void append_key_value_result::setKey(const std::string &key)
{
    append_key_value_result::key = key;
}

void append_key_value_result::setValue(const std::string &value)
{
    append_key_value_result::value = value;
}

getrange_result::getrange_result(int optype, const std::string &result, const std::string &key, const int32_t &start,
    const int32_t &anEnd)
    : op_result(optype, result)
    , key(key)
    , start(start)
    , end(anEnd)
{
}

const std::string &getrange_result::getKey() const
{
    return key;
}

int32_t getrange_result::getStart() const
{
    return start;
}

int32_t getrange_result::getAnEnd() const
{
    return end;
}

void getrange_result::setKey(const std::string &key)
{
    getrange_result::key = key;
}

void getrange_result::setStart(int32_t start)
{
    getrange_result::start = start;
}

void getrange_result::setAnEnd(int32_t anEnd)
{
    end = anEnd;
}

incr_result::incr_result(int optype, const std::string &result, const std::string &key) : op_result(optype, result),
                                                                                          key(key) {}

const std::string &incr_result::getKey() const {
    return key;
}

void incr_result::setKey(const std::string &key) {
    incr_result::key = key;
}

decr_result::decr_result(int optype, const std::string &result, const std::string &key) : op_result(optype, result),
                                                                                          key(key) {}

const std::string &decr_result::getKey() const {
    return key;
}

void decr_result::setKey(const std::string &key) {
    decr_result::key = key;
}

lpush_result::lpush_result(int optype, const std::string &result, const std::string &key, const ERObject &value)
        : op_result(optype, result), key(key), value(value) {}

const std::string &lpush_result::getKey() const {
    return key;
}

void lpush_result::setKey(const std::string &key) {
    lpush_result::key = key;
}

const ERObject &lpush_result::getValue() const {
    return value;
}

void lpush_result::setValue(const ERObject &value) {
    lpush_result::value = value;
}

rpush_result::rpush_result(int optype, const std::string &result, const std::string &key, const ERObject &value)
        : op_result(optype, result), key(key), value(value) {}

const std::string &rpush_result::getKey() const {
    return key;
}

void rpush_result::setKey(const std::string &key) {
    rpush_result::key = key;
}

const ERObject &rpush_result::getValue() const {
    return value;
}

void rpush_result::setValue(const ERObject &value) {
    rpush_result::value = value;
}

lrange_result::lrange_result(int optype, const std::string &result, const std::string &key, int start, int anEnd)
        : op_result(optype, result), key(key), start(start), end(anEnd) {}

const std::string &lrange_result::getKey() const {
    return key;
}

void lrange_result::setKey(const std::string &key) {
    lrange_result::key = key;
}

int lrange_result::getStart() const {
    return start;
}

void lrange_result::setStart(int start) {
    lrange_result::start = start;
}

int lrange_result::getAnEnd() const {
    return end;
}

void lrange_result::setAnEnd(int anEnd) {
    end = anEnd;
}

lpop_result::lpop_result(int optype, const std::string &result, const std::string &key) : op_result(optype, result),
                                                                                          key(key) {}

const std::string &lpop_result::getKey() const {
    return key;
}

void lpop_result::setKey(const std::string &key) {
    lpop_result::key = key;
}

rpop_result::rpop_result(int optype, const std::string &result, const std::string &key) : op_result(optype, result),
                                                                                          key(key) {}

const std::string &rpop_result::getKey() const {
    return key;
}

void rpop_result::setKey(const std::string &key) {
    rpop_result::key = key;
}

lindex_result::lindex_result(int optype, const std::string &result, const std::string &key,int index) : op_result(optype, result),
                                                                                              key(key),index(index) {}

const std::string &lindex_result::getKey() const {
    return key;
}

void lindex_result::setKey(const std::string &key) {
    lindex_result::key = key;
}
int lindex_result::getIndex() {
    return index;
}
llen_result::llen_result(int optype, const std::string &result, const std::string &key) : op_result(optype,
                                                                                                               result),
                                                                                                     key(key){}

const std::string &llen_result::getKey() const {
    return key;
}

void llen_result::setKey(const std::string &key) {
    llen_result::key = key;
}

lset_result::lset_result(int optype, const std::string &result, const std::string &key, int index, const std::string &value)
        : op_result(optype, result), key(key), index(index), value(value) {}

const std::string &lset_result::getKey() const {
    return key;
}

void lset_result::setKey(const std::string &key) {
    lset_result::key = key;
}

int lset_result::getIndex() const {
    return index;
}

void lset_result::setIndex(int index) {
    lset_result::index = index;
}

const std::string &lset_result::getValue() const {
    return value;
}

void lset_result::setValue(const std::string &value) {
    lset_result::value = value;
}

save_result::save_result(int optype, const std::string &result) : op_result(optype, result) {}

expire_result::expire_result(int optype, const std::string &result, const std::string &key, int time) : op_result(
        optype, result), key(key), time(time) {}

const std::string &expire_result::getKey() const {
    return key;
}

void expire_result::setKey(const std::string &key) {
    expire_result::key = key;
}

int expire_result::getTime() const {
    return time;
}

void expire_result::setTime(int time) {
    expire_result::time = time;
}

setex_result::setex_result(int optype, const std::string &result, const std::string &key, int time,
                           const ERObject &value) : op_result(optype, result), key(key), time(time), value(value) {}

const std::string &setex_result::getKey() const {
    return key;
}

void setex_result::setKey(const std::string &key) {
    setex_result::key = key;
}

int setex_result::getTime() const {
    return time;
}

void setex_result::setTime(int time) {
    setex_result::time = time;
}

const ERObject &setex_result::getValue() const {
    return value;
}

void setex_result::setValue(const ERObject &value) {
    setex_result::value = value;
}

ttl_key_result::ttl_key_result(int optype, const std::string &result, const std::string &key) : op_result(optype,
                                                                                                          result),
                                                                                                key(key) {}

const std::string &ttl_key_result::getKey() const {
    return key;
}

void ttl_key_result::setKey(const std::string &key) {
    ttl_key_result::key = key;
}

/*
 * parser的构造函数，需要解决的问题
 * 1. 将传入的字符串按照” “分割
 * 2. 根据字符串中的双引号判断语法是否正确，双引号用来规定字符串，除此之外出现在任何地方都是语法错误
 * */
// Parser::Parser(std::string& input) {
//     int a = split();
//     parser_result.get()->setOptype(Parser_Token::syntax_error);
//     parser_result.get()->setResult(NULL);
//     this->input = input;
// }
//     Parser_Token split_token=split();
//     switch (split_token) {
//         case Parser_Token::key_list_op:
//             key_op();
//             break;
//         case Parser_Token::exists_key_op:
//             exist_key_op();
//             break;
//         case Parser_Token::type_key_op:
//             type_key_op();
//             break;
//         case Parser_Token::del_key_op:
//             del_key_op();
//             break;
//         case Parser_Token::dbsize_op:
//             dbsize_op();
//             break;
//         case Parser_Token::flushdb_op:
//             flushdb_op();
//             break;
//         case Parser_Token::flushall_op:
//             flushall_op();
//             break;
//         case Parser_Token::select_n_op:
//             select_db_op();
//             break;
//         case Parser_Token::get_key_op:
//             get_key_op();
//             break;
//         case Parser_Token::set_key_value_op:
//             set_key_value_op();
//             break;
//         case Parser_Token::strlen_key:
//             strlen_op();
//             break;
//         case Parser_Token::append_key_value_op:
//             append_key_value_op();
//             break;
//         case Parser_Token::getrange_key_start_end:
//             getstrange_op();
//             break;
//         default:
//             syntax_error();
//             break;
//     }
//

Parser_Token Parser::split()
{

    std::string::size_type npos = -1;
    std::string::size_type space_pos;
    std::string::size_type quotation_pos;
    std::string::size_type cur_quotation_pos = 0;
    std::string::size_type space_n = 0;
    std::string::size_type quotation_n[10];
    input += " ";
    std::string::size_type size = input.size();
    //   quotation_pos=input.find('"');
    int N = 0;
    for (int i = 0; i < size; ++i) {
        quotation_pos = input.find('"', i);
        if (quotation_pos != npos) {
            N++;
            quotation_n[N] = quotation_pos;
            i = quotation_pos;
        }
    }
    if (N % 2 != 0) {
        return Parser_Token::syntax_error;
    }
    N = 1;
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] != ' ') {
            break;
        } else {
            space_n++;
        }
    }
    input.erase(0, space_n);
    quotation_pos = input.find('"');
    if (quotation_pos != npos) {
        for (std::string::size_type i = 0; i < size; ++i) {
            space_pos = input.find(' ', i);
            quotation_pos = input.find('"', i);
            //           if (space_pos==npos&&quotation_pos==npos){
            if (quotation_pos == npos) {
                std::string::size_type pos;
                //        input+=" ";
                for (; i < size; ++i) {
                    pos = input.find(' ', i);
                    if (pos < size) {
                        std::string s = input.substr(i, pos - i);
                        split_result.push_back(s);
                        for (i = pos; i < size; ++i) {
                            if (input[i] != ' ') {
                                //                        i=j;
                                break;
                            }
                        }
                        i--;
                    }
                }
                //                std::string sub_input=input.substr(i,input.size()-1-i);
                //                split_result.push_back(sub_input);
                break;
            }

            if (quotation_pos >= i) {
                if (' ' == input[quotation_pos - 1] && space_pos > i && N % 2 != 0) {

                    cur_quotation_pos = quotation_pos;
                    std::string sub_input = input.substr(i, space_pos - i);
                    split_result.push_back(sub_input);

                    for (i = space_pos; i < size; ++i) {

                        //                        if (input[i]!=' '&&input[i]!='"'){
                        //                            break;
                        //                        }
                        if (input[i] != ' ') {
                            if (input[i] == '"') {
                                i++;
                                N++;
                                break;
                            } else {
                                break;
                            }
                        }
                        cur_quotation_pos = input.find('"', i);
                    }
                    i--;

                    //                    if (space_pos!=quotation_pos-1){
                    //
                    //                        i=space_pos;
                    //                        continue;
                    //                    } else{
                    //                        i=quotation_pos;
                    //                        continue;
                    //                    }

                } else if (' ' == input[quotation_pos + 1] && cur_quotation_pos > 0 && N % 2 == 0) {
                    std::string sub_input = input.substr(cur_quotation_pos + 1, quotation_pos - cur_quotation_pos - 1);
                    split_result.push_back(sub_input);
                    i = quotation_pos + 1;
                    for (; i < size; ++i) {
                        if (input[i] != ' ') {
                            if (input[i] == '"') {
                                i++;
                                // N++;
                                break;
                            } else {
                                break;
                            }
                        }
                        //                        if (input[i]!=' '&&input[i]!='"'){
                        //                            break;
                        //                        }
                        cur_quotation_pos = input.find('"', i);
                    }
                    i--;
                    continue;
                } else {
                    return Parser_Token::syntax_error;
                }
            } else {
                if (space_pos > i) {
                    std::string sub_input = input.substr(i, space_pos - i);
                    split_result.push_back(sub_input);
                    i = space_pos - 1;
                    continue;
                } else {
                    return Parser_Token::syntax_error;
                }
            }
        }
    } else {
        std::string::size_type pos;
        //        input+=" ";
        for (std::string::size_type i = 0; i < size; ++i) {
            pos = input.find(' ', i);
            if (pos < size) {
                std::string s = input.substr(i, pos - i);
                split_result.push_back(s);
                for (i = pos; i < size; ++i) {
                    if (input[i] != ' ') {
                        //                        i=j;
                        break;
                    }
                }
                i--;
            }
        }
    }

    //    if (split_result[0] == "keys") {
    //        return Parser_Token::key_list_op;
    //    } else if (split_result[0] == "exists") {
    //        return Parser_Token::exists_key_op;
    //    } else if (split_result[0] == "type") {
    //        return Parser_Token::type_key_op;
    //    } else if (split_result[0] == "del") {
    //        return Parser_Token::del_key_op;
    //    } else if (split_result[0] == "dbsize") {
    //        return Parser_Token::dbsize_op;
    //    } else if (split_result[0] == "flushdb") {
    //        return Parser_Token::flushdb_op;
    //    } else if (split_result[0] == "flushall") {
    //        return Parser_Token::flushall_op;
    //    } else if (split_result[0] == "select") {
    //        return Parser_Token::select_n_op;
    //    } else if (split_result[0] == "set") {
    //        return Parser_Token::set_key_value_op;
    //    } else if (split_result[0] == "get") {
    //        return Parser_Token::get_key_op;
    //    } else if (split_result[0] == "strlen") {
    //        return Parser_Token::strlen_key;
    //    } else if (split_result[0] == "append") {
    //        return Parser_Token::append_key_value_op;
    //    } else if (split_result[0] == "getrange") {
    //        return Parser_Token::getrange_key_start_end;
    //    } else {
    //        return Parser_Token::syntax_error;
    //    }
    auto pair = PARSER_MAP.find(split_result[0]);
    return pair == PARSER_MAP.end() ? Parser_Token::syntax_error : pair->second;
}


std::unique_ptr<op_result> Parser::run()
{
    Parser_Token split_token = split();
    switch (split_token) {
        case Parser_Token::key_list_op:
            return key_op();
            break;
        case Parser_Token::exists_key_op:
            return exist_key_op();
            break;
        case Parser_Token::type_key_op:
            return type_key_op();
            break;
        case Parser_Token::del_key_op:
            return del_key_op();
            break;
        case Parser_Token::dbsize_op:
            return dbsize_op();
            break;
        case Parser_Token::flushdb_op:
            return flushdb_op();
            break;
        case Parser_Token::flushall_op:
            return flushall_op();
            break;
        case Parser_Token::select_n_op:
            return select_db_op();
            break;
        case Parser_Token::get_key_op:
            return get_key_op();
            break;
        case Parser_Token::set_key_value_op:
            return set_key_value_op();
            break;
        case Parser_Token::strlen_key:
            return strlen_op();
            break;
        case Parser_Token::append_key_value_op:
            return append_key_value_op();
            break;
        case Parser_Token::getrange_key_start_end:
            return getstrange_op();
            break;
        case Parser_Token::incr_key_op:
            return incr_key_op();
            break;
        case Parser_Token::decr_key_op:
            return decr_key_op();
            break;
        case Parser_Token::  lpush_key_list_op:
            return lpush_key_op();
            break;
        case Parser_Token::rpush_key_list_op:
            return rpush_key_op();
            break;
        case Parser_Token::lrange_key_op:
            return lrange_key_op();
            break;
        case Parser_Token::lpop_key_op:
            return lpop_key_op();
            break;
        case Parser_Token::rpop_key_op:
            return rpop_key_op();
            break;
        case Parser_Token::lindex_key_op:
            return lindex_key_op();
            break;
        case Parser_Token::llen_key_op:
            return llen_key_op();
            break;
        case Parser_Token::lset_key_index_op:
            return lset_key_op();
            break;
        case Parser_Token::save_op:
            return save_op();
            break;
        case Parser_Token::expire_key_op:
            return expire_key_op();
            break;
        case Parser_Token::setex_key_op:
            return setex_key_op();
            break;
        case Parser_Token::ttl_key_op:
            return ttl_key_op();
            break;
        default:
            return syntax_error();
            break;
    }
}

Parser::~Parser()
{
}

Parser::Parser()
{
}

Parser::Parser(const std::string &input)
    : input(input)
{
    //    int a = split();
    //    parser_result.get()->setOptype(Parser_Token::syntax_error);
    //    parser_result.get()->setResult(NULL);
    //    this->input = input;
}

void Parser::setInput(const std::string &input)
{
    Parser::input = input;
}

void Parser::set_SplitResult_NULL()
{
    std::vector<std::string>().swap(split_result);
}

void Parser::split_printf()
{
    for (int i = 0; i < split_result.size(); ++i) {
        std::cout << split_result[i] << "......" << std::endl;
    }
}


// bool string_is_int(std::string &str) {
//     if (str.size()==0){
//         return false;
//     }
//     for (std::string::size_type i = 0; i < str.size(); ++i) {
//         if (!std::isdigit(str[i])){
//             return false;
//         }
//     }
//     return true;
// }



//判断string是否可以转换为int的函数
