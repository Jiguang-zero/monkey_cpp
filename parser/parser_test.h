//
// Created by 86158 on 2024/3/27.
//

#include "Parser.h"
#include <iostream>
#include <utility>
#include <vector>
#include "typeinfo"
#include <tuple>
#include <variant>


using std::vector;
using std::tuple;
using std::cout;
using std::endl;
using std::cerr;
using std::get;
using std::variant;
using std::holds_alternative;

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

// 测试标识符 辅助函数
bool testIdentifier(ast::Expression * expression, const string& value) {
    auto * ident = dynamic_cast<ast::Identifier *>(expression);

    if (!ident) {
        cout << "testIdentifier() wrong: get ast::Identifier* Failed. " << endl;
        return false;
    }

    if (ident->getValue() != value) {
        cout << "ident.Value not " << value << " but " << ident->getValue() << endl;
        return false;
    }

    if (ident->TokenLiteral() != value) {
        cout << "ident.TokenLiteral not " << value << " but " << ident->TokenLiteral() << endl;
        return false;
    }

    return true;
}

// 判断 布尔值 的表达式字面量是否正确
bool testBooleanLiteral(ast::Expression* expression, bool value) {
    auto * bo = dynamic_cast<ast::Boolean *>(expression);

    if (!bo) {
        cout << "get ast::Boolean Failed. " << endl;
        return false;
    }

    if (bo->getValue() != value) {
        cout << "bo.Value not " << value << " but " << bo->getValue() << endl;
        return false;
    }

    // 获取 布尔值的 string
    auto fn = [&] () -> string {
        return value ? "true" : "false";
    };

    if (bo->TokenLiteral() != fn()) {
        cout << "bo.TokenLiteral() not " << fn() << " but " << bo->TokenLiteral() << endl;
        return false;
    }


    return true;
}

bool testLiteralExpression(
        ast::Expression * exp,
        std::variant<int, long long, string, bool> expected
        ) {
    if (holds_alternative<int>(expected)) {
        return testIntegerLiteral(exp, static_cast<long long>(get<int>(expected)));
    }
    else if (holds_alternative<long long>(expected)) {
        return testIntegerLiteral(exp, get<long long>(expected));
    }
    else if (holds_alternative<string>(expected)) {
        return testIdentifier(exp, get<string>(expected));
    }
    else if (holds_alternative<bool>(expected)) {
        return testBooleanLiteral(exp, get<bool>(expected));
    }

    cerr << "type of exp not handled. " << exp->String() << endl;
    return false;
}

bool testInfixExpression(
        ast::Expression * exp,
        variant<int, long long, string, bool> left,
        const string& op, // operator
        variant<int, long long, string, bool> right
        ) {
    auto * opExp = dynamic_cast<ast::InfixExpression *>(exp);
    if (!opExp) {
        cout << "get infixExpression Failed. " << endl;
        return false;
    }

    if (!testLiteralExpression(opExp->getLeft(), std::move(left))) {
        return false;
    }

    if (opExp->getOperator() != op) {
        cout << "exp.Operator not " << op << " but " << opExp->getOperator() << endl;
        return false;
    }

    if (!testLiteralExpression(opExp->getRight(), std::move(right))) {
        return false;
    }

    return true;
}



/************ 测试函数 ****************/
// 测试 let 语句
void testLetStatements() {
    struct TestType {
        string input;
        string expectedIdentifier;
        std::variant<int, long long, string, bool> expectedValue;
    };

    vector<TestType> tests = {
            {"let x = 5", "x", 5},
            {"let y = true", "y", true},
            {"let x = y", "x", "y"}
    };

    cout << "Test testLetStatements() Start: " << endl;
    bool flag = true;

    for (const auto & test : tests) {
        auto l = lexer::Lexer::New(test.input);
        auto p = parser::Parser::New(l);

        auto *program = p->ParseProgram();

        checkoutParserErrors(p);


        if (program == nullptr) {
            cerr << "ParseProgram() returned nullptr" << endl;
            return;
        }

        if (program->getStatements().size() != 1) {
            cout << "program.Statements does not contain 1 statements. got=" << program->getStatements().size() << endl;
            flag = false;
        }

        auto* stmt = program->getStatements()[0];
        if (!testLetStatement(*stmt, test.expectedIdentifier)) {
            flag = false;
        }

        auto * val = dynamic_cast<ast::LetStatement*>(stmt)->getValue();

        if (!testLiteralExpression(val, test.expectedValue)) {
            flag = false;
        }
    }

    cout << "Test testLetStatements() END: " << (flag ? "PASS" : "FAIL") << endl;

}

