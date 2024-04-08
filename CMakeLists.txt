cmake_minimum_required(VERSION 3.26)
project(monkey)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(monkey main.cpp
        token/Token.cpp
        lexer/Lexer.cpp
        repl/Repl.cpp
        ast/Program.cpp
        ast/Statement.cpp
        parser/Parser.cpp
        ast/Expression.cpp
        object/object.cpp
        evaluator/Evaluator.cpp
        object/environment.cpp
        evaluator/builtin.cpp
)

