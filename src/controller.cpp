//
// Created by 86136 on 2022/5/16.
//
#include "controller.h"
// Controller::Controller() {}
int Controller::run(std::string input)
{
    parser.setInput(input);
    auto parser_result = parser.run();
    parser.set_SplitResult_NULL();
    switch (parser_result.get()->getOptype()) {
    case Parser_Token::key_list_op: {
        auto data_result = server.get_all_keys(client.db_id);
        break;
    }
    case Parser_Token::exists_key_op: {
        auto parser_res = dynamic_cast<exist_key_result *>(parser_result.get());
        auto data_result = server.exists_key(client.db_id, parser_res->getKeyN());
        break;
    }
    case Parser_Token::type_key_op: {
        auto parser_res = dynamic_cast<type_key_result *>(parser_result.get());
        auto data_result = server.get_key_type(client.db_id, parser_res->getKeyN());
        break;
    }
    case Parser_Token::del_key_op: {
        auto parser_res = dynamic_cast<del_key_result *>(parser_result.get());
        auto data_result = server.del_key(client.db_id, parser_res->getKeyN());
        break;
    }
    case Parser_Token::dbsize_op: {
        auto data_result = server.get_dbsize(client.db_id);
        break;
    }
    case Parser_Token::flushdb_op: {
        auto data_result = server.flushdb(client.db_id);
    }
    case Parser_Token::flushall_op: {
        auto data_result = server.flushall();
        break;
    }
    case Parser_Token::select_n_op: {
        auto parser_res = dynamic_cast<select_db_result *>(parser_result.get());
        client.db_id = parser_res->getDbN();
        auto data_result = server.select_db(client.db_id, client.client_id);
        break;
    }
    case Parser_Token::get_key_op: {
        auto key = dynamic_cast<get_key_result *>(parser_result.get())->getKey();
        auto data_result = server.get_key(client.db_id, key);
        break;
    }
    case Parser_Token::set_key_value_op: {
        auto parser_res = dynamic_cast<set_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        auto data_result = server.set_key(client.db_id, key, value);
        break;
    }
    case Parser_Token::strlen_key: {
        auto key = dynamic_cast<strlen_result *>(parser_result.get())->getKey();
        auto data_result = server.get_strlen(client.db_id, key);
        break;
    }
    case Parser_Token::append_key_value_op: {
        auto parser_res = dynamic_cast<append_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        std::string value = parser_res->getValue();
        server.append_value(client.db_id, key, value);
        break;
    }
    case Parser_Token::getrange_key_start_end: {
        auto parser_res = dynamic_cast<getrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        server.getrange(client.db_id, key, start, end);
    }
    default:
        // TODO: ERROR
        break;
    }
    return 0;
}

Controller::Controller()
    : server(ERedisServer())
    , parser(Parser())
    , client(ERedisClient())
{
}