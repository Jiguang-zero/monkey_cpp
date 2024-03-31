#ifndef MONKEY_PARSER_H
#define MONKEY_PARSER_H

#include "../token/Token.h"
#include "../lexer/Lexer.h"
#include "../ast/Program.h"

namespace monkey {
    namespace parser {
        // 定义函数指针类型 返回 ast::Expression() 的函数 的指针 为 prefixParseFn
        typedef ast::Expression (*prefixParseFn)(ast::Expression**);

        // 定义函数指针类型 返回 ast::Expression(), 参数为 ast::Expression() 的 函数 的指针为 infixParseFn
        typedef ast::Expression (*infixParseFn) (ast::Expression);

        /**
         * 枚举类型，表示 PRECEDENCE 优先级
         */
        enum PRECEDENCE {
            LOWEST = 0, // 最低的优先级
            EQUALS, // =
            LESS_GREATER, // > <
            SUM, // +
            PRODUCT, //*
            PREFIX, // -X or !X
            CALL // myFunction(x)
        };

        // 语法解析器
        class Parser {
        protected:
            lexer::Lexer* l; // 词法解析器

            token::Token curToken; // 当前词法单元
            token::Token peekToken; // 下一个词法单元

            vector<string> errors; // 错误处理

            // 前缀函数映射集
            map<token::TokenType, prefixParseFn> prefixParseFns;

            // 中缀函数映射集
            map<token::TokenType, infixParseFn> infixParseFns;

        private:
            // 构造函数， private， 仅能通过New创建语法解析器
            explicit Parser(lexer::Lexer* _l);

            // 获取下一个词法单元 Token
            void nextToken();

            /**
             * 解析 let 语句
             * @param stmt 指向 let语句节点的指针 的指针
             */
             void parseLetStatement(ast::LetStatement** stmt);

             /**
              * 解析 return 语句
              * @param stmt 指向 return 语句节点指针 的指针
              */
             void parseReturnStatement(ast::ReturnStatement** stmt);

             /**
              * 解析 expression 语句
              * @param stmt 指向 expression 语句节点的指针 的指针
              */
              void parseExpressionStatement(ast::ExpressionStatement** stmt);

              /**
               * 解析表达式
               * @param precedence PRECEDENCE 枚举类型，表示优先级
               * @param leftExpression ast::Expression*&, 对表达式指针进行修改
               */
              void parseExpression(const PRECEDENCE& precedence, ast::Expression*& leftExpression);

              /**
               * 解析 Identifier 表达式
               * @param expression 指向Expression** 类型指针 的指针
               */
              static ast::Expression* parseIdentifier(parser::Parser& p);

            /**
             * 判断当前token的类型是否与想要的一样
             * @param t
             * @return
             */
            bool curTokenIs(const token::TokenType& t);

            /**
             * 判断下一个token的类型是否与想要的一样
             * @param t
             * @return
             */
            bool peekTokenIs(const token::TokenType& t);

            /**
             * 判断下一个token的类型是否与想要的一样，如果一样，移动词法单元
             * @param t
             * @return
             */
            bool expectPeek(const token::TokenType& t);

            /**
             * 当下一个 token 类型错误时，添加错误信息
             * @param t const token::TokenType&，使用常引用避免额外内存开销
             */
            void peekError(const token::TokenType& t);


            /**
             * 在 parser 的前缀函数映射集中增加函数
             * @param tokenType token::TokenType
             * @param fn prefixParseFn
             */
            void registerPrefix(const token::TokenType& tokenType, prefixParseFn fn) {
                prefixParseFns[tokenType] = fn;
            }

            /**
             * 在 parser 的中缀函数映射集中增加函数
             * @param tokenType token::TokenType
             * @param fn infixParseFn
             */
            void registerInfix(const token::TokenType& tokenType, infixParseFn fn) {
                infixParseFns[tokenType] = fn;
            }

        public:
            /**
             * 新建一个语法解析器
             * @param l Lexer*, 传入一个词法解析器的指针
             * @return 返回指向语法解析器的指针
             */
            static Parser* New(lexer::Lexer* l);

            /**
             * 解析 program
             * @param program  指向 program 节点 (ast的根节点) 的指针 的指针
             */
            void ParseProgram(ast::Program** program);


            /**
             * 解析 statement
             * @param stmt  指向statement 节点的指针
             */
            void parseStatement(ast::Statement** stmt);

            /**
             * 获取 parser 的错误信息
             * 不可从外部修改
             * @return vector<string>a
             */
            __attribute__((unused)) vector<string> getErrors();
        };

    }
}

#endif //MONKEY_PARSER_H