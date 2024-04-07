//
// Created by 86158 on 2024/3/26.
//

#include "Lexer.h"

#include <utility>

#include <iostream>

using std::cout;
using std::endl;


    namespace monkey::lexer {
        void Lexer::readChar() {
            if (readPosition >= input.length()) {
                ch = 0;
            } else {
                ch = input[readPosition];
            }

            position = readPosition;
            readPosition += 1;
        }

        Lexer *Lexer::New(string input) {
            auto* l = new Lexer(std::move(input));
            l->readChar();

            return l;
        }

        Lexer::Lexer(string _input) {
            input = std::move(_input);
            readPosition = 0;
            position = 0;
            ch = 0;
        }

        token::Token Lexer::newToken(token::TokenType tokenType, char ch) {
            return {std::move(tokenType), std::string(1, ch)};
        }

        token::Token Lexer::NextToken() {
            token::Token tok;

            skipWhiteSpace();

            switch (ch) {
                case '=':
                    if (peekChar() == '=') {
                        char c = ch;
                        readChar();
                        tok.setLiteral(std::string(1, c) + ch);
                        tok.setType(token::EQ);
                    }
                    else {
                        tok = newToken(token::ASSIGN, (char)ch);
                    }
                    break;
                case '+':
                    tok = newToken(token::PLUS, (char)ch);
                    break;
                case '-':
                    tok = newToken(token::MINUS, (char)ch);
                    break;
                case '*':
                    tok = newToken(token::ASTERISK, (char)ch);
                    break;
                case '/':
                    tok = newToken(token::SLASH, (char)ch);
                    break;

                case '!':
                    if (peekChar() == '=') {
                        char c = ch;
                        readChar();
                        tok.setLiteral(std::string(1, c) + ch);
                        tok.setType(token::NOT_EQ);
                    }
                    else {
                        tok = newToken(token::BANG, (char)ch);
                    }
                    break;
                case '<':
                    tok = newToken(token::LT, (char)ch);
                    break;
                case '>':
                    tok = newToken(token::GT, (char)ch);
                    break;


                case ',':
                    tok = newToken(token::COMMA, (char)ch);
                    break;
                case ';':
                    tok = newToken(token::SEMICOLON, (char)ch);
                    break;
                    
                case '(':
                    tok = newToken(token::LPAREN, (char)ch);
                    break;
                case ')':
                    tok = newToken(token::RPAREN, (char)ch);
                    break;
                case '{':
                    tok = newToken(token::LBRACE, (char)ch);
                    break;
                case '}':
                    tok = newToken(token::RBRACE, (char)ch);
                    break;

                case '"':
                    tok.setType(token::STRING);
                    tok.setLiteral(readString());
                    break;

                case 0:
                    tok.setType(token::TOKEN_EOF);
                    tok.setLiteral("");
                    break;

                default:
                    if (isLetter(ch)) {
                        tok.setLiteral(readIdentifier());
                        tok.setType(token::LookupIdent(tok.getLiteral()));
                        return tok;
                    }
                    else if (isDigit(ch)) {
                        tok.setType(token::INTEGER);
                        tok.setLiteral(readNumber());
                        return tok;
                    }
                    else {
                        tok = newToken(token::ILLEGAL, (char)ch);
                    }

            }
            readChar();

            return tok;
        }

        bool Lexer::isLetter(char ch) {
            return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
        }

        bool Lexer::isDigit(char ch) {
            return ch >= '0' && ch <= '9';
        }

        string Lexer::readIdentifier() {
            int p = position;
            while (isLetter(ch)) {
                readChar();
            }
            // 获取读取的字符串
            return input.substr(p, position - p);
        }

        void Lexer::skipWhiteSpace() {
            while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                readChar();
            }
        }

        string Lexer::readNumber() {
            int p = position;

            while (isDigit(ch)) {
                readChar();
            }

            return input.substr(p, position - p);
        }

        char Lexer::peekChar() {
            if (readPosition >= input.length()) {
                return 0;
            }
            else {
                return input[readPosition];
            }
        }

        string Lexer::readString() {
            //TODO: 转义字符的支持，报错处理
            int p = position + 1;
            while (true) {
                readChar();
                if (ch == '"' || ch == 0) {
                    break;
                }
            }
            return input.substr(p, position - p);
        }
    } // monkey
// lexer