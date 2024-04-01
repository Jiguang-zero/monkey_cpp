//
// Created by 86158 on 2024/3/27.
//

#include "Parser.h"
#include <iostream>
#include <utility>
#include <vector>
#include "typeinfo"
#include <tuple>


using std::vector;
using std::tuple;
using std::cout;
using std::endl;
using std::cerr;
using std::get;

using namespace monkey;

/******* 辅助函数 *************/

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
        // print "no errors "
        return;
    }

    cout << "parser: " << errors.size() << " errors." << endl;

    for (auto & msg : errors) {
        cout << "parser error: " << msg << endl;
    }

    // 如果exit 程序不好调试
    // exit(-1);
}

// 判断 integer 表达式的数字是否正确
bool testIntegerLiteral(ast::Expression* expression, long long value) {
    auto * integer = dynamic_cast<ast::IntegerLiteral *>(expression);

    if (!integer) {
        cout << "Get IntegerLiteral Failed." << endl;
        return false;
    }

    if (integer->getValue() != value) {
        cout << "integer.Value not " << value << " but " << integer->getValue() << endl;
        return false;
    }

    if (integer->TokenLiteral() != std::to_string(value)) {
        cout << "integer.TokenLiteral not " << std::to_string(value);
        cout << " but " << integer->TokenLiteral() << endl;
        return false;
    }

    return true;
}

/************ 测试函数 ****************/
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



    cout << "Test testLetStatements() END: " << (flag ? "PASS" : "FAIL") << endl;

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

    cout << "Test testReturnStatements() END: " << (flag ? "PASS" : "FAIL") << endl;
}

// 测试标识符
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

    auto * stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
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

    cout << "Test testIdentifierExpression() END: " << (flag ? "PASS" : "FAIL") << endl;
}

// 测试整数类型
void testIntegerLiteralExpression() {
    cout << "Test testIntegerLiteralExpression() START: " << endl;

    string input = "5;";

    auto l = lexer::Lexer::New(input);

    auto p = parser::Parser::New(l);

    auto* program = p->ParseProgram();

    bool flag(true);

    checkoutParserErrors(p);

    if (program->getStatements().size() != 1) {
        cout << "program has not enough statements. got " << program->getStatements().size() << endl;
        flag = false;
    }

    auto * stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
    if (!stmt) {
        cerr << "get ast::ExpressionStatement Failed." << endl;
        return;
    }

    auto * literal = dynamic_cast<ast::IntegerLiteral *>(stmt->getExpression());
    if (!literal) {
        cerr << "get ast::literal Failed." << endl;
        return;
    }

    if (literal->getValue() != 5) {
        cout << "literal.value not 5, but " << literal->getValue() << endl;
        flag = false;
    }

    if (literal->TokenLiteral() != "5") {
        cout << "literal.TokenLiteral() not \"5\", but " << literal->TokenLiteral() << endl;
        flag = false;
    }

    cout << "Test testIntegerLiteralExpression() END: " << (flag ? "PASS" : "FAIL") << endl;
}

