// ast 表达式的实现

#ifndef MONKEY_EXPRESSION_H
#define MONKEY_EXPRESSION_H

#include <utility>

#include "ast.h"
#include "../token/Token.h"

namespace monkey {

    namespace ast {
        
        /**
         * 标识符 表达式
         * 比如 let a = 2; a 就是标识符
         */
        class Identifier : virtual public Expression {
            private:
                token::Token Token; // IDENT 词法单元
                string Value;

            public:
                void expressionNode() override {} // 无作用，可以作为避免报错的手段

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
                 * 返回 Identifier 的 Value (就是字面含义)
                 * @return string, 不返回引用，无法从外部修改
                 */
                string getValue();

                /**
                 * 返回 Identifier 表达式的 String
                 * @return
                 */
                string String() override;
        };

        /**
         * 整数类型 表达式
         */
        class IntegerLiteral : virtual public Expression {
        private:
            token::Token Token;
            long long value; // 64 位的数值

        public:
            /**
             * 构造函数，传入 token 进行构造, 初始值是 0
             * @param token
             */
            __attribute__((unused)) explicit IntegerLiteral(token::Token token);

            void expressionNode() override {}

            /**
             * 从外部获取 Value
             * @return long long, 不能从外部修改
             */
            __attribute__((unused)) long long getValue() const;

            /**
             * 从外部设置 value
             * @param value long long 类型 64位数字
             */
            void setValue(long long value);

            /**
             * 整数类型表达式 TokenLiteral() 的实现
             * @return string
             */
            string TokenLiteral() override;

            /**
             * 整数类型表达式 String() 的实现
             * @return string
             */
            string String() override;
        };

        /**
         * 前缀表达式
         */
        class PrefixExpression : virtual public Expression {
        private:
            token::Token Token; // 前缀词法单元，比如 !, -
            string Operator; // 前缀表达式的运算符号 比如 !
            Expression* Right; // 前缀表达式中运算符号右边的表达式


        public:
            /**
             * 构造函数
             * Right 默认设为空指针
             * @param token
             * @param op string , 运算符
             */
            PrefixExpression(token::Token token, string op);


            /**
             * 从外部设置 Right
             * @param right Expression* 的引用
             */
            void setRightExpression(Expression* & right);

            /**
             * 从外部获取 Right表达式
             * @return Expression* 不返回引用
             */
            ast::Expression* getRightExpression();

            void expressionNode() override {}

            string TokenLiteral() override;

            /**
             * 从外部获取运算符，不可从外部修改
             * @return
             */
            __attribute__((unused)) string getOperator() const;

            /**
             * 获取 前缀表达式的 String() 函数
             * @return
             */
            string String() override;

        };

        /**
         * 中缀表达式 e.g. Left Operator Right
         */
        class InfixExpression : virtual public Expression {
        private:
            token::Token Token; // 运算符词法单元 比如 "+"
            Expression* Left;
            string Operator;
            Expression* Right;

        public:
            __attribute__((unused)) Expression* getLeft();

            __attribute__((unused)) Expression* getRight();

            __attribute__((unused)) string getOperator();

            /**
             * 构造函数， Right 默认设为空指针
             * @param token
             * @param left
             * @param op operator, string
             */
            InfixExpression(token::Token token, Expression* left, string op);

            void expressionNode() override {}

            // InfixExpression TokenLiteral() 的实现
            string TokenLiteral() override;

            string String() override;

            /**
             * 从外部设置 Right
             * @param right Expression* 的引用
             */
            void setRightExpression(Expression* & right);

        };

    } // ast

} // monkey


#endif //MONKEY_EXPRESSION_H