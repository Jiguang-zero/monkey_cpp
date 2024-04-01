//
// Created by 86158 on 2024/3/27.
//

#include "Parser.h"
#include <iostream>
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

/******* �������� *************/

// ����������ã���ô������� Statement�Ŀ�������������ݶ�ʧ
// important
bool testLetStatement(ast::Statement& s, const string& name) {
    if (s.TokenLiteral() != "let") {
        cout << "TokenLiteral not 'let', but " << s.TokenLiteral() << endl;
        return false;
    }

    // ת���̳л��� ���͵�ָ��
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

    // ���exit ���򲻺õ���
    // exit(-1);
}

// �ж� integer ���ʽ�������Ƿ���ȷ
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

/************ ���Ժ��� ****************/
// ���� let ���
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

// ���� return ���
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

// ���Ա�ʶ��
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

// ������������
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

// ����ǰ׺���ʽ
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

        // �ͷſռ�
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
            input = i;
            leftValue = lV;
            op = o;
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

    int i = 0;

    for (auto test : tests) {
        cout << "Round " << i++ << ": " << endl;

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

        // �ͷſռ�
        delete exp;
        exp = 0;
        delete stmt;
        stmt = 0;
        delete program;
        program = 0;
        delete p;
        p = 0;
        delete l;
        l = 0;
    }

    cout << "Test testParsingInfixExpressions() END: " << (flag ? "PASS" : "FAIL") << endl;



}