// 测试前缀表达式
void testParsingPrefixExpressions() {
    cout << "Test testParsingPrefixExpressions() START:" << endl;

    typedef tuple<string, string, long long> prefixTests;

    vector<prefixTests> tests = {
            {"!5;", "!", 5},
            {"-632; ", "-", 632}
    };

    bool flag(true);

    for (auto test : tests) {
        auto * l = lexer::Lexer::New(get<0>(test));
        auto * p = parser::Parser::New(l);

        auto * program = p->ParseProgram();
        checkoutParserErrors(p);

        if (program->getStatements().size() != 1) {
            cout << "program does not contain 1 statement. got " << program->getStatements().size() << endl;
            flag = false;
        }

        auto * stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
        if (!stmt) {
            cerr << "get ast::ExpressionStatement Failed." << endl;
            return;
        }

        auto * exp = dynamic_cast<ast::PrefixExpression *>(stmt->getExpression());
        if (!exp) {
            cerr << "get ast::PrefixExpression Failed." << endl;
        }

        if (exp->getOperator() != get<1>(test)) {
            flag = false;
            cout << "exp.Operator not " << get<1>(test) << " but " << exp->getOperator() << endl;
        }

        if (!testIntegerLiteral(exp->getRightExpression(), get<2>(test))) {
            flag = false;
        }

        // 释放空间
        delete exp;
        delete stmt;
        delete program;
        delete p;
        delete l;
    }

    cout << "Test testParsingPrefixExpressions() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testParsingInfixExpressions() {
    cout << "Test testParsingInfixExpressions() START:" << endl;

    struct infixTest {
        string input;
        long long leftValue;
        string op; // operator
        long long rightValue;

        infixTest(
                string i,
                long long lV,
                string o,
                long long rV
                ) {
            input = std::move(i);
            leftValue = lV;
            op = std::move(o);
            rightValue = rV;
        }
    };

    vector<infixTest> tests = {
            infixTest("5 + 5", 5, "+", 5),
            infixTest("5 - 5", 5, "-", 5),
            infixTest("5 * 5", 5, "*", 5),
            infixTest("5 / 5", 5, "/", 5),
            infixTest("5 > 5", 5, ">", 5),
            infixTest("5 < 5", 5, "<", 5),
            infixTest("5 == 5", 5, "==", 5),
            infixTest("5 != 5", 5, "!=", 5)
    };

    bool flag(true);


    for (const auto& test : tests) {

        auto * l = lexer::Lexer::New(test.input);
        auto * p = parser::Parser::New(l);

        auto * program = p->ParseProgram();
        checkoutParserErrors(p);

        if (program->getStatements().size() != 1) {
            cout << "program does not contain 1 statement. got " << program->getStatements().size() << endl;
            flag = false;
        }

        auto * stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
        if (!stmt) {
            cerr << "get ast::ExpressionStatement Failed." << endl;
            return;
        }

        auto * exp = dynamic_cast<ast::InfixExpression *>(stmt->getExpression());
        if (!exp) {
            cerr << "get ast::InfixExpression Failed." << endl;
            return;
        }

        if (!testIntegerLiteral(exp->getLeft(), test.leftValue)) {
            flag = false;
        }
        if (exp->getOperator() != test.op) {
            cout << "exp.Operator() not " << test.op;
            cout << " but " << exp->getOperator() << endl;
            flag = false;
        }
        if (!testIntegerLiteral(exp->getRight(), test.rightValue)) {
            flag = false;
        }

        // 释放空间
        delete exp;
        exp = nullptr;
        delete stmt;
        stmt = nullptr;
        delete program;
        program = nullptr;
        delete p;
        p = nullptr;
        delete l;
        l = nullptr;
    }

    cout << "Test testParsingInfixExpressions() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testOperatorPrecedenceParsing() {
    cout << "Test testOperatorPrecedenceParsing() START: " << endl;

    bool flag(true);

    struct TestType {
        string input;
        string expected;

        TestType(string input, string expected) {
            this->input = std::move(input);
            this->expected = std::move(expected);
        }
    };

    vector<TestType> tests = {
            TestType("-a * b", "((-a) * b)"),
            TestType("!-a", "(!(-a))"),
            TestType("a + b + c", "((a + b) + c)"),
            TestType("a + b - c", "((a + b) - c)"),
            TestType("a + b / c", "(a + (b / c))"),
            TestType("a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"),
            TestType("3 + 4; -5 * 5", "(3 + 4)((-5) * 5)"),
            TestType("3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))")
    };

    for (const auto& test : tests) {
        auto * l = lexer::Lexer::New(test.input);
        auto * p = parser::Parser::New(l);

        auto * program = p->ParseProgram();
        checkoutParserErrors(p);

        string actual = program->String();
        if (actual != test.expected) {
            flag = false;
            cout << "expected: " << test.expected << ". got: " << actual << endl;
        }

        delete program;
        program = nullptr;

        delete p;
        p = nullptr;

        delete l;
        l = nullptr;
    }

    cout << "Test testOperatorPrecedenceParsing() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testBooleanExpression() {
    cout << "Test testBooleanExpression() START: " << endl;

    bool flag(true);

    struct TestType {
        string input;
        bool value;

        TestType(string i, bool v) {
            input = std::move(i);
            value = v;
        }
    };

    vector<TestType> tests = {
            TestType("true", true),
            TestType("false", false)
    };

    for (const auto & test : tests) {
        auto * l = lexer::Lexer::New(test.input);
        auto * p = parser::Parser::New(l);

        auto * program = p->ParseProgram();
        checkoutParserErrors(p);

        if (program->getStatements().size() != 1) {
            cout << "program does not contain 1 statement. got " << program->getStatements().size() << endl;
            flag = false;
        }

        auto * stmt = dynamic_cast<ast::ExpressionStatement *>(program->getStatements()[0]);
        if (!stmt) {
            cerr << "get ast::ExpressionStatement Failed." << endl;
            return;
        }

        auto * expression = dynamic_cast<ast::Boolean *>(stmt->getExpression());
        if (!expression) {
            cerr << "get ast::Boolean Failed." << endl;
            return;
        }

        auto fn = [&] () -> string {
            return test.value ? "true" : "false";
        };

        if (expression->TokenLiteral() != fn()) {
            cout << "expression.TokenLiteral not " << fn() << " but " << expression->TokenLiteral() << endl;
            flag = false;
        }

        if (expression->getValue() != test.value) {
            cout << "expression.Value not " << test.value << " but " << expression->getValue() << endl;
            flag = false;
        }
    }

    cout << "Test testBooleanExpression() END: " << (flag ? "PASS" : "FAIL") << endl;
}
