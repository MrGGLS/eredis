#include "controller.h"
#include "erdb.hpp"
#include "eredis.hpp"
#include <iostream>
//#if _WIN32
//#include <winsock.h>
//#pragma comment(lib, "ws2_32.lib")
//#elif __APPLE__
//// macOS
//#elif __unix__
//// 处理 Linux
//#endif
void initialization();
void testList(ERedisServer &server);
void testExpire(ERedisServer &server);
void testServer(ERedisServer &server);
void testInt_Double(ERedisServer &server);
void testSave();
int main(int argc, char **argv)
{
    testSave();
    //定义长度变量
    int send_len = 0;
    int recv_len = 0;
    int len = 0;
    //定义发送缓冲区和接受缓冲区

    char output[100];
    char input[100];

    //解释器测试
    /* std::cin.getline(input, 100); */
    /* Controller ctrl; */
    /* std::cout<<ctrl.run(input); */

    //    while (true) {
    //        /* 处理输入 */
    //        // socket
    //        // auto input;
    //        // std::cin >> input;
    //
    //        // return
    //        /* Ret ret = contorller->run(input); */
    //
    //        /* 响应 */
    //        // cout << /* 一些输出 */;
    //    }
    //#if _WIN32
    //    //定义服务端套接字，接受请求套接字
    //    SOCKET s_server;
    //    SOCKET s_accept;
    //    //服务端地址客户端地址
    //    SOCKADDR_IN server_addr;
    //    SOCKADDR_IN accept_addr;
    //    initialization();
    //    //填充服务端信息
    //    server_addr.sin_family = AF_INET;
    //    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    //    server_addr.sin_port = htons(5010);
    //    //创建套接字
    //    s_server = socket(AF_INET, SOCK_STREAM, 0);
    //    if (bind(s_server, (SOCKADDR *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    //        std::cout << "error" << std::endl;
    //        WSACleanup();
    //    }
    //    //设置套接字为监听状态
    //    if (listen(s_server, SOMAXCONN) < 0) {
    //        std::cout << "error" << std::endl;
    //        WSACleanup();
    //    }
    //    std::cout << "wait..." << std::endl;
    //    //接受连接请求
    //    len = sizeof(SOCKADDR);
    //    s_accept = accept(s_server, (SOCKADDR *)&accept_addr, &len);
    //    if (s_accept == SOCKET_ERROR) {
    //        std::cout << "error" << std::endl;
    //        WSACleanup();
    //        return 0;
    //    }
    //
    //    //接收数据
    //    while (1) {
    //        recv_len = recv(s_accept, input, 100, 0);
    //        if (recv_len < 0) {
    //            break;
    //        }
    //
    //        //处理input数据并发送到客户端
    //        std::cin >> output;
    //
    //        send_len = send(s_accept, output, 100, 0);
    //        if (send_len < 0) {
    //            break;
    //        }
    //    }
    //    //关闭套接字
    //    closesocket(s_server);
    //    closesocket(s_accept);
    //    //释放DLL资源
    //    WSACleanup();
    //#endif
    return 0;
}

//#if _WIN32
// void initialization()
//{
//    //初始化套接字库
//    WORD w_req = MAKEWORD(2, 2); //版本号
//    WSADATA wsadata;
//    int err;
//    err = WSAStartup(w_req, &wsadata);
//    if (err != 0) {
//        std::cout << "初始化套接字库失败！" << std::endl;
//    }
//    //检测版本号
//    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
//        std::cout << "套接字库版本号不符！" << std::endl;
//        WSACleanup();
//    }
//    //填充服务端地址信息
//}
//#endif

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
    //    testList(server);
    //    testExpire(server);
    //    testServer(server);
    //    testInt_Double(server);
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
    object.set_str("I'm Mr.GGLS!");
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

/* CLI */

/* Parser --> controller --> dbops */
/* class controller { */
/* private: */
/*     Parser *par; */
/*     DBOPS *dbops; */
/* public: */
/*     run(); */
/* } */

/* class Parser { */

/* } */

/* controller::run(string input) { */
/*     auto retType = par.run(input); */
/*     switch (retType.type) { */
/*     case type1: */
/*         dbops.f1(); */

/*     } */
/* } */

/* class DBOPS { */
/*     f1(); */
/*     f2(); */
/*     f3(); */
/* } */
