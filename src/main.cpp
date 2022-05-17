#include "controller.h"
#include "eredis.hpp"
#include <iostream>

void test();
int main(int argc, char **argv)
{
    test();
    //解释器测试
    //    std::string input("set 12 13");
    //    Controller ctrl;
    //    ctrl.run(input);

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
    return 0;
}

void test(){
    auto server = getInstanceOfServer();

    std::thread check_keys_thread(clear_invalid_keys,&server);
    check_keys_thread.detach();

    std::string* value = new std::string ("hehe");
    ERObject object = ERObject(ObjectType::EREDIS_STRING,(void *) value);
    //    std::cout<<object.get_str()<<std::endl;
    /* test set key */
    std::cout<<"test set key"<<std::endl;
    std::cout<<server.set_key(0,"k1",object)<<std::endl;
    /* test get key */
    std::cout<<"test get key"<<std::endl;
    std::cout<<server.get_key(0,"k1")<<std::endl;
    /* get not exist key */
    std::cout<<"get not exist key"<<std::endl;
    std::cout<<server.get_key(0,"k2")<<std::endl;
    /* delete same key two times */
    std::cout<<"delete same key two times"<<std::endl;
    std::cout<<server.del_key(0,"k1")<<std::endl;
    std::cout<<server.del_key(0,"k1")<<std::endl;
    /* test get range */
    std::cout<<"test get range"<<std::endl;
    std::cout<<server.set_key(0,"k1",object)<<std::endl;
    std::cout<<server.getrange(0,"k1",1,3)<<std::endl;
    /* test append */
    std::cout<<"test append"<<std::endl;
    std::cout<<server.append_value(0,"k1","hihi")<<std::endl;
    std::cout<<server.get_strlen(0,"k1")<<std::endl;
    /* test get key type */
    std::cout<<"delete get key type"<<std::endl;
    std::cout<<server.get_key_type(0,"k1")<<std::endl;

    std::cout<<server.set_key(0,"k1",object)<<std::endl;
    std::cout<<server.set_key(0,"k2",object)<<std::endl;
    std::cout<<server.set_key(0,"k3",object)<<std::endl;
    /* test get all keys */
    std::cout<<"test get all keys"<<std::endl;

    std::cout<<server.get_all_keys(0)<<std::endl;
    /* test get db size */
    std::cout<<"test get db size"<<std::endl;
    std::cout<<server.get_dbsize(0)<<std::endl;
    /* test exists key */
    std::cout<<"test exists key"<<std::endl;
    std::cout<<server.exists_key(0,"k4")<< std::endl;
    std::cout<<server.exists_key(0,"k2")<< std::endl;
    /* test flush db*/
    std::cout<<"test flush db"<<std::endl;
    std::cout<<server.flushdb(0)<<std::endl;
    std::cout<<server.get_dbsize(0)<<std::endl;
    /* test flush all */
    std::cout<<"test flush all"<<std::endl;
    std::cout<<server.set_key(0,"k1",object)<<std::endl;
    std::cout<<server.set_key(0,"k2",object)<<std::endl;
    std::cout<<server.set_key(0,"k3",object)<<std::endl;
    std::cout<<server.set_key(1,"k1",object)<<std::endl;
    std::cout<<server.set_key(1,"k2",object)<<std::endl;
    std::cout<<server.set_key(1,"k3",object)<<std::endl;
    std::cout<<server.get_dbsize(0)<<std::endl;
    std::cout<<server.get_dbsize(1)<<std::endl;
    std::cout<<server.flushall()<<std::endl;
    std::cout<<server.get_dbsize(0)<<std::endl;
    std::cout<<server.get_dbsize(1)<<std::endl;
}

/* CLI */

/* parser --> controller --> dbops */
/* class controller { */
/* private: */
/*     parser *par; */
/*     DBOPS *dbops; */
/* public: */
/*     run(); */
/* } */

/* class parser { */

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
