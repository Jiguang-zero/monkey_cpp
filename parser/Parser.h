#ifndef MONKEY_PARSER_H
#define MONKEY_PARSER_H

#include "../token/Token.h"
#include "../lexer/Lexer.h"
#include "../ast/Program.h"


    namespace monkey::parser {
        /**
         * 枚举类型，表示 PRECEDENCE 优先级
         */
        enum PRECEDENCE {
            LOWEST = 0, // 最低的优先级
            EQUALS, // == , !=
            LESS_GREATER, // > <
            SUM, // +
            PRODUCT, //*
            PREFIX, // -X or !X
            CALL // myFunction(x)
        };



        // 先声明类 Parser
        class Parser;

        // 定义函数指针类型 返回 ast::Expression() 的函数 的指针 为 prefixParseFn
        typedef ast::Expression* (Parser::*prefixParseFn)();

        // 定义函数指针类型 返回 ast::Expression(), 参数为 ast::Expression() 的 函数 的指针为 infixParseFn
        typedef ast::Expression* (Parser::*infixParseFn) (ast::Expression*);



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

            /**
             * 获取下一个词法单元的优先级
             * @oldVersion
             *  map<token::TokenType, PRECEDENCE> Parser::precedences = {
                {token::EQ,         EQUALS},
                {token::NOT_EQ,     EQUALS},
                {token::LT,         LESS_GREATER},
                {token::GT,         LESS_GREATER},
                {token::PLUS,       SUM},
                {token::MINUS,      SUM},
                {token::SLASH,      PRODUCT},
                {token::ASTERISK,   PRODUCT}
                };
                但是作为这个哈希映射作为局部变量也好，作为全局变量也好，会提示警告
                token::EQ可能为未义。
                通过测试 peekToken 的优先级，会发现为0，也就是哈希映射中找不到
                因为 token::EQ 作为静态编译，声明与定义是分开的。

             * @newVersion 在此采用了稍微不那么高雅的解决方案，就是直接if语句选择优先级
             * @return PRECEDENCE 枚举类型
             */
            PRECEDENCE peekPrecedence();

            /**
             * 获取当前词法单元的优先级
             * @return PRECEDENCE 枚举类型
             */
            PRECEDENCE curPrecedence();

        private:
            /**
             * 从 token::TokenType 获取 优先级
             * @param type token::TokenType
             * @return PRECEDENCE
             */
            static PRECEDENCE getPrecedenceWithTokenType(const token::TokenType& type);

            // 构造函数， private， 仅能通过New创建语法解析器
            explicit Parser(lexer::Lexer* _l);

            // 获取下一个词法单元 Token
            void nextToken();

            /**
             * 如果没有前缀表达式的相关函数，则添加错误信息
             * @param t 需要的前缀表达式的符号
             */
            void noPrefixParseFnError(const token::TokenType& t);


             /**
              * 解析 let 语句
              * @return ast::LetStatement* 返回指向 let 语句节点的指针
              */
             ast::LetStatement* parseLetStatement();

             /**
              * 解析 return 语句
              * @return ast::ReturnStatement* 指向 return 语句节点的指针
              */
             ast::ReturnStatement* parseReturnStatement();

              /**
               * 解析 expression 语句
               * @return ast::ExpressionStatement* 指向 expression 语句节点的指针
               */
              ast::ExpressionStatement* parseExpressionStatement();

              /**
               * 解析表达式
               * @param precedence PRECEDENCE 枚举类型，表示优先级
               * @return ast::Expression* 指向表达式的指针
               */
              ast::Expression* parseExpression(const PRECEDENCE& precedence);

              /**
               * 解析 Identifier 表达式
               * @return ast::Expression*
               */
              ast::Expression* parseIdentifier();

              /**
               * 解析 IntegerLiteral 表达式
               * @return ast::Expression* 返回指针
               */
              ast::Expression* parseIntegerLiteral();

              /**
               * 解析 prefix 表达式
               * @return ast::Expression*
               */
              ast::Expression* parsePrefixExpression();


              /**
               * 解析 boolean 表达式
               * @return ast::Expression*
               */
              ast::Expression* parseBoolean();

              /**
               * 解析 string 表达式
               * @return ast::Expression*
               */
              ast::Expression* parseStringLiteral();

              /**
               * 解析 组 表达式 ()括号为一组
               * @return
               */
              ast::Expression* parseGroupedExpression();

              /**
               * 解析 if 表达式
               * @return
               */
              ast::Expression* parseIfExpression();

              /**
               * 解析 函数表达式
               * @return ast::Expression*
               */
              ast::Expression* parseFunctionLiteral();

              /**
               * 解析 infix 表达式
               * @param left  Expression*
               * @return ast::Expression*
               */
              ast::Expression* parseInfixExpression(ast::Expression* left);

              /**
               * 解析 函数调用 表达式
               * @param function ast::Expression* 表示这函数字面量的表达式
               * @return ast::Expression*
               */
              ast::Expression* parseCallExpression(ast::Expression* function);

              /**
               * 解析 语句块
               * @return ast::BlockStatement
               */
              ast::BlockStatement* parseBlockStatement();


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
            void registerPrefix(const token::TokenType &tokenType, prefixParseFn fn) {
                prefixParseFns[tokenType] = fn;
            }

            /**
             * 在 parser 的中缀函数映射集中增加函数
             * @param tokenType token::TokenType
             * @param fn infixParseFn
             */
            void registerInfix(const token::TokenType &tokenType, infixParseFn fn) {
                infixParseFns[tokenType] = fn;
            }

            /**
             * 解析获取函数表达式的所有参数
             * @return vector<ast::Identifier*>
             */
            vector<ast::Identifier*> parseFunctionParameters();

            /**
             * 解析获取调用函数表达式的所有参数
             * @return vector<ast::Expression*>
             */
            vector<ast::Expression*> parseCallArguments();

        public:
            /**
             * 新建一个语法解析器
             * @param l Lexer*, 传入一个词法解析器的指针
             * @return 返回指向语法解析器的指针
             */
            static Parser* New(lexer::Lexer* l);

            /**
             * 解析 program
             * @return ast::Program 指向program节点的指针
             */
            ast::Program* ParseProgram();


            /**
             * 解析 statement
             * @return ast::Statement* 指向语句节点的指针
             */
            ast::Statement* parseStatement();

            /**
             * 获取 parser 的错误信息
             * 不可从外部修改
             * @return vector<string>a
             */
            __attribute__((unused)) vector<string> getErrors();
        };

    }


#endif //MONKEY_PARSER_H