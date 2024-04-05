//
// Created by 86158 on 2024/4/3.
//

#include "Evaluator.h"

#include <iostream>
#include <utility>
#include <variant>

#include "../lexer/Lexer.h"
#include "../parser/Parser.h"


using namespace monkey;

using std::cout;
using std::endl;
using std::cerr;
using std::variant;
using std::holds_alternative;
using std::get;



/****************************** ∏®÷˙∫Ø ˝ **************************/
string getResult(const bool & flag) {
    return flag ? "PASS" : "FAIL";
}


object::Object* testEval(string input) {
    auto * l = lexer::Lexer::New(std::move(input));
    auto * p = parser::Parser::New(l);

    auto * program = p->ParseProgram();

    return evaluator::Evaluator::Eval(program);

}

// ≤‚ ‘Integer Object
bool testIntegerObject(object::Object* object, long long expected) {
    auto * result = dynamic_cast<object::Integer*>(object);
    if (!result) {
        cout << "get result integer failed. " << endl;
        return false;
    }

    if (auto value = result->getValue() != expected) {
        cout << "object has wrong value. got " << value << " want " << expected << endl;
        return false;
    }

    return true;
}

// ≤‚ ‘ Boolean object
bool testBooleanObject(object::Object* object, bool expected) {
    auto * result = dynamic_cast<object::Boolean*>(object);
    if (!result) {
        cout << "get result boolean failed. " << endl;
        return false;
    }

    if (auto value = result->getValue() != expected) {
        cout << "object has wrong value. got " << value << " want " << expected << endl;
        return false;
    }

    return true;
}

// ≤‚ ‘ null ∂‘œÛ (≈–∂œ «∑Ò « Null ∂‘œÛ )
bool testNullObject(object::Object* object) {
    if (object != (object::Object *)(&evaluator::Evaluator::MY_NULL)) {
        cout << "object " << object->Inspect() << " is not NULL. ";
        cout << " But " << object->Type() << endl;
        return false;
    }

    return true;
}





/****************************** ≤‚ ‘∫Ø ˝ **************************/
void testEvalIntegerExpression() {
    cout << "Test testEvalIntegerExpression() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        long long expected;
    };

    vector<TestType> tests = {
            {"53", 53},
            {"10", 10},
            {"-19", -19},
            {"-23", -23},
            {"5 + 5 + 5 + 5 - 10", 10},
            {"2 * 2 * 2 * 2 * 2", 32},
            {"-50 + 100 + -50", 0},
            {"5 * 2 + 10", 20},
            {"5 + 2 * 10", 25},
            {"20 + 2 * -10", 0},
            {"50 / 2 * 2 + 10", 60},
            {"2 * (5 + 10)", 30},
            {"3 * 3 * 3 + 10", 37},
            {"3 * (3 * 3) + 10", 37},
            {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50}
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);
        if (!testIntegerObject(evaluated, test.expected)) {
            flag = false;
        }
    }

    cout << "Test testEvalIntegerExpression() END: " << getResult(flag) << endl;
}

void testEvalBooleanExpression() {
    cout << "Test testEvalBooleanExpression() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        bool expected;
    };

    vector<TestType> tests = {
            {"true", true},
            {"false", false},
            {"1 < 2", true},
            {"1 > 2", false},
            {"1 < 1", false},
            {"1 > 1", false},
            {"1 == 1", true},
            {"1 != 1", false},
            {"1 == 2", false},
            {"1 != 2", true},
            {"true == true", true},
            {"false == false", true},
            {"true == false", false},
            {"true != false", true},
            {"false != true", true},
            {"(1 < 2) == true", true},
            {"(1 < 2) == false", false},
            {"(1 > 2) == true", false},
            {"(1 > 2) == false", true}
    };

    for (const auto & test :tests) {
        auto * evaluated = testEval(test.input);
        if (!testBooleanObject(evaluated, test.expected)) {
            flag = false;
        }
    }

    cout << "Test testEvalBooleanExpression() END: " << getResult(flag) << endl;
}

void testBangOperator() {
    cout << "Test testBangOperator() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        bool expected;
    };

    vector<TestType> tests = {
            {"!true", false},
            {"!!false", false},
            {"!!!!!5", false},
            {"!!true",true}
    };

    for (const auto & test :tests) {
        auto * evaluated = testEval(test.input);
        if (!testBooleanObject(evaluated, test.expected)) {
            flag = false;
        }
    }

    cout << "Test testBangOperator() END: " << getResult(flag) << endl;
}

void testIfElseExpressions() {
    cout << "Test testIfElseExpressions() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        variant<long long, void*> expected;
    };

    vector<TestType> tests =  {
            {"if (true) { 10 }", 10},
            {"if (false) { 10 }", nullptr},
            {"if (1) { 10 }", 10},
            {"if (1 < 2) { 10 }", 10},
            {"if (1 > 2) { 10 }", nullptr},
            {"if (1 > 2) { 10 } else { 20 }", 20},
            {"if (1 < 2) { 10 } else { 20 }", 10}
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);
        if (holds_alternative<long long>(test.expected)) {
            if (!testIntegerObject(evaluated, get<long long>(test.expected))) {
                flag = false;
            }
        }
        else if (holds_alternative<void *>(test.expected)) {
            if (!testNullObject(evaluated)) {
                flag = false;
            }
        }
    }

    cout << "Test testIfElseExpressions() END: " << getResult(flag) << endl;
}

void testReturnStatement() {
    cout << "Test testReturnStatement() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        long long expected;
    };

    vector<TestType> tests = {
            {"return 10;", 10},
            {"return 10; 9;", 10},
            {"return 2 * 5; 9;", 10},
            {"9; return 2 * 5; 9;", 10},
            {"if (true) { if(!false) { return 23;} return 3;} else { return 232;}", 23}
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);
        if (!testIntegerObject(evaluated, test.expected)) {
            flag = false;
        }
    }

    cout << "Test testReturnStatement() END: " << getResult(flag) << endl;
}
