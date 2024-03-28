#ifndef MONKEY_PARSER_H
#define MONKEY_PARSER_H

#include "../token/Token.h"
#include "../lexer/Lexer.h"
#include "../ast/Program.h"

namespace monkey {
    namespace parser {

        // 语法解析器
        class Parser {
        protected:
            lexer::Lexer* l; // 词法解析器

            token::Token curToken; // 当前词法单元
            token::Token peekToken; // 下一个词法单元

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
        };

    }
}

#endif //MONKEY_PARSER_H