// ast 表达式的实现

#ifndef MONKEY_EXPRESSION_H
#define MONKEY_EXPRESSION_H

#include "ast.h"
#include "../token/Token.h"

namespace monkey {

    namespace ast {
        
        /**
         * 标识符 表达式
         */
        class Identifier : virtual public Expression {
            private:
                token::Token Token; // IDENT 词法单元
                string Value;

            public:
                void expressionNode() {} // 无作用，可以作为避免报错的手段

                /**
                 * Identifier TokenLiteral 的实现
                 */
                string TokenLiteral();
        };

    } // ast

} // monkey


#endif //MONKEY_EXPRESSION_H