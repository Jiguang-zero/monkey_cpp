//
// Created by 86158 on 2024/3/26.
//

// 这是一个测试应用, 不属于cmake项目

#include "lexer/lexer_test.h"
#include "parser/parser_test.h"
#include "ast/ast_test.h"

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

    testReturnStatements();

    testIdentifierExpression();

    cout << "Test parser END" << endl;

    cout << endl;
}

void testAst() {
    cout << "Test ast START: " << endl;

    testString();

    cout << "Test ast END" << endl;

    cout << endl;
}