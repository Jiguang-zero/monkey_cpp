#include "Expression.h"

#include <utility>

namespace monkey {
    
    namespace ast {
        string Identifier::TokenLiteral() {
            return Token.getLiteral();
        }

        Identifier::Identifier(token::Token t, string v) {
            Token = std::move(t);
            Value = std::move(v);
        }

        string Identifier::String() {
            return Token.getLiteral() + " " + Value;
        }
    }

}