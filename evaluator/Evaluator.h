//
// Created by 86158 on 2024/4/3.
// ��ֵ
//

#ifndef MONKEY_EVALUATOR_H
#define MONKEY_EVALUATOR_H

#include "../object/object.h"
#include "../ast/program.h"


namespace monkey::evaluator {
    class Evaluator {
    private:
        /**
         * ����������ֵ
         * @param stmts std::vector<ast::Statements*>
         * @return object::Object*
         */
        static object::Object* evalStatements(const std::vector<ast::Statement*>& stmts);

    public:

        /**
        * �� �ڵ� ������ֵ
        * @param node ast::Node * , ָ�� node  ��ָ��
        * @return Object* ������ֵ����
        */
        static object::Object* Eval(ast::Node* node);
    };
}

#endif //MONKEY_EVALUATOR_H
