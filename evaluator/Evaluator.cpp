//
// Created by 86158 on 2024/4/3.
//

#include <iostream>
#include "Evaluator.h"

monkey::object::Object *monkey::evaluator::Evaluator::Eval(monkey::ast::Node *node) { // NOLINT(*-no-recursion)
    // 程序 program
    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        return evalStatements(program->getStatements());
    }

    // 表达式语句
    else if (auto * expressionStatements = dynamic_cast<ast::ExpressionStatement*>(node)) {
        return Eval(expressionStatements->getExpression());
    }

    // 整数字面量
    else if (auto * integerLiteral = dynamic_cast<ast::IntegerLiteral*>(node)) {
        auto * integerObject = new object::Integer(integerLiteral->getValue());
        return integerObject;
    }

    // 布尔值
    else if (auto * boolean = dynamic_cast<ast::Boolean*>(node)) {
        return nativeBoolToBooleanObject(boolean->getValue());
    }

    // 前缀表达式
    else if (auto * prefixExpression = dynamic_cast<ast::PrefixExpression*>(node)) {
        auto * right = Eval(prefixExpression->getRightExpression());
        return evalPrefixExpression(prefixExpression->getOperator(), right);
    }

    return nullptr;
}

monkey::object::Object *monkey::evaluator::Evaluator::evalStatements(const vector<ast::Statement *>& stmts) { // NOLINT(*-no-recursion)
    auto * object = new object::Object();

    for (const auto & s : stmts) {
        object = Eval(s);
    }

    return object;
}

namespace monkey::evaluator {
    object::Boolean *Evaluator::nativeBoolToBooleanObject(bool value) {
        if (value) {
            return const_cast<object::Boolean *>(&TRUE);
        }
        else {
            return const_cast<object::Boolean *>(&FALSE);
        }
    }

    object::Object *Evaluator::evalPrefixExpression(const string& op, object::Object *right) {
        if (op == "!") {
            return evalBangOperatorExpression(right);
        }

        else if (op == "-") {
            return evalMinusPrefixOperatorExpression(right);
        }

        else {
            return (object::Object *) &MY_NULL;
        }
    }

    object::Object *Evaluator::evalBangOperatorExpression(object::Object *right) {
        // if (right == &TRUE) {
        //      return (object::Object *) &FALSE;
        // }

        // 如果 ! 假 或者 ! 空， 则为真
        // 如果 ! 真 或者 ! 任何对象，则为假
        if (right == &FALSE || right == &MY_NULL) {
            return (object::Object *) &TRUE;
        }
        else {
            return (object::Object *) &FALSE;
        }
    }

    object::Object *Evaluator::evalMinusPrefixOperatorExpression(object::Object *right) {
        if (right->Type() != object::INTEGER_OBJ) {
            return (object::Object *) &MY_NULL;
        }

        long long value = (dynamic_cast<object::Integer*>(right))->getValue();

        auto * res = new object::Integer(-value);
        return res;
    }
}

// 定义变量
namespace monkey::evaluator {
    const object::Boolean Evaluator::TRUE = object::Boolean(true);
    const object::Boolean Evaluator::FALSE = object::Boolean(false);
    const object::Null Evaluator::MY_NULL = object::Null();

}
