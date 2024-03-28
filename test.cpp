//
// Created by 86158 on 2024/3/26.
//

// 这是一个测试应用, 不属于cmake项目

#include "lexer/lexer_test.cpp"
#include "parser/parser_test.h"

// token 测试
void testToken() {
    cout << "Test token START: " << endl;

    testNextToken();

    cout << "Test token END" << endl;

    cout << endl;
}

void testParser() {
    cout << "Test parser START: " << endl;

    testLetStatements();

    cout << "Test parser END" << endl;

    cout << endl;
}

int main() {
    // token 测试
    testParser();

    return 0;
}