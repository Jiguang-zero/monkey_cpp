#include "Statement.h"

#include <utility>

namespace monkey {
    namespace ast {
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
    }
}