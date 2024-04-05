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
        static object::Object* evalProgram(const std::vector<ast::Statement *> &stmts);


        /**
         * �������� {}
         * @param block  ast::BlockStatement*
         * @return object::Object*
         */
        static object::Object* evalBlockStatement(ast::BlockStatement* block);

        /**
         * ��ȡ ����ֵ���� (ҪôΪ�棬ҪôΪ��)
         * @param value bool
         * @return object::Boolean*
         */
        static object::Boolean* nativeBoolToBooleanObject(bool value);

        /**
         * ��ȡ ǰ׺���ʽ ��ֵ
         * @param op string operator
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalPrefixExpression(const string& op, object::Object* right);

        /**
         * �� "!" ǰ׺���ʽ������ֵ
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalBangOperatorExpression(object::Object* right);

        /**
         * �� '-' ǰ׺���ʽ������ֵ
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalMinusPrefixOperatorExpression(object::Object* right);

        /**
         * ����׺���ʽ������ֵ
         * @param op operator, string +, -, *, /, >, <, ==, !=
         * @param left object::Object*
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalInfixExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * ����׺���ʽ �������� ������ֵ left.Type() �� right.Type() �����϶����� Integer_Obj
         * @param op
         * @param left
         * @param right
         * @return
         */
        static object::Object* evalInfixIntegerExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * ���� if ���ʽ
         * @param expression ast::IfExpression*
         * @return object::Object *
         */
        static object::Object* evalIfExpression(ast::IfExpression * expression);

        /**
         * �ж� ���� �Ƿ��� ��(TRUE)
         * @param object object::Object*
         * @return bool
         */
        static bool isTruthy(object::Object* object);

    public:

        /**
        * �� �ڵ� ������ֵ
        * @param node ast::Node * , ָ�� node  ��ָ��
        * @return Object* ������ֵ����
        */
        static object::Object* Eval(ast::Node* node);

    // ��Ա
    public:
        // ����ֵΪ��ֻ��Ҫһ�����������
        // �������Ӧ�õ�ָ�벻Ӧ�ñ��ͷ� ������
        static const object::Boolean TRUE;
        // ����ֵΪ�ٵĶ���ͬ��ֻ��Ҫһ��������
        static const object::Boolean FALSE;

        // ��ֵҲӦ��ֵ����һ��
        static const object::Null MY_NULL;
    };
}

#endif //MONKEY_EVALUATOR_H