// 测试 return 语句
void testReturnStatements() {
    struct TestType {
        string input;
        std::variant<int, long long, string, bool> expectedValue;
    };

    cout << "Test testReturnStatements() Start: " << endl;
    vector<TestType> tests = {
            {"return 5;", 5},
            {"return true", true},
            {"return a;", "a"}
    };

    bool flag = true;

    for (const auto & test : tests) {
        auto l = lexer::Lexer::New(test.input);
        auto p = parser::Parser::New(l);

        auto *program = p->ParseProgram();
        checkoutParserErrors(p);

        if (program == nullptr) {
            cerr << "ParseProgram() returned nullptr" << endl;
            return;
        }

        if (program->getStatements().size() != 1) {
            cout << "program.Statements does not contain 1 statements. got=" << program->getStatements().size() << endl;
            flag = false;
        }

        auto* returnStmt = dynamic_cast<ast::ReturnStatement*>(program->getStatements()[0]);
        if (!returnStmt) {
            cerr << "s not *ast::ReturnStatement, but " << typeid(program->getStatements()[0]).name() << endl;
            return;
        }


        if (returnStmt->TokenLiteral() != "return") {
            cout << "returnStmt.TokenLiteral not 'return' but " << returnStmt->TokenLiteral() << endl;
            flag = false;
        }

        if (!testLiteralExpression(returnStmt->getReturnValue(), test.expectedValue)) {
            flag = false;
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

    struct PrefixTest {
        string input;
        string op;
        variant<int, long long, string, bool> value;

        PrefixTest(string i,
                   string o,
                   variant<int, long long, string, bool> v)
                   : input(std::move(i)), op(std::move(o)), value(std::move(v)) {};
    };

    vector<PrefixTest> tests = {
            {"!5;", "!", 5},
            {"-632; ", "-", 632},
            {"!true", "!", true}
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

        auto * exp = dynamic_cast<ast::PrefixExpression *>(stmt->getExpression());
        if (!exp) {
            cerr << "get ast::PrefixExpression Failed." << endl;
        }

        if (exp->getOperator() != test.op) {
            flag = false;
            cout << "exp.Operator not " << test.op << " but " << exp->getOperator() << endl;
        }

        if (!testLiteralExpression(exp->getRightExpression(), test.value)) {
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
        variant<int, long long, string, bool> leftValue;
        string op; // operator
        variant<int, long long, string, bool> rightValue;

        infixTest(
                string i,
                variant<int, long long, string, bool> lV,
                string o,
                variant<int, long long, string, bool> rV
                ) {
            input = std::move(i);
            leftValue = std::move(lV);
            op = std::move(o);
            rightValue = std::move(rV);
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
            infixTest("5 != 5", 5, "!=", 5),
            infixTest("true != false", true, "!=", false)
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

        if (!testInfixExpression(stmt->getExpression(),
                                 test.leftValue,
                                 test.op,
                                 test.rightValue)) {
            flag = false;
        }

        // 释放空间
        delete stmt;
        delete program;
        delete p;
        p = nullptr;
        delete l;
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
            TestType("3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"),
            TestType("3 > 5 == false", "((3 > 5) == false)"),
            TestType("3 < 5 == true", "((3 < 5) == true)"),
            TestType("!(true == true)", "(!(true == true))"),
            TestType("1 + 3 * (3 / 9) - (3 + 3)", "((1 + (3 * (3 / 9))) - (3 + 3))"),
            TestType("add(a, b, 1, 2 * 3, 4 + 5, add(6, 7 * 8))", "add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))"),
            {
                    "a * [1, 2, 3, 4][b * c] * d",
                    "((a * ([1, 2, 3, 4][(b * c)])) * d)",
            },
            {
                    "add(a * b[2], b[1], 2 * [1, 2][1])",
                    "add((a * (b[2])), (b[1]), (2 * ([1, 2][1])))",
            },
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

        delete p;
        p = nullptr;

        delete l;
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

        if (!testBooleanLiteral(stmt->getExpression(), test.value)) {
            flag = false;
        }
    }

    cout << "Test testBooleanExpression() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testIfElseExpression() {
    cout << "Test testIfElseExpression() START:" << endl;

    bool flag(true);

    string input = "if (x < y) {x;} else {y;}";

    auto * l = lexer::Lexer::New(input);
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

    auto * expression = dynamic_cast<ast::IfExpression *>(stmt->getExpression());
    if (!expression) {
        cerr << "get ast::IfExpression Failed." << endl;
        return;
    }

    if (!testInfixExpression(expression->getCondition(), "x", "<", "y")) {
        flag = false;
    }

    if (auto len = expression->getConsequence()->getStatements().size() != 1) {
        cout << "consequence does not include 1 statement. got " << len << endl;
        flag = false;
    }

    auto * consequence = dynamic_cast<ast::ExpressionStatement*>(
            expression->getConsequence()->getStatements()[0]
            );
    if (!testIdentifier(consequence->getExpression(), "x")) {
        flag = false;
    }

    if (expression->getAlternative()) {
        if (auto len = expression->getAlternative()->getStatements().size() != 1) {
            cout << "alternative does not include 1 statement. got " << len << endl;
            flag = false;
        }

        auto * alternative = dynamic_cast<ast::ExpressionStatement*>(
                expression->getAlternative()->getStatements()[0]
                );
        if (!testIdentifier(alternative->getExpression(), "y")) {
            flag = false;
        }

    }

    cout << "Test testIfElseExpression() END: " << (flag ? "PASS": "FAIL") << endl;
}

void testFunctionLiteralParsing() {
    cout << "Test testFunctionLiteralParsing() START:" << endl;

    string input = "fn(x, y) { x + y; }";

    bool flag(true);

    auto * l = lexer::Lexer::New(input);
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

    auto * function = dynamic_cast<ast::FunctionLiteral*>(stmt->getExpression());
    if (!function) {
        cerr << "get ast::FunctionLiteral Failed." << endl;
        return;
    }

    if (function->getParameters().size() != 2) {
        cout << "function.Parameters not 2 but " << function->getParameters().size() << endl;
        flag = false;
    }

    if (!testIdentifier(function->getParameters()[0], "x")) flag = false;
    if (!testIdentifier(function->getParameters()[1], "y")) flag = false;

    if (auto len = function->getBody()->getStatements().size() != 1) {
        cout << "function.body.statements does not include 1 statement. got " << len << endl;
        flag = false;
    }

    auto bodyStmt = dynamic_cast<ast::ExpressionStatement*>(function->getBody()->getStatements()[0]);
    if (!bodyStmt) {
        cerr << "get ast::ExpressionStatements Failed." << endl;
        return;
    }

    if (!testInfixExpression(bodyStmt->getExpression(), "x", "+", "y")) flag = false;

    cout << "Test testFunctionLiteralParsing() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testFunctionParameters() {
    cout << "Test testFunctionParameters() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        vector<string> expectedParams;
    };

    vector<TestType> tests = {
            {"fn() {};", {}},
            {"fn(x) {};", {"x"}},
            {"fn(x, y, z) {};", {"x", "y", "z"}}
    };

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

        auto * function = dynamic_cast<ast::FunctionLiteral*>(stmt->getExpression());
        if (!function) {
            cerr << "get ast::FunctionLiteral Failed." << endl;
            return;
        }

        if (auto actual = function->getParameters().size() != test.expectedParams.size()) {
            cout << "the number of parameters is wrong. want " << test.expectedParams.size();
            cout << " but get " << actual << endl;
            return;
        }

        int i(0);
        for (const auto& ident : test.expectedParams) {
            if (!testLiteralExpression(function->getParameters()[i], ident)) {
                flag = false;
            }
            i ++;
        }

        delete function;
        delete stmt;
        delete program;
        delete p;
        delete l;

    }

    cout << "Test testFunctionParameters() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testCallExpressionParsing() {
    cout << "Test testCallExpressionParsing() START:" << endl;

    bool flag(true);

    string input = "add(1, 2 * 3, 4 + 5);";

    auto * l = lexer::Lexer::New(input);
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

    auto * expression = dynamic_cast<ast::CallExpression*>(stmt->getExpression());
    if (!expression) {
        cerr << "get ast::CallExpression Failed." << endl;
        return;
    }

    if (!testIdentifier(expression->getFunction(), "add")) {
        flag = false;
    }

    if (auto len = expression->getArguments().size() != 3) {
        cout << "expression.Arguments' size not 3 but " << len << endl;
        flag = false;
    }

    if (!testLiteralExpression(expression->getArguments()[0], 1) ||
        !testInfixExpression(expression->getArguments()[1], 2, "*", 3) ||
        !testInfixExpression(expression->getArguments()[2], 4, "+", 5)) {
        flag = false;
    }


    cout << "Test testCallExpressionParsing() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testStringLiteralExpression() {
    cout << "Test testStringLiteralExpression() START:" << endl;

    bool flag(true);

    string input = "\"foobar\"";

    auto * l = lexer::Lexer::New(input);
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

    auto * literal = dynamic_cast<ast::StringLiteral*>(stmt->getExpression());
    if (!literal) {
        cerr << "get ast::StringLiteral Failed." << endl;
        return;
    }

    if (literal->getValue() != "foobar") {
        cout << "value not foobar " << "but " << literal->getValue() << endl;
        flag = false;
    }

    if (literal->TokenLiteral() != "foobar") {
        cout << "TokenLiteral() not foobar but " << literal->TokenLiteral() << endl;
        flag = false;
    }

    cout << "Test testStringLiteralExpression() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void TestParsingArrayLiterals() {
    cout << "Test testParsingArrayLiterals() START:" << endl;

    bool flag(true);

    string input = "[1, 2 * 2, 3 + 3]";

    auto * l = lexer::Lexer::New(input);
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

    auto * array = dynamic_cast<ast::ArrayLiteral*>(stmt->getExpression());
    if (!array) {
        cerr << "get ast::ArrayLiteral* Failed." << endl;
        return;
    }

    if (array->getElements().size() != 3) {
        cout << "array.elements.size() not 3 but " << array->getElements().size() << endl;
        flag = false;
    }

    if (!testIntegerLiteral(array->getElements()[0], 1) ||
        !testInfixExpression(array->getElements()[1], 2, "*", 2) ||
        !testInfixExpression(array->getElements()[2], 3, "+", 3)) {
        flag = false;
    }

    cout << "Test testParsingArrayLiterals() END: " << (flag ? "PASS" : "FAIL") << endl;
}

void testParsingIndexExpressions() {
    cout << "Test testParsingIndexExpressions() START:" << endl;

    bool flag(true);

    string input = "myArray[1 + 1]";

    auto * l = lexer::Lexer::New(input);
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

    auto * indexExpression = dynamic_cast<ast::IndexExpression*>(stmt->getExpression());
    if (!indexExpression) {
        cout << "get ast::IndexExpression Failed." << endl;
        return;
    }

    if (!testIdentifier(indexExpression->getLeft(), "myArray")) {
        flag = false;
    }

    if (!testInfixExpression(indexExpression->getIndex(), 1, "+", 1)) {
        flag = false;
    }

    cout << "Test testParsingIndexExpressions() END: " << (flag ? "PASS" : "FAIL") << endl;
}
