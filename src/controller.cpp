//
// Created by 86136 on 2022/5/16.
//
#include "controller.h"
//Controller::Controller() {}
int Controller::run(std::string input)
{
    parser.setInput(input);
    auto op_result = parser.run();
    parser.set_SplitResult_NULL();
    switch (op_result.get()->getOptype()) {
    case Parser_Token::key_list_op:
        // data

        break;
    case Parser_Token::exists_key_op:
        // data

        break;
    case Parser_Token::type_key_op:
        // data

        break;
    case Parser_Token::del_key_op:
        // data
        break;
    case Parser_Token::dbsize_op:
        // data
        break;
    case Parser_Token::flushdb_op:
        // data

        break;
    case Parser_Token::flushall_op:
        // data
        break;
    case Parser_Token::select_n_op:

        // data
        break;
    case Parser_Token::get_key_op:

        break;
    case Parser_Token::set_key_value_op:

        break;
    case Parser_Token::strlen_key:

        break;
    case Parser_Token::append_key_value_op:

        break;
    case Parser_Token::getrange_key_start_end:

        break;
    default:
        break;
    }
    //    std::cout<<op_result.get()->getOptype()<<std::endl;
    return 0;
}

Controller::Controller()
{
    this->server = getInstanceOfServer();
}