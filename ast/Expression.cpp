#include "Expression.h"

namespace monkey {
    
    namespace ast {
        string Identifier::TokenLiteral() {
            return Token.getLiteral();
        }
    }

}