//
// Created by 86158 on 2024/3/26.
//

// ����һ������Ӧ��, ������cmake��Ŀ

#include "lexer/lexer_test.cpp"
#include "parser/parser_test.h"

// token ����
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
    // token ����
    testParser();

    return 0;
}