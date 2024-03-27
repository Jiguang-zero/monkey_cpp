//
// Created by 86158 on 2024/3/27.
//

#include "Repl.h"
#include "../lexer/Lexer.h"
#include "../token/Token.h"

using monkey::lexer::Lexer;
using monkey::token::Token;

namespace monkey {
    namespace repl {
        void Repl::Start(istream &in, ostream &out) {
            while (true) {
                out << PROMPT;

                string line;
                std::getline(in, line);

                if (line == "exit") {
                    break;
                }

                Lexer *l = lexer::Lexer::New(line);

                for (Token tok = l->NextToken(); tok.getType() != token::TOKEN_EOF; tok = l->NextToken()) {
                    out << tok.String() << endl;
                }

            }
        }
    } // repl

    // 定义变量
    namespace repl {
        const string Repl::PROMPT = ">>";
    }
} // monkey