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



/****************************** ???????? **************************/
string getResult(const bool & flag) {
    return flag ? "PASS" : "FAIL";
}


object::Object* testEval(string input) {
    auto * l = lexer::Lexer::New(std::move(input));
    auto * p = parser::Parser::New(l);

    auto * program = p->ParseProgram();
    auto * env = object::Environment::NewEnvironment();

    return evaluator::Evaluator::Eval(program, env);

}

// ????Integer Object
bool testIntegerObject(object::Object* object, long long expected) {
    auto * result = dynamic_cast<object::Integer*>(object);
    if (!result) {
        cout << "get result integer failed. " << endl;
        return false;
    }

    if (result->getValue() != expected) {
        cout << "object has wrong value. got " << result->getValue() << " want " << expected << endl;
        return false;
    }

    return true;
}

// ???? Boolean object
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

// ???? null ???? (?§Ø?????? Null ???? )
bool testNullObject(object::Object* object) {
    if (object != (object::Object *)(&evaluator::Evaluator::MY_NULL)) {
        cout << "object " << object->Inspect() << " is not NULL. ";
        cout << " But " << object->Type() << endl;
        return false;
    }

    return true;
}





/****************************** ??????? **************************/
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

void testErrorHandling() {
    cout << "Test testErrorHandling() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        string expectedMessage;
    };

    vector<TestType> tests = {
            {
                    "5 + true;",
                    "type mismatch: INTEGER + BOOLEAN",
            },
            {
                    "5 + true; 5;",
                    "type mismatch: INTEGER + BOOLEAN",
            },
            {
                    "-true",
                    "unknown operator: -BOOLEAN",
            },
            {
                    "true + false;",
                    "unknown operator: BOOLEAN + BOOLEAN",
            },
            {
                    "5; true + false; 5",
                    "unknown operator: BOOLEAN + BOOLEAN",
            },
            {
                    "if (10 > 1) { true + false; }",
                    "unknown operator: BOOLEAN + BOOLEAN",
            },
            {
                "if (10 > 1) { if (10 > 1) { return true + false;} return 1; }",
                "unknown operator: BOOLEAN + BOOLEAN"
            },
            {
                    "foo_bar",
                    "identifier not found: foo_bar",
            },
            {
                    R"("Hello" - "World")",
                    "unknown operator: STRING - STRING",
            },
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);

        auto * errObj = dynamic_cast<object::Error*>(evaluated);
        if (!errObj) {
            cout << "no error returned. got " << evaluated->Type() << endl;
            flag = false;
            continue;
        }

        if (errObj->getMessage() != test.expectedMessage) {
            flag = false;
            cout << "wrong error message. expected " << test.expectedMessage;
            cout << " but " << errObj->getMessage() << endl;
        }
    }



    cout << "Test testErrorHandling() END: " << getResult(flag) << endl;
}

void testLetStatement() {
    cout << "Test testLetStatement() Start:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        long long expected;
    };

    vector<TestType> tests = {
            {"let a = 5; a;", 5},
            {"let a = 5 * 5; a;", 25},
            {"let a = 5; let b = a; b;", 5},
            {"let a = 5; let b = a; let c = a + b + 5 * 10; c;", 60},
            {"let a = 5; let c = a * 99; let d = if (c > a) { 99 } else { 100 }; d; ", 99}
    };

    for (const auto & test : tests) {
        if (!testIntegerObject(testEval(test.input), test.expected)) {
            flag = false;
        }
    }

    cout << "Test testLetStatement() END: " << getResult(flag) << endl;
}

void testFunctionObject() {
    cout << "Test testFunctionObject() START:" << endl;

    bool flag(true);

    string input = "fn(x) { x + 2;}";

    auto * evaluated = testEval(input);
    auto * fn = dynamic_cast<object::Function*>(evaluated);
    if (!fn) {
        cerr << "Get object::Function FAILED." << endl;
        return;
    }

    if (fn->getParameters().size() != 1) {
        cout << "function has wrong parameters: " << fn->getParameters().size() << endl;
        flag = false;
    }

    if (fn->getParameters()[0]->String() != "x") {
        cout << "parameter is not 'x', but " << fn->getParameters()[0]->String() << endl;
        flag = false;
    }

    string expectedBody = "(x + 2)";

    if (fn->getBody()->String() != expectedBody) {
        cout << "body is not " << expectedBody << " but " << fn->getBody()->String() << endl;
        flag = false;
    }

    cout << "Test testFunctionObject() END: " << getResult(flag) << endl;
}

void testFunctionApplication() {
    cout << "Test testFunctionApplication() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        long long expected;
    };

    vector<TestType> tests = {
            {"let identity = fn(x) { x; }; identity(5);", 5},
            {"let identity = fn(x) { return x; }; identity(5);", 5},
            {"let double = fn(x) { x * 2; }; double(5);", 10},
            {"let add = fn(x, y) { x + y; }; add(5, 5);", 10},
            {"let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20},
            {"fn(x) { x; }(5)", 5}
    };

    for (const auto & test :tests) {
        if (!testIntegerObject(testEval(test.input), test.expected)) {
            flag = false;
        }
    }

    cout << "Test testFunctionApplication() END: " << getResult(flag) << endl;
}

