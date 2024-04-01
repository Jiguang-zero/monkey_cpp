//
// Created by 86158 on 2024/3/26.
//

// ����һ������Ӧ��, ������cmake��Ŀ

#include "lexer/lexer_test.h"
#include "parser/parser_test.h"
#include "ast/ast_test.h"

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

    testReturnStatements();

    testIdentifierExpression();

    testIntegerLiteralExpression();

    testParsingPrefixExpressions();

    testParsingInfixExpressions();

    cout << "Test parser END" << endl;

    cout << endl;
}

void testAst() {
    cout << "Test ast START: " << endl;

    testString();

    cout << "Test ast END" << endl;

    cout << endl;
}