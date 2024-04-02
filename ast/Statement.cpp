#include "Statement.h"

#include <utility>


    namespace monkey::ast {
        string LetStatement::TokenLiteral() {
            return Token.getLiteral();
        }

        LetStatement::LetStatement(token::Token token) {
            Token = std::move(token);
            Name = nullptr;
            Value = nullptr;
        }

        void LetStatement::setName(Identifier *identifier) {
            Name = identifier;
        }

        __attribute__((unused)) string LetStatement::String() {
            std::ostringstream oss;

            oss << TokenLiteral() << " ";
            oss << Name->String();
            oss << " = ";

            if (Value != nullptr) {
                oss << Value->String();
            }

            oss << ";";

            return oss.str();
        }

        __attribute__((unused)) Identifier *LetStatement::getName() {return Name;}

        __attribute__((unused)) LetStatement::LetStatement(token::Token token, Identifier *name, Expression *value) {
            Token = std::move(token);
            Name = name;
            Value = value;
        }

        [[maybe_unused]] Expression *LetStatement::getValue() {
            return Value;
        }

        [[maybe_unused]] void LetStatement::setValue(Expression *&value) {
            Value = value;
        }

        string ReturnStatement::TokenLiteral() {
            return Token.getLiteral();
        }

        ReturnStatement::ReturnStatement(token::Token token) {
            Token = std::move(token);
            ReturnValue = nullptr;
        }

        string ReturnStatement::String() {
            std::ostringstream oss;

            oss << TokenLiteral() << " ";

            if (ReturnValue) {
                oss << ReturnValue->String();
            }

            oss << ";";

            return oss.str();
        }

        [[maybe_unused]] void ReturnStatement::setReturnValue(Expression *&value) {
            ReturnValue = value;
        }

        [[maybe_unused]] Expression *ReturnStatement::getReturnValue() {
            return ReturnValue;
        }

        string ExpressionStatement::TokenLiteral() {
            return Token.getLiteral();
        }

        ExpressionStatement::ExpressionStatement(token::Token token) {
            Token = std::move(token);

            Expression = nullptr;
        }

        string ExpressionStatement::String() {
            if (Expression) {
                return Expression->String();
            }
            return "";
        }

        void ExpressionStatement::setExpression(class Expression *&expression) {
            Expression = expression;
        }

        [[maybe_unused]] class Expression *ExpressionStatement::getExpression() {
            return Expression;
        }

        string BlockStatement::String() {
            std::ostringstream oss;

            for (const auto & statement : Statements) {
                oss << statement->String();
            }

            return oss.str();
        }

        string BlockStatement::TokenLiteral() {
            return Token.getLiteral();
        }

        __attribute__((unused)) BlockStatement::BlockStatement(token::Token token) {
            Token = std::move(token);
        }

        __attribute__((unused)) void BlockStatement::setStatements(std::vector<Statement *> statements) {
            Statements = std::move(statements);
        }

        [[maybe_unused]] std::vector<Statement *> & BlockStatement::getStatements() {
            return Statements;
        }
    }
