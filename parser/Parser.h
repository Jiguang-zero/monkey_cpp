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
            Parser(lexer::Lexer* _l);

            // 获取下一个词法单元 Token
            void nextToken();

        public:
            /**
             * 新建一个语法解析器
             * @param l Lexer*, 传入一个词法解析器的指针
             * @return 返回指向语法解析器的指针
             */
            Parser* New(lexer::Lexer* l);

            /**
             * 解析 program
             * @return 返回 指向 program 节点 (ast的根节点) 的指针
             */
            ast::Program* ParseProgram();

            /**
             * 解析 statement
             * @return 返回 statement 节点
             */
            ast::Statement parseStatement();
        };

    }
}

#endif //MONKEY_PARSER_H