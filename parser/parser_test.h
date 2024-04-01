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

void checkoutParserErrors(parser::Parser* & p) {
    auto errors = p->getErrors();
    if (errors.empty()) {
        // cout << "no errors " << endl;
        return;
    }

    cout << "parser: " << errors.size() << " errors." << endl;

    for (auto & msg : errors) {
        cout << "parser error: " << msg << endl;
    }

    // 如果exit 程序不好调试
    // exit(-1);
}

// 测试 let 语句
void testLetStatements() {
    string input =
            "let x = 5 ;\n"
            "let foo_bar = 832343;\n";

    cout << "Test testLetStatements() Start: " << endl;

    auto l = lexer::Lexer::New(input);
    auto p = parser::Parser::New(l);

    auto *program = p->ParseProgram();

    checkoutParserErrors(p);

    bool flag = true;

    if (program == nullptr) {
        cerr << "ParseProgram() returned nullptr" << endl;
        return;
    }

    if (program->getStatements().size() != 2) {
        cout << "program.Statements does not contain 2 statements. got=" << program->getStatements().size() << endl;
        flag = false;
    }

    vector<string> identifiers = {
            "x",
            "foo_bar"
    };

    int i = 0;
    for (const auto& identifier : identifiers) {
        auto* stmt = program->getStatements()[i];
        if (!testLetStatement(*stmt, identifier)) {
            flag = false;
        }

        i++;
    }



    cout << "Test testLetStatements() end: " << (flag ? "PASS" : "FAIL") << endl;

}

// 测试 return 语句
void testReturnStatements() {
    string input = "return 5; \n"
                   "return = a; \n"
                   "return 324 232;";

    cout << "Test testReturnStatements() Start: " << endl;

    auto l = lexer::Lexer::New(input);
    auto p = parser::Parser::New(l);

    auto *program = p->ParseProgram();
    checkoutParserErrors(p);

    bool flag = true;

    if (program == nullptr) {
        cerr << "ParseProgram() returned nullptr" << endl;
        return;
    }

    if (program->getStatements().size() != 3) {
        cout << "program.Statements does not contain 3 statements. got=" << program->getStatements().size() << endl;
        flag = false;
    }

    for (const auto& stmt : program->getStatements()) {
        auto* returnStmt = dynamic_cast<ast::ReturnStatement*>(stmt);
        if (!returnStmt) {
            cout << "s not *ast::ReturnStatement, but " << typeid(*stmt).name() << endl;
            flag = false;
        }
        if (returnStmt->TokenLiteral() != "return") {
            cout << "returnStmt.TokenLiteral not 'return' but " << returnStmt->TokenLiteral() << endl;
        }
    }

    cout << "Test testReturnStatements() end: " << (flag ? "PASS" : "FAIL") << endl;
}

void testIdentifierExpression() {
    string input = "foo_bar";

    cout << "Test testIdentifierExpression() Start: " << endl;

    auto l = lexer::Lexer::New(input);

    auto p = parser::Parser::New(l);

    auto *program = p->ParseProgram();

    checkoutParserErrors(p);

    bool flag(true);

    if (program->getStatements().size() != 1) {
        cout << "program has not enough statements. got " << program->getStatements().size() << endl;
        flag = false;
    }

    auto* stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
    if (!stmt) {
        cerr << "get ExpressionStatement Failed." << endl;
        return;
    }

    auto ident = dynamic_cast<ast::Identifier *>(stmt->getExpression());
    if (!ident) {
        cerr << "get Identifier Failed." << endl;
        return;
    }

    if (ident->getValue() != "foo_bar") {
        cout << "ident.Value not foo_bar, but " << ident->getValue() << endl;
        flag = false;
    }

    if (ident->TokenLiteral() != "foo_bar") {
        cout << "ident.TokenLiteral() not foo_bar, but " << ident->TokenLiteral() << endl;
        flag = false;
    }

    cout << "Test testIdentifierExpression() end: " << (flag ? "PASS" : "FAIL") << endl;
}
