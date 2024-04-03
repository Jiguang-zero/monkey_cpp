//
// Created by 86158 on 2024/4/3.
//

#include "Evaluator.h"

monkey::object::Object *monkey::evaluator::Evaluator::Eval(monkey::ast::Node *node) {
    auto * object = new object::Object();

    if (auto * program = dynamic_cast<ast::Program*>(node)) {
        object = evalStatements(program->getStatements());
        return object;
    }

    else if (auto * expressionStatements = dynamic_cast<ast::ExpressionStatement*>(node)) {
        object = Eval(expressionStatements->getExpression());
        return object;
    }

    else if (auto * integerLiteral = dynamic_cast<ast::IntegerLiteral*>(node)) {
        auto * integerObject = new object::Integer(integerLiteral->getValue());
        object = integerObject;
        return object;
    }

    return nullptr;
}

monkey::object::Object *monkey::evaluator::Evaluator::evalStatements(const vector<ast::Statement *>& stmts) {
    auto * object = new object::Object();

    for (const auto & s : stmts) {
        object = Eval(s);
    }

    return object;
}
