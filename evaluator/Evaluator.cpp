//
// Created by 86158 on 2024/4/3.
//

#include <iostream>
#include "Evaluator.h"

monkey::object::Object *monkey::evaluator::Evaluator::Eval(monkey::ast::Node *node) { // NOLINT(*-no-recursion)
    // ���� program
    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        return evalStatements(program->getStatements());
    }

    // ���ʽ���
    else if (auto * expressionStatements = dynamic_cast<ast::ExpressionStatement*>(node)) {
        return Eval(expressionStatements->getExpression());
    }

    // ����������
    else if (auto * integerLiteral = dynamic_cast<ast::IntegerLiteral*>(node)) {
        auto * integerObject = new object::Integer(integerLiteral->getValue());
        return integerObject;
    }

    // ����ֵ
    else if (auto * boolean = dynamic_cast<ast::Boolean*>(node)) {
        return nativeBoolToBooleanObject(boolean->getValue());
    }

    // ǰ׺���ʽ
    else if (auto * prefixExpression = dynamic_cast<ast::PrefixExpression*>(node)) {
        auto * right = Eval(prefixExpression->getRightExpression());
        return evalPrefixExpression(prefixExpression->getOperator(), right);
    }

    // ��׺���ʽ
    else if (auto * infixExpression = dynamic_cast<ast::InfixExpression*>(node)) {
        auto * left = Eval(infixExpression->getLeft());
        auto * right = Eval(infixExpression->getRight());
        return evalInfixExpression(infixExpression->getOperator(), left, right);
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

        // ��� ! �� ���� ! �գ� ��Ϊ��
        // ��� ! �� ���� ! �κζ�����Ϊ��
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

        // ����������Ŀǰ��֧�ֲ���ֵ
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
            //TODO: ��������������Ϊ 0
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
}

// �������

const monkey::object::Boolean monkey::evaluator::Evaluator::TRUE = object::Boolean(true);
const monkey::object::Boolean monkey::evaluator::Evaluator::FALSE = object::Boolean(false);
const monkey::object::Null monkey::evaluator::Evaluator::MY_NULL = object::Null();