void testStringLiteral() {
    cout << "Test testStringLiteral() START:" << endl;

    bool flag(true);

    string input = "\"hello world\"";

    auto * evaluated = testEval(input);
    auto * stringLiteral = dynamic_cast<object::String*>(evaluated);
    if (!stringLiteral) {
        cout << "not string. but " << evaluated->Type() << endl;
        return;
    }

    if (stringLiteral->getValue() != "hello world") {
        cout << "no hello world. but " << stringLiteral->getValue() << endl;
        flag = false;
    }

    cout << "Test testStringLiteral() END: " << getResult(flag) << endl;
}

void testStringConcatenation() {
    cout << "Test testStringConcatenation START:" << endl;

    bool flag(true);

    string input = R"("Hello" + " " + "World!")";

    auto * evaluated = testEval(input);
    auto * stringLiteral = dynamic_cast<object::String*>(evaluated);
    if (!stringLiteral) {
        cout << "not string. but " << evaluated->Type() << endl;
        return;
    }

    if (stringLiteral->getValue() != "Hello World!") {
        cout << "not Hello World!. but " << stringLiteral->getValue() << endl;
        flag = false;
    }

    cout << "Test testStringConcatenation END: " << getResult(flag) << endl;
}

void testBuiltinFunctions() {
    cout << "Test testBuiltinFunctions() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        variant<long long, string> expected;
    };

    vector<TestType> tests {
            {R"(len(""))", 0},
            {R"(len("four"))", 4},
            {R"(len("hello world"))", 11},
            {R"(len(1))", "argument to `len` not supported, got INTEGER"},
            {R"(len("one", "two"))", "wrong number of arguments. got=2, want=1"},
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);

        if (holds_alternative<long long>(test.expected)) {
            if (!testIntegerObject(evaluated, get<long long>(test.expected))) {
                flag = false;
            }
        }
        else if (holds_alternative<string>(test.expected)) {
            auto * errObj = dynamic_cast<object::Error*>(evaluated);
            if (!errObj) {
                cout << "get error wrong. " << evaluated->Type() << endl;
                flag = false;
                continue;
            }
            if (errObj->getMessage() != get<string>(test.expected)) {
                cout << "wrong error not " << get<string>(test.expected);
                cout << " but " << errObj->getMessage() << endl;
                flag = false;
            }
        }

    }

    cout << "Test testBuiltinFunctions() END: " << getResult(flag) << endl;
}

void testEvalArrayLiterals() {
    cout << "Test testEvalArrayLiterals() START:" << endl;

    bool flag(true);

    string input = "[1, 2 * 2, 3 + 3]";

    auto * evaluated = testEval(input);
    auto * array = dynamic_cast<object::Array*>(evaluated);
    if (!array) {
        cerr << "get array wrong." << endl;
        return;
    }

    if (array->getElements().size() != 3) {
        cout << "elements.size() not 3 but " << array->getElements().size() << endl;
        flag = false;
    }

    if (!testIntegerObject(array->getElements()[0], 1) ||
        !testIntegerObject(array->getElements()[1], 4) ||
        !testIntegerObject(array->getElements()[2], 6)) {
        flag = false;
    }

    cout << "Test testEvalArrayLiterals() END: " << getResult(flag) << endl;
}

void testEvalArrayIndexExpressions() {
    cout << "Test testEvalArrayIndexExpressions() START:" << endl;

    bool flag(true);

    struct TestType {
        string input;
        variant<long long, void*> expected;
    };

    vector<TestType> tests = {
            {"[1, 2, 3][0]",
                    1,
            },
            {
             "[1, 2, 3][1]",
                    2,
            },
            {
             "[1, 2, 3][2]",
                    3,
            },
            {
             "let i = 0; [1][i];",
                    1,
            },
            {
             "[1, 2, 3][1 + 1];",
                    3,
            },
            {
             "let myArray = [1, 2, 3]; myArray[2];",
                    3,
            },
            {
             "let myArray = [1, 2, 3]; myArray[0] + myArray[1] + myArray[2];",
                    6,
            },
            {
             "let myArray = [1, 2, 3]; let i = myArray[0]; myArray[i]",
                    2,
            },
            {
                    "[1, 2, 3][3]",
                    nullptr,
            },
            {
                    "[1, 2, 3][-1]",
                    nullptr,
            }
    };

    for (const auto & test : tests) {
        auto * evaluated = testEval(test.input);
        if (holds_alternative<long long>(test.expected)) {
            if (!testIntegerObject(evaluated, get<long long>(test.expected))) {
                flag = false;
            }
        }
        else {
            if (!testNullObject(evaluated)) {
                flag = false;
            }
        }
    }

    cout << "Test testEvalArrayIndexExpressions() END: " << getResult(flag) << endl;
}
