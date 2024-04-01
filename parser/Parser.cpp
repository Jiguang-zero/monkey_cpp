#include "Parser.h"
#include <iostream>
#include <functional>

namespace monkey {
    namespace parser {
        Parser::Parser(lexer::Lexer* _l) {
            l = _l;
        }

        Parser* Parser::New(lexer::Lexer* l) {
            auto* p = new Parser(l);

            p->registerPrefix(token::IDENT, &Parser::parseIdentifier);


            // 设置 curToken
            p->nextToken();
            p->nextToken();

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



//        ast::Expression* Parser::parseIdentifier(parser::Parser& p) {
//            return new ast::Identifier(
//                    p.curToken,
//                    p.curToken.getLiteral()
//                    );
//        }



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
                return nullptr;
            }
            auto* leftExp = new ast::Expression();
            leftExp = (this->*prefix)();

            return leftExp;
        }

        ast::Expression* Parser::parseIdentifier() {
            auto * expression = new ast::Expression();
            expression = new ast::Identifier(curToken, curToken.getLiteral());
            return expression;
        }


    }
}