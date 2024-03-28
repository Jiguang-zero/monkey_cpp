#include "Parser.h"

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

            return p;
        }

        void Parser::nextToken() {
            curToken = peekToken;
            peekToken = l->NextToken();
        }

        void Parser::ParseProgram(ast::Program** program) {
            *program = new ast::Program();

            (*program)->setStatements(vector<ast::Statement>());

            while (curToken.getType() != token::TOKEN_EOF) {
                ast::Statement *stmt = nullptr;
                parseStatement(&stmt);
                if (stmt != nullptr) {
                    (*program)->getStatements().emplace_back(*stmt);
                }
                nextToken();
            }
        }

        void Parser::parseStatement(ast::Statement** statement) {
            token::TokenType type = curToken.getType();

            if (type == token::LET) {
                parseLetStatement(reinterpret_cast<ast::LetStatement **>(statement));
            }
        }

        void Parser::parseLetStatement(ast::LetStatement ** stmt) {
            *stmt = new ast::LetStatement(curToken);

            if (!expectPeek(token::IDENT)) {
                *stmt = nullptr;
                return;
            }

            (*stmt)->setName(new ast::Identifier(curToken, curToken.getLiteral()));

            if (!expectPeek(token::ASSIGN)) {
                *stmt = nullptr;
                return;
            }


            while (!curTokenIs(token::SEMICOLON)) {
                nextToken();
            }
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
            return false;
        }
    }
}