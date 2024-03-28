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
            /**
             * 外部设置 LetStatement 的 变量名
             * @param identifier Identifier *
             */
            void setName(Identifier* identifier);

            void statementNode() override {};

            /**
             * 获取 Let 语句的Name (不可从外部修改)
             * @return
             */
            __attribute__((unused)) Identifier* getName();

            // let 语句 TokenLiteral 的实现
            string TokenLiteral() override;

            /**
             * 构造函数，传入词法单元
             * @param token
             */
            explicit LetStatement(token::Token token);

            // 返回 Let 语句的 String() 函数
            __attribute__((unused)) string String();
        };
    }
}


#endif