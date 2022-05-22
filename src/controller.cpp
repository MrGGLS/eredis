/**
 * Controller
 *
 * @authors Xiao Jianwei
 *          Zhang Jing
 *          Mr.GGLS
 */

#include "controller.h"
#include "erdb.hpp"
#include "eredis.hpp"
#include "json.hpp"

std::string Controller::run(std::string input)
{
    parser.setInput(input);
    auto parser_result = parser.run();
    parser.set_SplitResult_NULL();
    switch (parser_result.get()->getOptype()) {
    case Parser_Token::key_list_op: {
        return set_json(server.get_all_keys(client->db_id), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::exists_key_op: {
        auto parser_res = dynamic_cast<exist_key_result *>(parser_result.get());
        std::string data_result = server.exists_key(client->db_id, parser_res->getKeyN());
        return set_json(data_result, SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::type_key_op: {
        auto parser_res = dynamic_cast<type_key_result *>(parser_result.get());
        std::string data_result = server.get_key_type(client->db_id, parser_res->getKeyN());
        return set_json(data_result, SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::del_key_op: {
        auto parser_res = dynamic_cast<del_key_result *>(parser_result.get());
        return set_json(server.del_key(client->db_id, parser_res->getKeyN()), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::dbsize_op: {
        return set_json(server.get_dbsize(client->db_id), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::flushdb_op: {
        return set_json(server.flushdb(client->db_id), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::flushall_op: {
        return set_json(server.flushall(), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::select_n_op: {
        auto parser_res = dynamic_cast<select_db_result *>(parser_result.get());
        client->db_id = parser_res->getDbN();
        return set_json(server.select_db(client->db_id, client->client_id), SERVER_EXEC_CHANGE_DB, &client->db_id);
    }
    case Parser_Token::get_key_op: {
        auto key = dynamic_cast<get_key_result *>(parser_result.get())->getKey();
        return set_json(server.get_key(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::set_key_value_op: {
        auto parser_res = dynamic_cast<set_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        return set_json(server.set_key(client->db_id, key, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::strlen_key: {
        auto key = dynamic_cast<strlen_result *>(parser_result.get())->getKey();
        return set_json(server.get_strlen(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::append_key_value_op: {
        auto parser_res = dynamic_cast<append_key_value_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        std::string value = parser_res->getValue();
        return set_json(server.append_value(client->db_id, key, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::getrange_key_start_end: {
        auto parser_res = dynamic_cast<getrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        std::cout << "key: " << key << std::endl;
        return set_json(server.getrange(client->db_id, key, start, end), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::incr_key_op: {
        auto parser_res = dynamic_cast<incr_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.incr(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::decr_key_op: {
        auto parser_res = dynamic_cast<decr_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.decr(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::lpush_key_list_op: {
        auto parser_res = dynamic_cast<lpush_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();
        return set_json(server.lpush(client->db_id, key, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::rpush_key_list_op: {
        auto parser_res = dynamic_cast<rpush_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        ERObject value = parser_res->getValue();

        return set_json(server.rpush(client->db_id, key, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::lrange_key_op: {
        auto parser_res = dynamic_cast<lrange_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int32_t start = parser_res->getStart();
        int32_t end = parser_res->getAnEnd();
        return set_json(server.lrange(client->db_id, key, start, end), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::lpop_key_op: {
        auto parser_res = dynamic_cast<lpop_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.lpop(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::rpop_key_op: {
        auto parser_res = dynamic_cast<rpop_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.rpop(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::lindex_key_op: {
        auto parser_res = dynamic_cast<lindex_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int index = parser_res->getIndex();
        return set_json(server.lindex(client->db_id, key, index), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::llen_key_op: {
        auto parser_res = dynamic_cast<llen_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.llen(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::lset_key_index_op: {
        auto parser_res = dynamic_cast<lset_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int index = parser_res->getIndex();
        std::string value = parser_res->getValue();
        return set_json(server.lset(client->db_id, key, index, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::expire_key_op: {
        auto parser_res = dynamic_cast<expire_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int time = parser_res->getTime();
        return set_json(server.set_expire(client->db_id, key, time), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::setex_key_op: {
        auto parser_res = dynamic_cast<setex_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        int time = parser_res->getTime();
        ERObject value = parser_res->getValue();
        return set_json(server.setex(client->db_id, key, time, value), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::ttl_key_op: {
        auto parser_res = dynamic_cast<ttl_key_result *>(parser_result.get());
        std::string key = parser_res->getKey();
        return set_json(server.ttl(client->db_id, key), SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::save_op: {
        if (save_data(&server)) {
            return set_json("save succeed", SERVER_EXEC_RETURN, NULL);
        } else
            return set_json("save failed", SERVER_EXEC_RETURN, NULL);
    }
    case Parser_Token::syntax_error: {
        return set_json("syntax error", SERVER_SYNTAX_ERR, NULL);
    }
    case Parser_Token::arguments_error: {
        return set_json("ERR wrong number of arguments", SERVER_SYNTAX_ERR, NULL);
    }
    case Parser_Token::no_vaild_error: {
        return set_json("Invalid argument(s)", SERVER_SYNTAX_ERR, NULL);
    }
    default:
        // TODO: ERROR
        break;
    }
    return set_json("Unknown Command", SERVER_SYNTAX_ERR, NULL);
}

Controller::Controller()
    : server(ERedisServer())
    , parser(Parser())
    , client(nullptr)
{
    load_data(&server);
}
std::string Controller::set_json(std::string msg, int type, int *value)
{
    nlohmann::json j;
    j["message"] = msg;
    j["type"] = type;
    if (value != nullptr)
        j["value"] = *value;
    return j.dump();
}
