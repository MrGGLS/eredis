/**
 * Controller
 *
 * @authors Xiao Jianwei
 *          Zhang Jing
 *          Mr.GGLS
 */

#include "controller.h"
#include "erdb.hpp"

std::string Controller::run(std::string input)
{
    parser.setInput(input);
    auto parser_result = parser.run();
    parser.set_SplitResult_NULL();
    switch (parser_result.get()->getOptype()) {
    case Parser_Token::key_list_op: {
        return server.get_all_keys(client->db_id);
    }
    case Parser_Token::exists_key_op: {
        auto parser_res = dynamic_cast<exist_key_result *>(parser_result.get());
        return server.exists_key(client->db_id, parser_res->getKeyN());
    }
    case Parser_Token::type_key_op: {
        auto parser_res = dynamic_cast<type_key_result *>(parser_result.get());
        return server.get_key_type(client->db_id, parser_res->getKeyN());
    }
    case Parser_Token::del_key_op: {
        auto parser_res = dynamic_cast<del_key_result *>(parser_result.get());
        return server.del_key(client->db_id, parser_res->getKeyN());
    }
    case Parser_Token::dbsize_op: {
        return server.get_dbsize(client->db_id);
    }
    case Parser_Token::flushdb_op: {
        return server.flushdb(client->db_id);
    }
    case Parser_Token::flushall_op: {
        return server.flushall();
    }
    case Parser_Token::select_n_op: {
        auto parser_res = dynamic_cast<select_db_result *>(parser_result.get());
        client->db_id = parser_res->getDbN();
        return server.select_db(client->db_id, client->client_id);
    }
    case Parser_Token::get_key_op: {
        auto key = dynamic_cast<get_key_result *>(parser_result.get())->getKey();
        return server.get_key(client->db_id, key);
    }
    case Parser_Token::set_key_value_op: {
        auto parser_res = dynamic_cast<set_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        return server.set_key(client->db_id, key, value);
    }
    case Parser_Token::strlen_key: {
        auto key = dynamic_cast<strlen_result *>(parser_result.get())->getKey();
        return server.get_strlen(client->db_id, key);
    }
    case Parser_Token::append_key_value_op: {
        auto parser_res = dynamic_cast<append_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        std::string value = parser_res->getValue();
        return server.append_value(client->db_id, key, value);
    }
    case Parser_Token::getrange_key_start_end: {
        auto parser_res = dynamic_cast<getrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        std::cout << "key: " << key << std::endl;
        return server.getrange(client->db_id, key, start, end);
    }
    case Parser_Token::incr_key_op: {
        auto parser_res = dynamic_cast<incr_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.incr(client->db_id, key);
    }
    case Parser_Token::decr_key_op: {
        auto parser_res = dynamic_cast<decr_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.decr(client->db_id, key);
    }
    case Parser_Token::lpush_key_list_op: {
        auto parser_res = dynamic_cast<lpush_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        return server.lpush(client->db_id, key, value);
    }
    case Parser_Token::rpush_key_list_op: {
        auto parser_res = dynamic_cast<rpush_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();

        return server.rpush(client->db_id, key, value);
    }
    case Parser_Token::lrange_key_op: {
        auto parser_res = dynamic_cast<lrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        return server.lrange(client->db_id, key, start, end);
    }
    case Parser_Token::lpop_key_op: {
        auto parser_res = dynamic_cast<lpop_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.lpop(client->db_id, key);
    }
    case Parser_Token::rpop_key_op: {
        auto parser_res = dynamic_cast<rpop_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.rpop(client->db_id, key);
    }
    case Parser_Token::lindex_key_op: {
        auto parser_res = dynamic_cast<lindex_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int index = parser_res->getIndex();
        return server.lindex(client->db_id, key, index);
    }
    case Parser_Token::llen_key_op: {
        auto parser_res = dynamic_cast<llen_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.llen(client->db_id, key);
    }
    case Parser_Token::lset_key_index_op: {
        auto parser_res = dynamic_cast<lset_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int index = parser_res->getIndex();
        std::string value = parser_res->getValue();
        return server.lset(client->db_id, key, index, value);
    }
    case Parser_Token::expire_key_op: {
        auto parser_res = dynamic_cast<expire_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int time = parser_res->getTime();
        return server.set_expire(client->db_id, key, time);
    }
    case Parser_Token::setex_key_op: {
        auto parser_res = dynamic_cast<setex_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int time = parser_res->getTime();
        ERObject value = parser_res->getValue();
        return server.setex(client->db_id, key, time, value);
    }
    case Parser_Token::ttl_key_op: {
        auto parser_res = dynamic_cast<ttl_key_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return server.ttl(client->db_id, key);
    }
    case Parser_Token::save_op: {
        if (save_data(&server)) {
            return "save succeed";
        } else
            return "save failed";
    }
    case Parser_Token::syntax_error: {
        return "syntax error";
    }
    case Parser_Token::arguments_error: {
        return "ERR wrong number of arguments";
    }
    case Parser_Token::no_vaild_error: {
        return "Invalid argument(s)";
    }
    default:
        // TODO: ERROR
        break;
    }
    return "Unknown Command";
}

Controller::Controller()
    : server(ERedisServer())
    , parser(Parser())
    , client(nullptr)
{
    load_data(&server);
}