//
// Created by 86158 on 2024/3/26.
//

#include "Token.h"

#include <utility>

namespace monkey {
    namespace token {

        Token::Token(TokenType tokenType, string literal) {
            Type = std::move(tokenType);
            Literal = std::move(literal);
        }

        void Token::setType(TokenType type) {
            Type = std::move(type);
        }

        void Token::setLiteral(std::string literal) {
            Literal = std::move(literal);
        }

        TokenType Token::getType() {
            return Type;
        }

        string Token::getLiteral() {
            return Literal;
        }


    } // token

    namespace token {
        TokenType ILLEGAL = "ILLEGAL";
        TokenType TOKEN_EOF = "EOF";

        TokenType STRING = "STRING";
        TokenType INTEGER = "INT";

        TokenType ASSIGN = "=";
        TokenType PLUS = "+";
        TokenType MINUS = "-";
        TokenType ASTERISK = "*";
        TokenType SLASH = "/";

        TokenType BANG = "!";
        TokenType LT = "<";
        TokenType GT = ">";
        TokenType EQ = "==";
        TokenType NOT_EQ = "!=";

        TokenType COMMA = ",";
        TokenType SEMICOLON = ";";

        TokenType IDENT = "IDENT";

        TokenType LPAREN = "(";
        TokenType RPAREN = ")";
        TokenType LBRACE = "{";
        TokenType RBRACE = "}";

        TokenType FUNCTION = "FUNCTION";
        TokenType LET = "LET";
        TokenType TRUE = "TRUE";
        TokenType FALSE = "FALSE";
        TokenType RETURN = "RETURN";
        TokenType IF = "IF";
        TokenType ELSE = "ELSE";


        map<string, TokenType > keywords = {
                {"fn", FUNCTION},
                {"let", LET},
                {"true", TRUE},
                {"false", FALSE},
                {"if", IF},
                {"else", ELSE},
                {"return", RETURN}
        };

        TokenType LookupIdent(const string& ident) {
            if(keywords.count(ident)) {
                return keywords[ident];
            }
            return IDENT;
        }

    }
} // monkey