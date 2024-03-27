//
// Created by 86158 on 2024/3/27.
//

#ifndef MONKEY_REPL_H
#define MONKEY_REPL_H

#include <string>
#include <iostream>

using std::string;
using std::istream;
using std::ostream;
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
            static void start(istream& in, ostream& out);

        };

    } // monkey
} // repl

#endif //MONKEY_REPL_H
