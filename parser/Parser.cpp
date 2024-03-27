#include "Parser.h"

namespace monkey {
    namespace parser {
        Parser::Parser(lexer::Lexer* _l) {
            l = _l;
        }

        Parser* Parser::New(lexer::Lexer* l) {
            Parser* p = new Parser(l);

            // 设置 curToken
            p->nextToken();
            p->nextToken();

            return p;
        }

        void Parser::nextToken() {
            curToken = peekToken;
            peekToken = l->NextToken();
        }

        ast::Program* Parser::ParseProgram() {
            ast::Program* program = new ast::Program();

            program->setStatements(vector<ast::Statement>());

            while (curToken.getType() != token::TOKEN_EOF) {
                auto stmt = parseStatement();
            }
        }

        ast::Statement Parser::parseStatement() {
            
        }
    }
}