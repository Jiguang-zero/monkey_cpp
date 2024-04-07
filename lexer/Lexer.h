//
// Created by 86158 on 2024/3/26.
//

#ifndef MONKEY_LEXER_H
#define MONKEY_LEXER_H

#include "string"
#include "../token/Token.h"

using std::string;




        namespace monkey::lexer {

        /**
         * 词法解析器
         */
        class Lexer {
        protected:
            string input; // 词法解析器的输入
            int position; // 字符串的当前位置
            int readPosition; // 字符串的当前读取位置(当前字符的下一个字符)
            char ch; // 当前字符


        private:
            /**
             * 判断一个字符是否是字母(或者说是标识符的一部分)
             * @param ch
             * @return bool
             * TODO: 可以限制下划线的位置
             */
            static bool isLetter(char ch);

            /**
             * 判断字符是否是数字
             * @param ch
             * @return
             */
            static bool isDigit(char ch);

            /**
             * 获取标识符
             * @return 返回标识符字符串的字面量
             */
            string readIdentifier();

            /**
             * 跳过空白字符 ' ', '\t', '\n', '\r'
             */
            void skipWhiteSpace();

            /**
             * 获取数字
             * @return 返回数字的 string
             */
            string readNumber();

            /**
             * 读取字符
             * 只能用于读取非Unicode 字符
             * TODO: 增加Unicode字符的读取
             */
            void readChar();

            /**
             * 窥视下一个字符，不移动位置 position
             * @return 返回下一个字符
             */
            char peekChar();

            /**
             * 读取字符串
             * @return string
             */
            string readString();

        public:
            /**
             * 构造函数，传入字符串
             * @param _input
             */
            explicit Lexer(string _input);



            /**
             * 获取下一个词法单元
             * @return 返回词法单元
             */
            token::Token NextToken();

            /**
             * 新建一个词法解析器
             * @param input string, 字符串
             * @return Lexer*, 指向词法解析器的指针
             */
            static Lexer* New(string input);

            /**
             * 新建一个词法单元
             * @param tokenType 词法单元类型
             * @param ch 词法单元的字面量
             * @return Token 对象
             */
            static token::Token newToken(token::TokenType tokenType, char ch);

        };


    } // lexer
// monkey

#endif //MONKEY_LEXER_H
