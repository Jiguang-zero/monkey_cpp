#include "Statement.h"

namespace monkey {
    namespace ast {
        string LetStatement::TokenLiteral() {
            return Token.getLiteral();
        }
    }
}