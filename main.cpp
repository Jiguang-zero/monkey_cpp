//
// Created by 86158 on 2024/3/26.
//

#include "test.h"

using namespace monkey;

void test() {
    testToken();

    testParser();

    testAst();
};


int main(int argc, char** argv) {
//    // window 环境获取用户
//    char username[UNLEN+1];
//    DWORD username_len = UNLEN+1;
//    GetUserNameA(username, &username_len);
//
//    string a;
//    printf("Hello %s! Feel free to type in commands\n", username);
//
//    monkey::repl::Repl::Start(std::cin, std::cout);

    test();

    return 0;
}