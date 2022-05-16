#include "controller.h"
#include "dbdata.h"
#include <iostream>

static EasyDB easydb;

int main(int argc, char **argv)
{
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
