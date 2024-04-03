//
// Created by 86158 on 2024/3/29.
//

#include <iostream>
#include "Program.h"

using std::cout;

using namespace monkey;

using std::endl;


// �� ast���й��� , Ȼ����֤ String() �Ƿ����Ҫ��
void testString() {
    cout << "Test testString() Start: " << endl;

    bool flag(true);


    auto* identifier = new ast::Identifier(
            token::Token(token::IDENT, "anotherVar"),
            "anotherVar");

    ast::LetStatement letStatement(
            token::Token(token::IDENT, "let"),
            new ast::Identifier(
                    token::Token(token::IDENT, "myVar"),
                    "myVar"
                    ),
            identifier
            );
    auto* program = new ast::Program(vector<ast::Statement*>{&letStatement});

    if (program->String() !=  "let myVar = anotherVar;") {
        cout << "program.String() wrong. got " << program->String() << endl;
        flag = false;
    }

    cout << "Test testString() End: " << (flag ? "PASS" : "FAIL") << endl;

    delete identifier;
    delete program;
}