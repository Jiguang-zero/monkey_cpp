#include "Parser.h"
#include <iostream>
#include <cstdlib>

namespace monkey {
    namespace parser {
        Parser::Parser(lexer::Lexer* _l) {
            l = _l;
        }

        Parser* Parser::New(lexer::Lexer* l) {
            auto* p = new Parser(l);

            // 设置 curToken
            p->nextToken();
            p->nextToken();


            // 注册前缀函数
            p->registerPrefix(token::IDENT, &Parser::parseIdentifier);
            p->registerPrefix(token::INTEGER, &Parser::parseIntegerLiteral);
            p->registerPrefix(token::BANG, &Parser::parsePrefixExpression);
            p->registerPrefix(token::MINUS, &Parser::parsePrefixExpression);

            // 注册中缀函数
            p->registerInfix(token::PLUS, &Parser::parseInfixExpression);
            p->registerInfix(token::MINUS, &Parser::parseInfixExpression);
            p->registerInfix(token::SLASH, &Parser::parseInfixExpression);
            p->registerInfix(token::ASTERISK, &Parser::parseInfixExpression);
            p->registerInfix(token::EQ, &Parser::parseInfixExpression);
            p->registerInfix(token::NOT_EQ, &Parser::parseInfixExpression);
            p->registerInfix(token::LT, &Parser::parseInfixExpression);
            p->registerInfix(token::GT, &Parser::parseInfixExpression);

            return p;
        }

        void Parser::nextToken() {
            curToken = peekToken;
            peekToken = l->NextToken();
        }

        bool Parser::curTokenIs(const token::TokenType &t) {
            return curToken.getType() == t;
        }

        bool Parser::peekTokenIs(const token::TokenType &t) {
            return peekToken.getType() == t;
        }

        bool Parser::expectPeek(const token::TokenType &t) {
            if (peekTokenIs(t)) {
                nextToken();
                return true;
            }
            else {
                peekError(t);
                //TODO 错误输出测试
                return false;
            }
        }

        __attribute__((unused)) vector<string> Parser::getErrors() {
            return errors;
        }

        void Parser::peekError(const token::TokenType &t) {
            string msg = "expected next token to be " + t + ", got "
                    + peekToken.getType() + " instead";
            errors.emplace_back(msg);
        }

        void Parser::noPrefixParseFnError(const token::TokenType& t) {
            auto msg = "no prefix parse function for " + t + " found.";
            errors.emplace_back(msg);
        }

        ast::Program *Parser::ParseProgram() {
            auto * program = new ast::Program();

            (program)->setStatements(vector<ast::Statement*>());

            while (curToken.getType() != token::TOKEN_EOF) {
                auto *stmt = new ast::Statement();
                stmt = parseStatement();

                if (stmt != nullptr) {
                    (*program).getStatements().emplace_back(stmt);

                }
                nextToken();
            }

            return program;
        }

        ast::Statement *Parser::parseStatement() {
            token::TokenType type = curToken.getType();
            auto * statement = new ast::Statement();

            if (type == token::LET) {
                statement = parseLetStatement();
            }
            else if (type == token::RETURN) {
                statement = parseReturnStatement();
            }

            else {
                statement = parseExpressionStatement();
            }

            return statement;
        }

        ast::LetStatement *Parser::parseLetStatement() {
            auto* stmt = new ast::LetStatement(curToken);

            if (!expectPeek(token::IDENT)) {
                return nullptr;
            }

            (*stmt).setName(new ast::Identifier(curToken, curToken.getLiteral()));

            if (!expectPeek(token::ASSIGN)) {
                return nullptr;
            }


            while (!curTokenIs(token::SEMICOLON)) {
                nextToken();
            }

            return stmt;
        }

        ast::ReturnStatement *Parser::parseReturnStatement() {
            auto* stmt = new ast::ReturnStatement(curToken);

            nextToken();

            while (!curTokenIs(token::SEMICOLON)) {
                nextToken();
            }

            return stmt;
        }

        ast::ExpressionStatement *Parser::parseExpressionStatement() {
            auto* stmt = new ast::ExpressionStatement(curToken);

            auto expression = new ast::Expression();
            expression = parseExpression(PRECEDENCE::LOWEST);

            (*stmt).setExpression(expression);


            if (peekTokenIs(token::SEMICOLON)) {
                nextToken();
            }
            return stmt;
        }

        ast::Expression *Parser::parseExpression(const PRECEDENCE &precedence) {
            auto prefix = prefixParseFns[curToken.getType()];
            if (prefix == nullptr) {
                noPrefixParseFnError(curToken.getType());
                return nullptr;
            }

            auto* leftExp = new ast::Expression();
            leftExp = (this->*prefix)();

            while (!peekTokenIs(token::SEMICOLON) && precedence < peekPrecedence()) {
                auto infix = infixParseFns[peekToken.getType()];
                if (infix == nullptr) {
                    return leftExp;
                }

                nextToken();

                leftExp = (this->*infix)(leftExp);
            }

            return leftExp;
        }

        ast::Expression* Parser::parseIdentifier() {
            auto * expression = new ast::Expression();
            expression = new ast::Identifier(curToken, curToken.getLiteral());
            return expression;
        }

        ast::Expression *Parser::parseIntegerLiteral() {
            auto * expression = new ast::Expression();

            auto * integerLiteral = new ast::IntegerLiteral(curToken);

            char* endPtr;
            long long value = strtoll(curToken.getLiteral().c_str(), &endPtr, 10);
            if (endPtr == curToken.getLiteral().c_str()) {
                errors.emplace_back("could not parse " + curToken.getLiteral() + " as a number(64)");
            }

            integerLiteral->setValue(value);

            expression = integerLiteral;

            return expression;
        }


        ast::Expression *Parser::parsePrefixExpression() {
            auto * expression = new ast::Expression();

            auto * prefixExpression = new ast::PrefixExpression(curToken, curToken.getLiteral());

            nextToken();

            auto * right = new ast::Expression();
            right = parseExpression(PRECEDENCE::PREFIX);
            prefixExpression->setRightExpression(right);

            expression = prefixExpression;

            return expression;
        }

        PRECEDENCE Parser::peekPrecedence() {
            return getPrecedenceWithTokenType(peekToken.getType());
        }

        PRECEDENCE Parser::curPrecedence() {
            return getPrecedenceWithTokenType(curToken.getType());
        }

        ast::Expression * Parser::parseInfixExpression(ast::Expression* left) {
            auto * expression = new ast::Expression();

            auto * infixExpression = new ast::InfixExpression(
                    curToken,
                    left,
                    curToken.getLiteral()
                );

            auto precedence = curPrecedence();
            nextToken();

            auto * right = new ast::Expression();
            right = parseExpression(precedence);

            infixExpression->setRightExpression(right);

            expression = infixExpression;

            return expression;
        }


        PRECEDENCE Parser::getPrecedenceWithTokenType(const token::TokenType& type) {
            if (type == token::EQ || type == token::NOT_EQ) {
                return EQUALS;
            }
            if (type == token::LT || type == token::GT) {
                return LESS_GREATER;
            }
            if (type == token::PLUS || type == token::MINUS) {
                return SUM;
            }
            if (type == token::SLASH || type == token::ASTERISK) {
                return PRODUCT;
            }
            return LOWEST;
        }
    }

}