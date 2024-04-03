//
// Created by 86158 on 2024/3/27.
//

#include "Repl.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "../evaluator/Evaluator.h"

using monkey::lexer::Lexer;
using monkey::token::Token;
using monkey::parser::Parser;

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
                Parser *p = parser::Parser::New(l);

                auto * program = p->ParseProgram();
                if (!p->getErrors().empty()) {
                    printParserErrors(out, p->getErrors());
                    // 错误后不必求值
                    continue;
                }

                auto * evaluated = evaluator::Evaluator::Eval(program);
                if (evaluated) {
                    out << evaluated->Inspect();
                    out << endl;
                }

                if (evaluated != &evaluator::Evaluator::MY_NULL &&
                    evaluated != &evaluator::Evaluator::TRUE &&
                    evaluated != &evaluator::Evaluator::FALSE ) {
                    // 如果是布尔值 或者是空值，不应该删除。。。
                    delete evaluated;
                }

                // 释放空间
                delete program;
                delete p;
                delete l;
            }
        }

        void Repl::printParserErrors(ostream &out, const vector<string>& errors) {
            for (const auto & error : errors) {
                out << "\t" << error << "\n";
            }
        }
    } // repl

    // 定义变量
    namespace repl {
        const string Repl::PROMPT = ">>";

    }
} // monkey