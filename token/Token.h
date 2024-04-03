//
// Created by 86158 on 2024/3/26.
//

#ifndef MONKEY_TOKEN_H
#define MONKEY_TOKEN_H

#include "string"
#include <map>

using std::string;
using std::map;



    namespace monkey::token {
        typedef string TokenType;

        /**
         * 词法单元
         */
        class Token {
        protected:
            TokenType Type; // 词法单元的类型
            string Literal; // 词法单元的字面量


        public:
            /**
             * 空白构造函数，不初始化任意参数
             */
             Token() = default;

            /**
             * 构造函数
             * @param tokenType 词法单元的类型
             * @param literal 词法单元的字面量 string
             */
            Token(TokenType tokenType, string literal);

            /**
             * 设置类型 TokenType
             * @param type
             */
            void setType(TokenType type);

            /**
             * 设置字面量
             * @param literal string
             */
            void setLiteral(string literal);

            // 获取类型的值
            TokenType getType();

            // 获取字面量的值
            string getLiteral();

            /**
             * 将 token 转化为 string
             * @return string
             */
            string String();
        };

        extern const TokenType ILLEGAL; // 非法单元

        extern const TokenType TOKEN_EOF; // 结束单元

        // 数据类型
        extern const TokenType STRING; // 字符串单元
        extern const TokenType INTEGER; // 整数单元
        //TODO: 增加浮点数类型

        // 运算符号
        extern const TokenType ASSIGN; // 赋值符号单元 =
        extern const TokenType PLUS; // 加号 +
        extern const TokenType MINUS; // 减号 -
        extern const TokenType ASTERISK; // 星号 *
        extern const TokenType SLASH; // 斜线 /(除法)

        // 条件运算符号
        extern const TokenType BANG; // 感叹号 !
        extern const TokenType LT; // 小于号 < less than
        extern const TokenType GT; // 大于号 > greater than
        extern const TokenType EQ; // 相等 == equal
        extern const TokenType NOT_EQ; // 不相等 != equal

        // 分隔符
        extern const TokenType COMMA; // 逗号
        extern const TokenType SEMICOLON; // 分号

        // 标识符
        extern const TokenType IDENT; // 参数名等

        extern const TokenType LPAREN; // 左括号 (
        extern const TokenType RPAREN; // 右括号 )
        extern const TokenType LBRACE; // 左大括号 {
        extern const TokenType RBRACE; // 右大括号

        // 关键字
        extern const TokenType FUNCTION; // 函数关键字 fn
        extern const TokenType LET; // 关键字 let
        extern const TokenType TOKEN_TRUE; // 关键字 true
        extern const TokenType TOKEN_FALSE; // 关键字 false
        extern const TokenType IF; // 关键字 if
        extern const TokenType ELSE; // 关键字 else
        extern const TokenType RETURN; // 关键字 return

        extern map<string, TokenType > keywords;

        /**
         * 从关键字 map 中查找 ident 是否是关键字
         * @param ident const string&
         * @return 返回类型 TokenType
         */
        TokenType LookupIdent(const string& ident);

    } // token

// monkey

#endif //MONKEY_TOKEN_H
