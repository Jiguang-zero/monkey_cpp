//
// Created by 86158 on 2024/4/3.
//

#include <iostream>
#include "Evaluator.h"

monkey::object::Object *monkey::evaluator::Evaluator::Eval(ast::Node *node, object::Environment *& env) { // NOLINT(*-no-recursion)
    // ���� program
    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        return evalProgram(program->getStatements(), env);
    }

    // ���ʽ���
    else if (auto * expressionStatements = dynamic_cast<ast::ExpressionStatement*>(node)) {
        return Eval(expressionStatements->getExpression(), env);
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
        auto * right = Eval(prefixExpression->getRightExpression(), env);
        if (isError(right)) {
            return right;
        }
        return evalPrefixExpression(prefixExpression->getOperator(), right);
    }

    // ��׺���ʽ
    else if (auto * infixExpression = dynamic_cast<ast::InfixExpression*>(node)) {
        auto * left = Eval(infixExpression->getLeft(), env);
        if (isError(left)) {
            return left;
        }

        auto * right = Eval(infixExpression->getRight(), env);
        if (isError(right)) {
            return right;
        }

        return evalInfixExpression(infixExpression->getOperator(), left, right);
    }

    // ������� (blockStatement)
    else if (auto * blockStatement = dynamic_cast<ast::BlockStatement*>(node)) {
        return evalBlockStatement(blockStatement, env);
    }

    // ѡ�����
    else if (auto * ifExpression = dynamic_cast<ast::IfExpression*>(node)) {
        return evalIfExpression(ifExpression, env);
    }

    // return ���
    else if (auto * returnStatement = dynamic_cast<ast::ReturnStatement*>(node)) {
        auto * value = Eval(returnStatement->getReturnValue(), env);
        if (isError(value)) {
            return value;
        }
        return new object::ReturnValue(value);
    }

    // let ��� (��������)
    else if (auto * letStatement = dynamic_cast<ast::LetStatement*>(node)) {
        auto * value = Eval(letStatement->getValue(), env);
        if (isError(value)) {
            return value;
        }
        env->Set(letStatement->getName()->getValue(), value);
    }

    // ��ʶ�� ������
    else if (auto * identifier = dynamic_cast<ast::Identifier*>(node)) {
        auto * res = evalIdentifier(identifier, env);
        return res;
    }

    return nullptr;
}

monkey::object::Object *monkey::evaluator::Evaluator::evalProgram(const std::vector<ast::Statement *> &stmts, // NOLINT(*-no-recursion)
                                                          object::Environment *& env) {
    auto * object = new object::Object();

    for (const auto & s : stmts) {
        object = Eval(s, env);

        if (auto * returnValue = dynamic_cast<object::ReturnValue*>(object)) {
            return returnValue->getValue();
        }
        else if (auto * error = dynamic_cast<object::Error*>(object)) {
            return error;
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
            return newError("unknown operator: " + op + " " + right->Type());
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
            return newError("unknown operator: -" + right->Type());
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

        else if (left->Type() != right->Type()) {
            return newError("type mismatch: " + left->Type() + " " + op + " " + right->Type());
        }

        else {
            return newError("unknown operator: " + left->Type() + " " + op + " " + right->Type());
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
            return newError("unknown operator: " + left->Type() + " " + op + " " + right->Type());
        }
    }

    object::Object *Evaluator::evalIfExpression(ast::IfExpression *expression, object::Environment *& env) { // NOLINT(*-no-recursion)
        auto * condition = Eval(expression->getCondition(), env);
        if (isError(condition)) {
            return condition;
        }

        if (isTruthy(condition)) {
            return Eval(expression->getConsequence(), env);
        }
        // �����֧��Ϊ��
        else if (auto * alternative = expression->getAlternative()) {
                return Eval(alternative, env);
        }
        else {
            return (object::Object *) &MY_NULL;
        }
    }

    bool Evaluator::isTruthy(object::Object *object) {
        // ���Ϊ�գ� ���ֵΪ��
        if (object == (object::Object*) &MY_NULL || object == (object::Object*) &FALSE) {
            return false;
        }
        else if (object == (object::Object*) &TRUE) {
            return true;
        }

        // ����ֵһ����Ϊ true (�������)
        return true;

    }

    object::Object *Evaluator::evalBlockStatement(ast::BlockStatement *block, object::Environment *& env) { // NOLINT(*-no-recursion)
        auto * result = new object::Object();

        for (const auto & statement : block->getStatements()) {
            result = Eval(statement, env);

            if (result &&
                (result->Type() == object::RETURN_VALUE_OBJ ||
                result->Type() == object::ERROR_OBJ)
                ) {
                return result;
            }
        }

        return result;
    }

    object::Error *Evaluator::newError(const string& error) {
        return new object::Error(error);
    }

    bool Evaluator::isError(monkey::object::Object *obj) {
        if (obj != nullptr) {
            return obj->Type() == object::ERROR_OBJ;
        }
        return false;
    }

    object::Object *
    Evaluator::evalIdentifier(monkey::ast::Identifier *node, monkey::object::Environment *& env) {
        auto * val = env->Get(node->getValue());
        if (val == nullptr) {
            return newError("identifier not found: " + node->getValue());
        }

        return val;
    }
}

// �������

const monkey::object::Boolean monkey::evaluator::Evaluator::TRUE = object::Boolean(true);
const monkey::object::Boolean monkey::evaluator::Evaluator::FALSE = object::Boolean(false);
const monkey::object::Null monkey::evaluator::Evaluator::MY_NULL = object::Null();

