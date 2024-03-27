// ast 中语句的实现

#ifndef MONKEY_STATEMENT_H
#define MONKEY_STATEMENT_H

#include "ast.h"
#include "../token/Token.h"
#include "Expression.h"

namespace monkey {
    namespace ast {

        // Let 语句的实现 
        class LetStatement : virtual public Statement {
        private:
            token::Token Token; //Let 词法单元
            Identifier* Name;
            Expression Value;

        public:
            void statementNode() {};

            // let 语句 TokenLiteral 的实现
            string TokenLiteral();
        };
    }
}


#endif