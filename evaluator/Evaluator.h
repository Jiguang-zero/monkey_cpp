//
// Created by 86158 on 2024/4/3.
// 求值
//

#ifndef MONKEY_EVALUATOR_H
#define MONKEY_EVALUATOR_H

#include "../object/object.h"
#include "../ast/program.h"


namespace monkey::evaluator {
    class Evaluator {
    private:
        /**
         * 对语句进行求值
         * @param stmts std::vector<ast::Statements*>
         * @return object::Object*
         */
        static object::Object* evalStatements(const std::vector<ast::Statement*>& stmts);

    public:

        /**
        * 对 节点 进行求值
        * @param node ast::Node * , 指向 node  的指针
        * @return Object* 返回求值对象
        */
        static object::Object* Eval(ast::Node* node);
    };
}

#endif //MONKEY_EVALUATOR_H
