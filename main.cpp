//
// Created by 86158 on 2024/3/26.
//

#include "test.h"

#include <windows.h>
#include <Lmcons.h>
#include <conio.h>

#include "repl/Repl.h"

using namespace monkey;

void test() {
    testToken();

    testParser();

    testAst();

    testEvaluator();
}


int main(int argc, char** argv) {
    cout << "Please Choose 1 to TEST and 2 to REPL(NOT ANY OTHER CHARACTER INCLUDING SPACE): \n";

    char ch = getch();
    if (ch - '0' == 1) {
        test();
    }
    else if (ch - '0' == 2) {
        // window 环境获取用户
        char username[UNLEN+1];
        DWORD username_len = UNLEN+1;
        GetUserNameA(username, &username_len);

        string a;
        printf("Hello %s! Feel free to type in commands\n", username);

        monkey::repl::Repl::Start(std::cin, std::cout);
    }
    else {
        cout << "No Other Choice." << endl;
    }




    system("pause");
    return 0;
}