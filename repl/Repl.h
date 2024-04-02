//
// Created by 86158 on 2024/3/27.
//

#ifndef MONKEY_REPL_H
#define MONKEY_REPL_H

#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::istream;
using std::ostream;
using std::vector;
using std::endl;

namespace monkey {
    namespace repl {

        class Repl {
        public:
            static const string PROMPT;

        public:
            /**
             * 程序的入口 REPL
             * read, equal, print and loop
             * @param in istream&
             * @param out ostream&
             */
            static void Start(istream& in, ostream& out);

        private:
            /**
             * 打印错误信息
             * @param out ostream&
             * @param errors std::vector<string>
             */
            static void printParserErrors(ostream& out, const std::vector<string>& errors);

        };

    } // monkey
} // repl

#endif //MONKEY_REPL_H
