//
// Created by 86158 on 2024/3/26.
//

#include "Lexer.cpp"
#include "../token/Token.cpp"
#include <iostream>

#include <vector>

using std::vector;

using namespace monkey;

using std::cout;
using std::endl;

void testNextToken() {
    string input =
            "+={}(),;\n"
            "let x = 5;\n"
            "let y = 10;\n"
            "!><-*/\n"
            "let result = fn add(five, ten);\n"
            "true false\n"
            "return if else\t"
            "== !=";

    vector<string> literals = {
            "+",
            "=",
            "{",
            "}",
            "(",
            ")",
            ",",
            ";",
            "let",
            "x",
            "=",
            "5",
            ";",
            "let",
            "y",
            "=",
            "10",
            ";",
            "!",
            ">",
            "<",
            "-",
            "*",
            "/",
            "let",
            "result",
            "=",
            "fn",
            "add",
            "(",
            "five",
            ",",
            "ten",
            ")",
            ";",
            "true",
            "false",
            "return",
            "if",
            "else",
            "==",
            "!=",

            ""
    };

    vector<token::TokenType> types = {
            token::PLUS,
            token::ASSIGN,
            token::LBRACE,
            token::RBRACE,
            token::LPAREN,
            token::RPAREN,
            token::COMMA,
            token::SEMICOLON,
            token::LET,
            token::IDENT,
            token::ASSIGN,
            token::INTEGER,
            token::SEMICOLON,
            token::LET,
            token::IDENT,
            token::ASSIGN,
            token::INTEGER,
            token::SEMICOLON,
            token::BANG,
            token::GT,
            token::LT,
            token::MINUS,
            token::ASTERISK,
            token::SLASH,
            token::LET,
            token::IDENT,
            token::ASSIGN,
            token::FUNCTION,
            token::IDENT,
            token::LPAREN,
            token::IDENT,
            token::COMMA,
            token::IDENT,
            token::RPAREN,
            token::SEMICOLON,
            token::TRUE,
            token::FALSE,
            token::RETURN,
            token::IF,
            token::ELSE,
            token::EQ,
            token::NOT_EQ,

            token::TOKEN_EOF
    };

    lexer::Lexer* l = lexer::Lexer::New(input);

    bool flag(true);

    cout << "Test testNextToken() Start: " << endl;
    if (types.size() != literals.size()) {
        cout << "The size of literals is not the same as types.\n";
        flag = false;
    }
    else {
        for (int i = 0; i < types.size(); i ++ ) {
            auto tok = l->NextToken();

            if (tok.getLiteral() != literals[i]) {
                cout << i << ": " << tok.getLiteral() << "Literal: wrong, it should be " << literals[i] << endl;
                flag = false;
            }
            if (tok.getType() != types[i]) {
                cout << i << ": " << tok.getType() << "Type: wrong, it should be" << types[i] << endl;
                flag = false;
            }
        }
    }

    cout << "Test testNextToken() end: " << (flag ? "PASS" : "FAIL") << endl;
}