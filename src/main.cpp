#include "controller.h"
#include "erdb.hpp"
#include "eredis.hpp"
#include "json.hpp"
#include "utils.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif __APPLE__
#include <arpa/inet.h>
#include <netdb.h>
#include <pcap/socket.h>
#include <sys/event.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

void testList(ERedisServer &server);
void testExpire(ERedisServer &server);
void testServer(ERedisServer &server);
void testInt_Double(ERedisServer &server);
void testSave();
[[noreturn]] void event_loop();

static const auto LOGO = {
    "▓█████  ██▀███    ██████ ▓█████  ██▀███   ██▒   █▓▓█████  ██▀███\n"
    "▓█   ▀ ▓██ ▒ ██▒▒██    ▒ ▓█   ▀ ▓██ ▒ ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n"
    "▒███   ▓██ ░▄█ ▒░ ▓██▄   ▒███   ▓██ ░▄█ ▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n"
    "▒▓█  ▄ ▒██▀▀█▄    ▒   ██▒▒▓█  ▄ ▒██▀▀█▄    ▒██ █░░▒▓█  ▄ ▒██▀▀█▄\n"
    "░▒████▒░██▓ ▒██▒▒██████▒▒░▒████▒░██▓ ▒██▒   ▒▀█░  ░▒████▒░██▓ ▒██▒\n"
    "░░ ▒░ ░░ ▒▓ ░▒▓░▒ ▒▓▒ ▒ ░░░ ▒░ ░░ ▒▓ ░▒▓░   ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n"
    "░ ░  ░  ░▒ ░ ▒░░ ░▒  ░ ░ ░ ░  ░  ░▒ ░ ▒░   ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n"
    "░     ░░   ░ ░  ░  ░     ░     ░░   ░      ░░     ░     ░░   ░\n"
    "░  ░   ░           ░     ░  ░   ░           ░     ░  ░   ░\n"
    "░\n"
};

void logo()
{
    std::cout << "\n";
#ifdef __APPLE__
    std::cout << "\x1b[36m";
#endif
    for (auto iter : LOGO) {
        std::cout << iter;
    }
#ifdef __APPLE__
    std::cout << "\x1b[0m";
#endif
    std::cout << "\n";
    std::cout << "<host>: 127.0.0.1\n"
              << "<port>: " << SERVER_PORT
              << std::endl;
}

int main(int argc, char **argv)
{
    logo();
    event_loop();
}

void event_loop()
{
#ifdef _WIN32
    /* init WS DLL for process */
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        std::cerr << "Sorry, can't initialize...." << std::endl;
    }
#endif

    /* create server socket */
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    SOCKET max_fd = server_socket;
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Sorry, can't create server socket...." << std::endl;
    }

    /* socket config */
    sockaddr_in hint;
    hint.sin_family = AF_INET; // ipv4
    hint.sin_port = htons(SERVER_PORT); // port
#ifdef _WIN32
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
#elif __APPLE__
    hint.sin_addr.s_addr = INADDR_ANY;
#endif

    /* bind addr */
    if (bind(server_socket, (sockaddr *)&hint, sizeof(hint)) < 0) {
        std::cerr << "bind error" << std::endl;
        exit(-1);
    }

    /* set listening & max backlog */
    listen(server_socket, MAX_BACKLOG);

    /* init controller*/
    Controller controller;
    load_data(&(controller.server));

