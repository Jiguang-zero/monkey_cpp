//
// Created by 86158 on 2024/3/27.
//

#include "Parser.h"
#include <iostream>
#include <vector>


using std::vector;
using std::cout;
using std::endl;
using std::cerr;

using namespace monkey;

void testLetStatements() {
    string input =
            "let x = 5;\n"
            "let foobar = 832343;\n";
    cout << "Test testLetStatements() Start: " << endl;

    auto l = lexer::Lexer::New(input);
    auto p = parser::Parser::New(l);

    ast::Program *program = nullptr;
    p->ParseProgram(&program);



    bool flag = true;

    if (program == nullptr) {
        flag = false;

        cerr << "ParseProgram() returned nullptr" << endl;
        goto END;
    }

    if (program->getStatements().size() != 2) {
        cerr << "program.Statements does not contain 2 statements. got=" << program->getStatements().size() << endl;
        flag = false;
        goto END;
    }



    END:
    {
        cout << "Test testNextToken() end: " << (flag ? "PASS" : "FAIL") << endl;
    };
}