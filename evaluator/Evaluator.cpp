//
// Created by 86158 on 2024/4/3.
//

#include <iostream>
#include "builtin.h"
#include "Evaluator.h"


monkey::object::Object *monkey::evaluator::Evaluator::Eval(ast::Node *node, object::Environment *& env) { // NOLINT(*-no-recursion)
    // 程序 program
    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        return evalProgram(program->getStatements(), env);
    }

    // 表达式语句
    else if (auto * expressionStatements = dynamic_cast<ast::ExpressionStatement*>(node)) {
        return Eval(expressionStatements->getExpression(), env);
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
        auto * right = Eval(prefixExpression->getRightExpression(), env);
        if (isError(right)) {
            return right;
        }
        return evalPrefixExpression(prefixExpression->getOperator(), right);
    }

    // 中缀表达式
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

    // 区块语句 (blockStatement)
    else if (auto * blockStatement = dynamic_cast<ast::BlockStatement*>(node)) {
        return evalBlockStatement(blockStatement, env);
    }

    // 选择语句
    else if (auto * ifExpression = dynamic_cast<ast::IfExpression*>(node)) {
        return evalIfExpression(ifExpression, env);
    }

    // return 语句
    else if (auto * returnStatement = dynamic_cast<ast::ReturnStatement*>(node)) {
        auto * value = Eval(returnStatement->getReturnValue(), env);
        if (isError(value)) {
            return value;
        }
        return new object::ReturnValue(value);
    }

    // let 语句 (定义声明)
    else if (auto * letStatement = dynamic_cast<ast::LetStatement*>(node)) {
        auto * value = Eval(letStatement->getValue(), env);
        if (isError(value)) {
            return value;
        }
        env->Set(letStatement->getName()->getValue(), value);
    }

    // 标识符 参数名
    else if (auto * identifier = dynamic_cast<ast::Identifier*>(node)) {
        auto * res = evalIdentifier(identifier, env);
        return res;
    }

    // 函数 定义
    else if (auto * functionLiteral = dynamic_cast<ast::FunctionLiteral*>(node)) {
        auto params = functionLiteral->getParameters();
        auto * body = functionLiteral->getBody();

        return new object::Function(params, body, env);
    }

    // 函数调用
    else if (auto * callExpression = dynamic_cast<ast::CallExpression*>(node)) {
        auto * function = Eval(callExpression->getFunction(), env);
        if (isError(function)) {
            return function;
        }

        auto args = evalExpressions(callExpression->getArguments(), env);
        if (args.size() == 1 && isError(args[0])) {
            return args[0];
        }

        return applyFunction(function, args);
    }

    // string
    else if (auto * stringLiteral = dynamic_cast<ast::StringLiteral*>(node)) {
        return new object::String(stringLiteral->getValue());
    }

    // 数组
    else if (auto * arrayLiteral = dynamic_cast<ast::ArrayLiteral*>(node)) {
        auto elements = evalExpressions(arrayLiteral->getElements(), env);

        if (elements.size() == 1 && isError(elements[0])) {
            return elements[0];
        }

        return new object::Array(elements);
    }

    // 索引求值
    else if (auto * indexExpression = dynamic_cast<ast::IndexExpression*>(node)) {
        auto * left = Eval(indexExpression->getLeft(), env);
        if (isError(left)) {
            return left;
        }

        auto * index = Eval(indexExpression->getIndex(), env);
        if (isError(index)) {
            return index;
        }

        return evalIndexExpression(left, index);
    }

    else if (auto * hashLiteral = dynamic_cast<ast::HashLiteral*>(node)) {
        return evalHashLiteral(hashLiteral, env);
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

        // 字符串
        else if (left->Type() == object::STRING_OBJ && right->Type() == object::STRING_OBJ) {
            return evalInfixStringExpression(op, left, right);
        }

        // 支持布尔值
        else if (left->Type() != right->Type()) {
            return newError("type mismatch: " + left->Type() + " " + op + " " + right->Type());
        }
        else if (op == "==") {
            return nativeBoolToBooleanObject(left == right);
        }
        else if (op == "!=") {
            return nativeBoolToBooleanObject(left != right);
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
        // 如果分支不为空
        else if (auto * alternative = expression->getAlternative()) {
                return Eval(alternative, env);
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
        if (val) {
            return val;
        }

        auto * builtin = builtins[node->getValue()];
        if (builtin) {
            return builtin;
        }


        return newError("identifier not found: " + node->getValue());
    }

    vector<object::Object *>
    Evaluator::evalExpressions(const vector<ast::Expression *>& exps, monkey::object::Environment *env) { // NOLINT(*-no-recursion)
        vector<object::Object*> result;

        for (const auto & e : exps) {
            auto * evaluated = Eval(e, env);
            if (isError(evaluated)) {
                // 只要有一个错误就返回错误，此时参数数组的大小肯定为1
                return {evaluated};
            }
            result.emplace_back(evaluated);
        }

        return result;
    }

    object::Object *
    Evaluator::applyFunction(monkey::object::Object *fn, const vector<object::Object *>& args) { // NOLINT(*-no-recursion)
        if (auto * function = dynamic_cast<object::Function*>(fn)) {
            //TODO: 处理参数不匹配的错误 e.g. let add = fn(a, b); let func = fn(a, b, f) {f(a, b)};  此时传入 func(1, 2) 程序会退出
            auto * extendedEnv = extendedFunctionEnv(function, args);
            auto * evaluated = Eval(function->getBody(), extendedEnv);

            return unwrapReturnValue(evaluated);
        }

        else if (auto * builtin = dynamic_cast<object::Builtin*>(fn)) {
            return builtin->getFn()(args);
        }

        else {
            return newError("not a function: " + fn->Type());
        }

    }

    object::Environment *
    Evaluator::extendedFunctionEnv(monkey::object::Function * fn, const vector<object::Object *>& args) {
        auto * env = object::Environment::NewEnclosedEnvironment(fn->getEnv());

        int i = 0;
        for (const auto & param : fn->getParameters()) {
            env->Set(param->getValue(), args[i ++]);
        }

        return env;
    }

    object::Object *Evaluator::unwrapReturnValue(monkey::object::Object *obj) {
        if (auto returnValue = dynamic_cast<object::ReturnValue*>(obj)) {
            return returnValue->getValue();
        }

        return obj;
    }

}

// 定义变量

const monkey::object::Boolean monkey::evaluator::Evaluator::TRUE = object::Boolean(true);
const monkey::object::Boolean monkey::evaluator::Evaluator::FALSE = object::Boolean(false);
const monkey::object::Null monkey::evaluator::Evaluator::MY_NULL = object::Null();

monkey::object::Object *
monkey::evaluator::Evaluator::evalInfixStringExpression(const string &op, monkey::object::Object *left,
                                                        monkey::object::Object *right) {
    if (op != "+") {
        return newError("unknown operator: " + left->Type() + " " + op + " " + right->Type());
    }

    auto leftVal = dynamic_cast<object::String*>(left)->getValue();
    auto rightVal = dynamic_cast<object::String*>(right)->getValue();
    return new object::String(leftVal + rightVal);
}

monkey::object::Object *
monkey::evaluator::Evaluator::evalIndexExpression(monkey::object::Object *left, monkey::object::Object *index) {
    if (left->Type() == object::ARRAY_OBJ && index->Type() == object::INTEGER_OBJ) {
        return evalArrayIndexExpression(left, index);
    }

    else if (left->Type() == object::HASH_OBJ) {
        return evalHashIndexExpression(left, index);
    }

    else {
        return newError("index operator not supported: " + left->Type());
    }


}

monkey::object::Object *
monkey::evaluator::Evaluator::evalArrayIndexExpression(monkey::object::Object *array, monkey::object::Object *index) {
    // 函数调用完成后自动释放空间
    auto * arrayObject = dynamic_cast<object::Array*>(array);

    auto idx = dynamic_cast<object::Integer*>(index)->getValue();
    auto maxIndex = static_cast<long long>(arrayObject->getElements().size() - 1);

    if (idx < 0 || idx > maxIndex) {
        return (object::Object*) (&MY_NULL);
    }

    return arrayObject->getElements()[idx];
}

monkey::object::Object *
monkey::evaluator::Evaluator::evalHashLiteral(monkey::ast::HashLiteral *node, monkey::object::Environment *env) { // NOLINT(*-no-recursion)
    map<object::HashKey, object::HashPair> pairs;

    for (const auto & pair : node->getPairs()) {
        auto * key = Eval(pair.first, env);
        if (isError(key)) {
            return key;
        }

        auto * hashKey = dynamic_cast<object::Hashable*>(key);
        if (!hashKey) {
            auto msg = "unusable as hash key: " + key->Type();
            return newError(msg);
        }

        auto * value = Eval(pair.second, env);
        if (isError(value)) {
            return value;
        }

        object::HashKey hashed = hashKey->HashKey();

        pairs[hashed] = {key, value};
    }

    return new object::Hash(pairs);
}

monkey::object::Object *
monkey::evaluator::Evaluator::evalHashIndexExpression(monkey::object::Object *hash, monkey::object::Object *index) {
    auto * hashObject = dynamic_cast<object::Hash*>(hash);

    auto * key = dynamic_cast<object::Hashable*>(index);
    if (!key) {
        return newError("unusable as hash key: " + index->Type());
    }

    auto pairs = hashObject->getPairs();
    if (pairs.find(key->HashKey()) == pairs.end()) {
        return (object::Object*) &MY_NULL;
    }
    auto pair = pairs[key->HashKey()];

    return pair.Value;

}







