//
// Created by 86158 on 2024/3/27.
//

#include "Parser.h"
#include <iostream>
#include <vector>
#include "typeinfo"


using std::vector;
using std::cout;
using std::endl;
using std::cerr;

using namespace monkey;

// 如果不加引用，那么传入的是 Statement的拷贝，会造成数据丢失
// important
bool testLetStatement(ast::Statement& s, const string& name) {
    cout << "19: " << s.TokenLiteral() << endl;
    if (s.TokenLiteral() != "let") {
        cout << "TokenLiteral not 'let', but " << s.TokenLiteral() << endl;
        return false;
    }

    // 转化继承基类 类型的指针
    auto* letStmt = dynamic_cast<ast::LetStatement*>(&s);
    if (!letStmt) {
        cout << "s not *ast::LetStatement, but " << typeid(s).name() << endl;
        return false;
    }

    if (letStmt->getName()->TokenLiteral() != name) {
        cout << "letStmt.Name.TokenLiteral() not " << name << ", ";
        cout << "but " << letStmt->getName()->TokenLiteral() << endl;
        return false;
    }

    return true;
}

void testLetStatements() {
    string input =
            "let x = 5;\n"
            "let foo_bar = 832343;\n";
    cout << "Test testLetStatements() Start: " << endl;

    auto l = lexer::Lexer::New(input);
    auto p = parser::Parser::New(l);

    auto *program = new ast::Program();
    p->ParseProgram(&program);


    bool flag = true;

    if (program == nullptr) {
        flag = false;

        cerr << "ParseProgram() returned nullptr" << endl;
    }

    if (program->getStatements().size() != 2) {
        cerr << "program.Statements does not contain 2 statements. got=" << program->getStatements().size() << endl;
        flag = false;
    }

    vector<string> identifiers = {
            "x",
            "foo_bar"
    };

    int i = 0;
    for (const auto& identifier : identifiers) {
        auto* stmt = program->getStatements()[i];
        if (!testLetStatement(*stmt, identifiers[i])) {
            flag = false;
        }

        i++;
    }



    cout << "Test testNextToken() end: " << (flag ? "PASS" : "FAIL") << endl;

}

