//
// Created by 86158 on 2024/4/3.
//

#include <iostream>
#include "Evaluator.h"

monkey::object::Object *monkey::evaluator::Evaluator::Eval(monkey::ast::Node *node) { // NOLINT(*-no-recursion)
    // 程序 program
    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        return evalProgram(program->getStatements());
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

    // 中缀表达式
    else if (auto * infixExpression = dynamic_cast<ast::InfixExpression*>(node)) {
        auto * left = Eval(infixExpression->getLeft());
        auto * right = Eval(infixExpression->getRight());
        return evalInfixExpression(infixExpression->getOperator(), left, right);
    }

    // 区块语句 (blockStatement)
    else if (auto * blockStatement = dynamic_cast<ast::BlockStatement*>(node)) {
        return evalBlockStatement(blockStatement);
    }

    else if (auto * ifExpression = dynamic_cast<ast::IfExpression*>(node)) {
        return evalIfExpression(ifExpression);
    }

    else if (auto * returnStatement = dynamic_cast<ast::ReturnStatement*>(node)) {
        auto * value = Eval(returnStatement->getReturnValue());
        return new object::ReturnValue(value);
    }

    return nullptr;
}

monkey::object::Object *monkey::evaluator::Evaluator::evalProgram(const std::vector<ast::Statement *> &stmts) { // NOLINT(*-no-recursion)
    auto * object = new object::Object();

    for (const auto & s : stmts) {
        object = Eval(s);

        if (auto * returnValue = dynamic_cast<object::ReturnValue*>(object)) {
            return returnValue->getValue();
        }
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

    object::Object *Evaluator::evalInfixExpression(const string &op, object::Object *left, object::Object *right) {
        if (left->Type() == object::INTEGER_OBJ && right->Type() == object::INTEGER_OBJ) {
            return evalInfixIntegerExpression(op, left, right);
        }

        // 不是整数，目前就支持布尔值
        else if (op == "==") {
            return nativeBoolToBooleanObject(left == right);
        }
        else if (op == "!=") {
            return nativeBoolToBooleanObject(left != right);
        }

        else {
            return (object::Object *) (&MY_NULL);
        }
    }

    object::Object *
    Evaluator::evalInfixIntegerExpression(const string &op, object::Object *left, object::Object *right) {
        auto leftVal = dynamic_cast<object::Integer*>(left)->getValue();
        auto rightVal = dynamic_cast<object::Integer*>(right)->getValue();

        if (op == "+") {
            return new object::Integer(leftVal + rightVal);
        }
        else if (op == "-") {
            return new object::Integer(leftVal - rightVal);
        }
        else if (op == "*") {
            return new object::Integer(leftVal * rightVal);
        }
        else if (op == "/") {
            //TODO: 错误处理，除数不能为 0
            return new object::Integer(leftVal / rightVal);
        }

        else if (op == "<") {
            return nativeBoolToBooleanObject(leftVal < rightVal);
        }
        else if (op == ">") {
            return nativeBoolToBooleanObject(leftVal > rightVal);
        }
        else if (op == "==") {
            return nativeBoolToBooleanObject(leftVal == rightVal);
        }
        else if (op == "!=") {
            return nativeBoolToBooleanObject(leftVal != rightVal);
        }


        else {
            return (object::Object *) (&MY_NULL);
        }
    }

    object::Object *Evaluator::evalIfExpression(ast::IfExpression *expression) { // NOLINT(*-no-recursion)
        auto * condition = Eval(expression->getCondition());

        if (isTruthy(condition)) {
            return Eval(expression->getConsequence());
        }
        // 如果分支不为空
        else if (auto * alternative = expression->getAlternative()) {
            return Eval(alternative);
        }
        else {
            return (object::Object *) &MY_NULL;
        }
    }

    bool Evaluator::isTruthy(object::Object *object) {
        // 如果为空， 这个值为假
        if (object == (object::Object*) &MY_NULL || object == (object::Object*) &FALSE) {
            return false;
        }
        else if (object == (object::Object*) &TRUE) {
            return true;
        }

        // 其他值一律设为 true (对象存在)
        return true;

    }

    object::Object *Evaluator::evalBlockStatement(monkey::ast::BlockStatement *block) { // NOLINT(*-no-recursion)
        auto * result = new object::Object();

        for (const auto & statement : block->getStatements()) {
            result = Eval(statement);

            if (result && result->Type() == object::RETURN_VALUE_OBJ) {
                return result;
            }
        }

        return result;
    }

}

// 定义变量

const monkey::object::Boolean monkey::evaluator::Evaluator::TRUE = object::Boolean(true);
const monkey::object::Boolean monkey::evaluator::Evaluator::FALSE = object::Boolean(false);
const monkey::object::Null monkey::evaluator::Evaluator::MY_NULL = object::Null();









