//
// Created by 86158 on 2024/4/3.
//

#include "Evaluator.h"
#include <iostream>
#include <utility>
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"


using namespace monkey;

using std::cout;
using std::endl;
using std::cerr;




/****************************** ∏®÷˙∫Ø ˝ **************************/
string getResult(const bool & flag) {
    return flag ? "PASS" : "FAIL";
}


object::Object* testEval(string input) {
    auto * l = lexer::Lexer::New(std::move(input));
    auto * p = parser::Parser::New(l);

    auto * program = p->ParseProgram();

    auto * object = new object::Object();
    object = evaluator::Evaluator::Eval(program);

    return object;

}

// ≤‚ ‘Integer Object
bool testIntegerObject(object::Object* object, long long expected) {
    auto * result = dynamic_cast<object::Integer*>(object);
    if (!result) {
        cout << "get result failed. " << endl;
        return false;
    }

    if (auto len = result->getValue() != expected) {
        cout << "object has wrong value. got " << len << " want " << expected << endl;
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
            {"10", 10}
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);
        if (!testIntegerObject(evaluated, test.expected)) {
            flag = false;
        }
    }

    cout << "Test testEvalIntegerExpression() END: " << getResult(flag) << endl;
}