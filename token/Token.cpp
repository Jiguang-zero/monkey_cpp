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

        string Token::String() {
            return "Type: " + getType() + " Literal: " + getLiteral();
        }


    } // token

    namespace token {
        const TokenType ILLEGAL = "ILLEGAL";
        const TokenType TOKEN_EOF = "EOF";

        const TokenType STRING = "STRING";
        const TokenType INTEGER = "INT";

        const TokenType ASSIGN = "=";
        const TokenType PLUS = "+";
        const TokenType MINUS = "-";
        const TokenType ASTERISK = "*";
        const TokenType SLASH = "/";

        const TokenType BANG = "!";
        const TokenType LT = "<";
        const TokenType GT = ">";
        const TokenType EQ = "==";
        const TokenType NOT_EQ = "!=";

        const TokenType COMMA = ",";
        const TokenType SEMICOLON = ";";

        const TokenType IDENT = "IDENT";

        const TokenType LPAREN = "(";
        const TokenType RPAREN = ")";
        const TokenType LBRACE = "{";
        const TokenType RBRACE = "}";
        const TokenType LBRACKET = "[";
        const TokenType RBRACKET = "]";

        const TokenType FUNCTION = "FUNCTION";
        const TokenType LET = "LET";
        const TokenType TOKEN_TRUE = "TOKEN_TRUE";
        const TokenType TOKEN_FALSE = "TOKEN_FALSE";
        const TokenType RETURN = "RETURN";
        const TokenType IF = "IF";
        const TokenType ELSE = "ELSE";


        map<string, TokenType > keywords = {
                {"fn",     FUNCTION},
                {"let",    LET},
                {"true",   TOKEN_TRUE},
                {"false",  TOKEN_FALSE},
                {"if",     IF},
                {"else",   ELSE},
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