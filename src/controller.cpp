//
// Created by 86136 on 2022/5/16.
//
#include "controller.h"
#include "erdb.hpp"
// Controller::Controller() {}
std::string Controller::run(std::string input)
{
    parser.setInput(input);
    auto parser_result = parser.run();
    parser.set_SplitResult_NULL();
    switch (parser_result.get()->getOptype()) {
    case Parser_Token::key_list_op: {
        return server.get_all_keys(client.db_id);
    }
    case Parser_Token::exists_key_op: {
        auto parser_res = dynamic_cast<exist_key_result *>(parser_result.get());
        return server.exists_key(client.db_id, parser_res->getKeyN());
    }
    case Parser_Token::type_key_op: {
        auto parser_res = dynamic_cast<type_key_result *>(parser_result.get());
        return  server.get_key_type(client.db_id, parser_res->getKeyN());
    }
    case Parser_Token::del_key_op: {
        auto parser_res = dynamic_cast<del_key_result *>(parser_result.get());
        return server.del_key(client.db_id, parser_res->getKeyN());
    }
    case Parser_Token::dbsize_op: {
        return server.get_dbsize(client.db_id);
    }
    case Parser_Token::flushdb_op: {
        return server.flushdb(client.db_id);
    }
    case Parser_Token::flushall_op: {
        return server.flushall();
    }
    case Parser_Token::select_n_op: {
        auto parser_res = dynamic_cast<select_db_result *>(parser_result.get());
        client.db_id = parser_res->getDbN();
        return server.select_db(client.db_id, client.client_id);
    }
    case Parser_Token::get_key_op: {
        auto key = dynamic_cast<get_key_result *>(parser_result.get())->getKey();
        return server.get_key(client.db_id, key);
    }
    case Parser_Token::set_key_value_op: {
        auto parser_res = dynamic_cast<set_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        return server.set_key(client.db_id, key, value);
    }
    case Parser_Token::strlen_key: {
        auto key = dynamic_cast<strlen_result *>(parser_result.get())->getKey();
        return server.get_strlen(client.db_id, key);
    }
    case Parser_Token::append_key_value_op: {
        auto parser_res = dynamic_cast<append_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        std::string value = parser_res->getValue();
        return server.append_value(client.db_id, key, value);
    }
    case Parser_Token::getrange_key_start_end: {
        auto parser_res = dynamic_cast<getrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        return server.getrange(client.db_id, key, start, end);
    }
    default:
        // TODO: ERROR
        break;
    }
    return "UNKOWN COMMAND";
}

Controller::Controller()
    : server(ERedisServer())
    , parser(Parser())
    , client(ERedisClient())
{
    load_data(&server);
}