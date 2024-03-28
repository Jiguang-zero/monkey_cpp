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
        }

        void LetStatement::setName(Identifier *identifier) {
            Name = identifier;
        }

        __attribute__((unused)) string LetStatement::String() {
            return "Token: " + TokenLiteral() + " " + "Identifier: " + Name->String() + " Value: ";
        }
    }
}