#ifdef __APPLE__
    int kq = kqueue();
    struct kevent changes;
    EV_SET(&changes, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    kevent(kq, &changes, 1, NULL, 0, NULL);
#elif __WIN32
    fd_set master;
    FD_ZERO(&master);
    FD_SET(server_socket, &master);
#endif

    while (true) {

#ifdef _WIN32
        /* we don't want our master to be destroyed */
        fd_set copy = master;
        int client_num = select(0, &copy, nullptr, nullptr, nullptr);
#elif __APPLE__
        struct kevent events[FD_SETSIZE];
        int client_num = kevent(kq, NULL, 0, events, FD_SETSIZE, NULL);
#endif

        if (client_num < 0) {
            log_err("client num < 0\n");
        } else if (client_num == 0) {
            log_system("client equals zero\n");
        }

        // check all socket

        for (int i = 0; i < client_num; i++) {

#ifdef _WIN32
            SOCKET sock = copy.fd_array[i];
#elif __APPLE__
            struct kevent event = events[i];
            SOCKET sock = event.flags & EV_ERROR ? -1 : event.ident;
            if (sock == -1) {
                log_err("sock == -1\n");
                continue;
            }
#endif

            if (sock == server_socket) {
                /* can't get more connections */
                if (client_num >= MAX_BACKLOG) {
                    continue;
                } else {
                    sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    /* accept new client*/
                    SOCKET new_client = accept(server_socket, (sockaddr *)&client_addr, &addr_len);
                    max_fd = std::max(new_client, max_fd);

#ifdef _WIN32
                    FD_SET(new_client, &master);
#elif __APPLE__
                    struct kevent new_event;
                    EV_SET(&new_event, new_client, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
                    kevent(kq, &new_event, 1, NULL, 0, NULL);
#endif
                    /* complete new client info */
                    auto new_eclient = new ERedisClient();
                    new_eclient->client_id = new_client;
                    new_eclient->db_id = 0;
                    new_eclient->c_time = time(0);
                    new_eclient->last_interaction = time(0);
                    new_eclient->client_name = "eclient" + std::to_string(new_client);

                    char host[NI_MAXHOST];
                    char port[NI_MAXSERV];
#ifdef _WIN32
                    ZeroMemory(host, NI_MAXHOST);
                    ZeroMemory(port, NI_MAXSERV);
#elif __APPLE__
                    memset(host, 0, NI_MAXHOST);
                    memset(port, 0, NI_MAXSERV);
#endif

                    if (getnameinfo((sockaddr *)&client_addr, addr_len, host, NI_MAXHOST, port, NI_MAXSERV, 0) == 0) {
                        std::stringstream ss;
                        ss << "connect to client <host>: " << host << " <port>: " << port << std::endl;
                        log_system(ss.str());

                    } else {
                        inet_ntop(AF_INET, &client_addr.sin_addr, host, NI_MAXHOST);
                        std::stringstream ss;
                        ss << "connect to client <host>: " << host << " <port>: " << ntohs(client_addr.sin_port)
                           << std::endl;
                        log_system(ss.str());
                    }
                    new_eclient->hostname = std::string(host);
                    new_eclient->port = std::to_string(ntohs(client_addr.sin_port));
                    /* put new client into server */
                    std::lock_guard<std::mutex> lg(*(controller.server.cli_mtx));
                    controller.server.clients[new_client] = new_eclient;
                    std::string msg = "Welcome to ERedis server!";
                    nlohmann::json j;
                    j["type"] = 5;
                    j["message"] = msg.c_str();
                    send(new_client, j.dump().c_str(), j.dump().length(), 0);
                }
            } else {

                char buffer[BUFFER_LEN];
#ifdef _WIN32
                ZeroMemory(buffer, BUFFER_LEN);
#elif __APPLE__
                memset(buffer, 0, BUFFER_LEN);
#endif

                int bytes_in = recv(sock, buffer, BUFFER_LEN, 0);
                //                assert(bytes_in > 0);
                std::lock_guard<std::mutex> lg(*(controller.server.cli_mtx));
                if (controller.server.clients.count(sock) <= 0 || bytes_in <= 0) {
#ifdef _WIN32
                    closesocket(sock);
                    FD_CLR(sock, &master);
#elif __APPLE__
                    close(sock);
                    struct kevent del_event;
                    EV_SET(&del_event, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &del_event, 1, NULL, 0, NULL);
#endif
                    std::stringstream ss;
                    ss << "Disconnect to client <host>: " << controller.server.clients[sock]->hostname
                       << " <port>: " << controller.server.clients[sock]->port
                       << std::endl;
                    log_warn(ss.str());
                    controller.server.clients.erase(sock);
                } else {
                    std::cout << "from client [" << sock << "] received command: " << buffer << std::endl;
                    /* change current client */
                    controller.client = controller.server.clients[sock];
                    auto res = controller.run(std::string(buffer));
                    std::cout << "result after executing:\n"
                              << res << std::endl;
                    // if (to_upper(res) == REDIS_EXIT) {
                    //     std::lock_guard<std::mutex> lg2(*(controller.server.cli_mtx));
                    //#ifdef _WIN32
                    //     closesocket(sock);
                    //     FD_CLR(sock, &master);
                    //#elif __APPLE__
                    //     close(sock);
                    //     struct kevent del_event;
                    //     EV_SET(&del_event, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    //     kevent(kq, &del_event, 1, NULL, 0, NULL);
                    //#endif
                    //     std::stringstream ss;
                    //     ss << "Disconnect to client <host>: " << controller.server.clients[sock]->hostname
                    //     << " <port>: " << controller.server.clients[sock]->port
                    //     << std::endl;
                    //     log_warn(ss.str());
                    //     controller.server.clients.erase(sock);
                    //     } else {
                    nlohmann::json j;
                    j["type"] = 6;
                    j["message"] = res.c_str();
                    std::cout << j << std::endl;
                    send(sock, j.dump().c_str(), j.dump().length(), 0);
                    //    }
                }
            }
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif
}

void testSave()
{
    auto server = ERedisServer();
    testList(server);
    testExpire(server);
    testServer(server);
    testInt_Double(server);
    save_data(&server);
    //    server = ERedisServer();
    load_data(&server);
    std::cout << "-------------load data---------------" << std::endl;
    std::cout << server.get_key_type(0, "k1") << std::endl;
    std::cout << server.get_key(0, "k1") << std::endl;
    std::cout << server.get_key_type(0, "k2") << std::endl;
    std::cout << server.get_key(0, "k2") << std::endl;
    testList(server);
    save_data(&server);
    testExpire(server);
    save_data(&server);
    testServer(server);
    save_data(&server);
    testInt_Double(server);
    save_data(&server);
}

void testInt_Double(ERedisServer &server)
{
    auto value1 = 123;
    auto value2 = 123.123;
    ERObject o1 = ERObject(ObjectType::EREDIS_INT, (void *)&value1);
    ERObject o2 = ERObject(ObjectType::EREDIS_DOUBLE, (void *)&value2);
    std::cout << "test integer" << std::endl;
    std::cout << server.set_key(0, "k1", o1) << std::endl;
    std::cout << server.get_key_type(0, "k1") << std::endl;
    std::cout << server.get_key(0, "k1") << std::endl;
    /* incr/decr */
    std::cout << server.incr(0, "k1") << std::endl;
    std::cout << server.decr(0, "k1") << std::endl;
    std::cout << "test double" << std::endl;
    std::cout << server.set_key(0, "k2", o2) << std::endl;
    std::cout << server.get_key_type(0, "k2") << std::endl;
    std::cout << server.get_key(0, "k2") << std::endl;
    std::cout << server.incr(0, "k2") << std::endl;
    std::cout << server.decr(0, "k2") << std::endl;
}

void testList(ERedisServer &server)
{
    auto value1 = new std::vector<std::string> { "hehe", "haha", "Mr.GGLS!" };
    auto value2 = new std::vector<std::string> { "wanqian", "heihei", "I'm Mr.GGLS!" };
    ERObject object1 = ERObject(ObjectType::EREDIS_LIST, (void *)value1);
    ERObject object2 = ERObject(ObjectType::EREDIS_LIST, (void *)value2);
    /* test set key */
    std::cout << "test lpush " << std::endl;
    std::cout << server.lpush(0, "k1", object1) << std::endl;
    std::cout << server.lpush(0, "k1", object2) << std::endl;
    std::cout << server.rpush(0, "k2", object1) << std::endl;
    std::cout << server.rpush(0, "k2", object2) << std::endl;
    /* test get range */
    std::cout << "test lrange " << std::endl;
    std::cout << server.lrange(0, "k1", 2, 4) << std::endl;
    std::cout << server.lrange(0, "k1", 2, 100) << std::endl;
    std::cout << server.lrange(0, "k1", 0, -1) << std::endl;
    /* test lpop/rpop */
    std::cout << "test lpop" << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lrange(0, "k1", 0, -1) << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lrange(0, "k1", 0, -1) << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << server.lpop(0, "k1") << std::endl;
    std::cout << "test rpop" << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.lrange(0, "k2", 0, -1) << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.lrange(0, "k2", 0, -1) << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    std::cout << server.rpop(0, "k2") << std::endl;
    /* test llen */
    std::cout << "test llen " << std::endl;
    std::cout << server.lpush(0, "k1", object1) << std::endl;
    std::cout << server.lpush(0, "k1", object2) << std::endl;
    std::cout << server.rpush(0, "k2", object1) << std::endl;
    std::cout << server.rpush(0, "k2", object2) << std::endl;
    std::cout << server.llen(0, "k1") << std::endl;
    /* test lset */
    std::cout << "test lset " << std::endl;
    std::cout << server.lset(0, "k1", 1, "GGLS") << std::endl;
    std::cout << server.lrange(0, "k1", 0, 3) << std::endl;
    std::cout << server.lset(0, "k1", 1, "Mr.GGLS") << std::endl;
    std::cout << server.lrange(0, "k1", 0, 3) << std::endl;
}

void testExpire(ERedisServer &server)
{
    std::string *value = new std::string("hehe");
    ERObject object = ERObject(ObjectType::EREDIS_STRING, (void *)value);
    //    std::cout<<object.get_str()<<std::endl;
    /* test set key */
    std::cout << "test set expired key" << std::endl;
    std::cout << server.setex(0, "k1", 5, object) << std::endl;
    /* test get expired key */
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "test get expired key" << std::endl;
        std::cout << server.get_key(0, "k1") << std::endl;
        std::cout << server.ttl(0, "k1") << std::endl;
    }
    /* test change expire */
    object.set_string("I'm Mr.GGLS!");
    std::cout << server.set_key(0, "k2", object) << std::endl;
    std::cout << "before set expire................" << std::endl;
    std::cout << server.ttl(0, "k2") << std::endl;
    std::cout << server.set_expire(0, "k2", 8) << std::endl;
    std::cout << "after set expire................" << std::endl;
    std::cout << server.ttl(0, "k2") << std::endl;
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "test get expired key" << std::endl;
        std::cout << server.get_key(0, "k2") << std::endl;
        std::cout << server.ttl(0, "k2") << std::endl;
    }
    std::cout << "-------end of test-------" << std::endl;
}

void testServer(ERedisServer &server)
{

    //    auto server = ERedisServer();

    //    std::thread check_keys_thread(clear_invalid_keys,&server);
    //    check_keys_thread.detach();
    //    std::thread check_clients_thread(clear_idle_clients,&server);
    //    check_clients_thread.detach();

    std::string *value = new std::string("hehe");
    ERObject object = ERObject(ObjectType::EREDIS_STRING, (void *)value);
    //    std::cout<<object.get_str()<<std::endl;
    /* test set key */
    std::cout << "test set key" << std::endl;
    std::cout << server.set_key(0, "k1", object) << std::endl;
    /* test get key */
    std::cout << "test get key" << std::endl;
    std::cout << server.get_key(0, "k1") << std::endl;
    /* get not exist key */
    std::cout << "get not exist key" << std::endl;
    std::cout << server.get_key(0, "k2") << std::endl;
    /* delete same key two times */
    std::cout << "delete same key two times" << std::endl;
    std::cout << server.del_key(0, "k1") << std::endl;
    std::cout << server.del_key(0, "k1") << std::endl;
    /* test get range */
    std::cout << "test get range" << std::endl;
    std::cout << server.set_key(0, "k1", object) << std::endl;
    std::cout << server.getrange(0, "k1", 1, 3) << std::endl;
    /* test append */
    std::cout << "test append" << std::endl;
    std::cout << server.append_value(0, "k1", "hihi") << std::endl;
    std::cout << server.get_strlen(0, "k1") << std::endl;
    /* test get key type */
    std::cout << "delete get key type" << std::endl;
    std::cout << server.get_key_type(0, "k1") << std::endl;

    std::cout << server.set_key(0, "k1", object) << std::endl;
    std::cout << server.set_key(0, "k2", object) << std::endl;
    std::cout << server.set_key(0, "k3", object) << std::endl;
    /* test get all keys */
    std::cout << "test get all keys" << std::endl;

    std::cout << server.get_all_keys(0) << std::endl;
    /* test get db size */
    std::cout << "test get db size" << std::endl;
    std::cout << server.get_dbsize(0) << std::endl;
    /* test exists key */
    std::cout << "test exists key" << std::endl;
    std::cout << server.exists_key(0, "k4") << std::endl;
    std::cout << server.exists_key(0, "k2") << std::endl;
    /* test flush db*/
    std::cout << "test flush db" << std::endl;
    std::cout << server.flushdb(0) << std::endl;
    std::cout << server.get_dbsize(0) << std::endl;
    /* test flush all */
    std::cout << "test flush all" << std::endl;
    std::cout << server.set_key(0, "k1", object) << std::endl;
    std::cout << server.set_key(0, "k2", object) << std::endl;
    std::cout << server.set_key(0, "k3", object) << std::endl;
    std::cout << server.set_key(1, "k1", object) << std::endl;
    std::cout << server.set_key(1, "k2", object) << std::endl;
    std::cout << server.set_key(1, "k3", object) << std::endl;
    std::cout << server.get_dbsize(0) << std::endl;
    std::cout << server.get_dbsize(1) << std::endl;
    std::cout << server.flushall() << std::endl;
    std::cout << server.get_dbsize(0) << std::endl;
    std::cout << server.get_dbsize(1) << std::endl;
}
