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
                string TokenLiteral() override;

                /**
                 * 构造函数
                 * @param t token::Token 表示该标识符的词法单元 IDENT
                 * @param p string 表示该标识符的字面量 比如 let a = 2 , a 就是 value
                 */
                Identifier(token::Token t, string v);

                /**
                 * 返回 Identifier 表达式的 String
                 * @return
                 */
                string String();
        };

    } // ast

} // monkey


#endif //MONKEY_EXPRESSION_H