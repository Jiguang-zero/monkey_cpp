// ast 中语句的实现

#ifndef MONKEY_STATEMENT_H
#define MONKEY_STATEMENT_H

#include "../token/Token.h"
#include "Expression.h"

namespace monkey {
    namespace ast {

        // Let 语句的实现 
        class LetStatement : virtual public Statement {
        private:
            token::Token Token; //Let 词法单元
            Identifier* Name;
            Expression* Value;

        public:
            /**
             * 构造函数，传入词法单元
             * @param token
             */
            __attribute__((unused)) explicit LetStatement(token::Token token);

            /**
             * 构造函数
             * @param token 词法单元
             * @param name Identifier 表达式
             * @param value Expression 表达式
             */
            __attribute__((unused)) LetStatement(token::Token token, Identifier*  name, Expression* value);

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


            // 返回 Let 语句的 String() 函数
            __attribute__((unused)) string String() override;
        };

        // Return 语句的实现
        class ReturnStatement : virtual public Statement {
        private:
            token::Token Token;  // RETURN 词法单元
            Expression* ReturnValue;

        public:
            void statementNode() override {}

            explicit ReturnStatement(token::Token token);

            // Return 语句 TokenLiteral 的实现
            string TokenLiteral() override;

            // 返回 ReturnStatement() 的String
            string String() override;


        };

        // 表达式 语句的实现
        class ExpressionStatement : virtual public Statement {
        private:
            token::Token Token; // 表达式的第一个词法单元
            Expression* Expression;

        public:
            /**
             * 从外部设置 Expression
             * @param expression  class Expression*& 使用引用，避免复制的时候丢失数据
             */
            __attribute__((unused)) void setExpression(class Expression*& expression);;

            explicit ExpressionStatement(token::Token token);

            void statementNode() override {}

            string TokenLiteral() override;

            /**
             * 获取 表达式 Expression 的 Expression
             * @return Expression* 无法从外部修改
             */
            class Expression* getExpression();

            string String() override;

        };
    }
}


#endif