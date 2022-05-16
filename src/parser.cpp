//
// Created by 86136 on 2022/5/9.
//
#include "parser.h"

//操作对象的返回函数
std::unique_ptr<op_result> parser::key_op()
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
std::unique_ptr<op_result> parser::exist_key_op()
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

std::unique_ptr<op_result> parser::type_key_op()
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

std::unique_ptr<op_result> parser::del_key_op()
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

std::unique_ptr<op_result> parser::dbsize_op()
{
    std::unique_ptr<dbsize_result> res = std::make_unique<dbsize_result>(Parser_Token::dbsize_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> parser::flushdb_op()
{
    std::unique_ptr<flushdb_result> res = std::make_unique<flushdb_result>(Parser_Token::flushdb_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> parser::flushall_op()
{
    std::unique_ptr<flushall_result> res = std::make_unique<flushall_result>(Parser_Token::flushall_op, "");
    if (split_result.size() == 1) {
        return res;
    } else {
        res.get()->setOptype(Parser_Token::arguments_error);
        return res;
    }
}

std::unique_ptr<op_result> parser::select_db_op()
{
    std::unique_ptr<select_db_result> res = std::make_unique<select_db_result>(Parser_Token::select_n_op, "", 0);

    if (split_result.size() == 2) {
        if (string_to_int(split_result[1])) {
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

std::unique_ptr<op_result> parser::get_key_op()
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

std::unique_ptr<op_result> parser::strlen_op()
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

std::unique_ptr<op_result> parser::append_key_value_op()
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

std::unique_ptr<op_result> parser::getstrange_op()
{
    std::unique_ptr<getrange_result> res = std::make_unique<getrange_result>(Parser_Token::getrange_key_start_end, "",
        "", 0, 0);
    if (4 == split_result.size()) {
        if (string_to_int(split_result[2]) && string_to_int(split_result[3])) {
            res.get()->setStart(std::stoi(split_result[2]));
            res.get()->setAnEnd(std::stoi(split_result[3]));
            return res;
        } else {
            res.get()->setOptype(Parser_Token::no_vaild_error);
            return res;
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

// op_result类的函数
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

set_key_value_result::set_key_value_result(int optype, const std::string &result, const std::string &key,
    const std::string &value)
    : op_result(optype, result)
    , key(key)
    , value(value)
{
}

const std::string &set_key_value_result::getKey() const
{
    return key;
}

const std::string &set_key_value_result::getValue() const
{
    return value;
}

void set_key_value_result::setKey(const std::string &key)
{
    set_key_value_result::key = key;
}

void set_key_value_result::setValue(const std::string &value)
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

/*
 * parser的构造函数，需要解决的问题
 * 1. 将传入的字符串按照” “分割
 * 2. 根据字符串中的双引号判断语法是否正确，双引号用来规定字符串，除此之外出现在任何地方都是语法错误
 * */
// parser::parser(std::string& input) {
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

Parser_Token parser::split()
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

    if (split_result[0] == "keys") {
        return Parser_Token::key_list_op;
    } else if (split_result[0] == "exists") {
        return Parser_Token::exists_key_op;
    } else if (split_result[0] == "type") {
        return Parser_Token::type_key_op;
    } else if (split_result[0] == "del") {
        return Parser_Token::del_key_op;
    } else if (split_result[0] == "dbsize") {
        return Parser_Token::dbsize_op;
    } else if (split_result[0] == "flushdb") {
        return Parser_Token::flushdb_op;
    } else if (split_result[0] == "flushall") {
        return Parser_Token::flushall_op;
    } else if (split_result[0] == "select") {
        return Parser_Token::select_n_op;
    } else if (split_result[0] == "set") {
        return Parser_Token::set_key_value_op;
    } else if (split_result[0] == "get") {
        return Parser_Token::get_key_op;
    } else if (split_result[0] == "strlen") {
        return Parser_Token::strlen_key;
    } else if (split_result[0] == "append") {
        return Parser_Token::append_key_value_op;
    } else if (split_result[0] == "getrange") {
        return Parser_Token::getrange_key_start_end;
    } else {
        return Parser_Token::syntax_error;
    }
}

std::unique_ptr<op_result> parser::syntax_error()
{

    return std::make_unique<op_result>(Parser_Token::syntax_error, "");
}

std::unique_ptr<op_result> parser::set_key_value_op()
{
    std::unique_ptr<set_key_value_result> res = std::make_unique<set_key_value_result>(Parser_Token::append_key_value_op,
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

std::unique_ptr<op_result> parser::run()
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
    default:
        return syntax_error();
        break;
    }
}

parser::~parser()
{
}
parser::parser()
{
}
parser::parser(const std::string &input)
    : input(input)
{
    //    int a = split();
    //    parser_result.get()->setOptype(Parser_Token::syntax_error);
    //    parser_result.get()->setResult(NULL);
    //    this->input = input;
}

void parser::setInput(const std::string &input)
{
    parser::input = input;
}

void parser::set_SplitResult_NULL()
{
    std::vector<std::string>().swap(split_result);
}

void parser::split_printf()
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

bool string_to_int(std::string &str)
{
    if (str.size() == 0) {
        return false;
    }
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}