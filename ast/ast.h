//
// 定义 ast 的 父类(虚类)
// Created by 86158 on 2024/3/27.
//

#ifndef MONKEY_AST_H
#define MONKEY_AST_H

#include <string>
#include "sstream"

using std::string;


    namespace monkey::ast {
        
        /**
         * 通用Node ast节点
         */
        class Node {
            public:
                Node() = default;

            virtual /**
                 * 获取Node节点的词法单元字面量 
                 */
                string TokenLiteral() {return "";};

            virtual /**
                 * 获取Node节点的String
                 */
                string String() {return "";};
        };


        /**
         * 语句 ast
         */
        class Statement : public Node {
            public:
                Statement() = default;

            __attribute__((unused)) virtual /**
                 * 接口，没有实际功能
                 */
                void statementNode() {};
        };

        /**
         * 表达式 ast
         */
        class Expression : virtual public Node {
            public:
                Expression() = default;

            __attribute__((unused)) virtual /**
                 * 接口，没有实际功能
                 */
                void expressionNode() {};
        };

    } // ast
// monkey

#endif //MONKEY_AST_H
