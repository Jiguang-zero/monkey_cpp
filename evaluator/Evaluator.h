//
// Created by 86158 on 2024/4/3.
// ��ֵ
//

#ifndef MONKEY_EVALUATOR_H
#define MONKEY_EVALUATOR_H

#include "../object/environment.h"
#include "../ast/Program.h"


namespace monkey::evaluator {
    class Evaluator {
    private:
        /**
         * ����������ֵ
         * @param stmts std::vector<ast::Statements*>
         * @param env object::Environment*&
         * @return object::Object*
         */
        static object::Object *evalProgram(const std::vector<ast::Statement *> &stmts, object::Environment *& env);


        /**
         * �������� {}
         * @param block  ast::BlockStatement*
         * @param env object::Environment *&
         * @return object::Object*
         */
        static object::Object *evalBlockStatement(ast::BlockStatement *block, object::Environment *& env);

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
         * ����׺���ʽ string���� ������ֵ left.Type() �� right.Type() �����϶����� String_Obj
         * @param op operator
         * @param left
         * @param right
         * @return
         */
        static object::Object* evalInfixStringExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * ���� if ���ʽ
         * @param expression ast::IfExpression*
         * @param env object::Environment *&
         * @return object::Object *
         */
        static object::Object *evalIfExpression(ast::IfExpression *expression, object::Environment *& env);

        /**
         * �ж� ���� �Ƿ��� ��(TRUE)
         * @param object object::Object*
         * @return bool
         */
        static bool isTruthy(object::Object* object);

        /**
         * ������� error
         * @param error string
         * @return object::Error*
         */
        static object::Error* newError(const string& error);

        /**
         * �ж϶����Ƿ��Ǵ������
         * @param obj  object::Object*
         * @return  bool
         */
        static bool isError(object::Object* obj);

        /**
         * ���� ��ʶ��
         * @param node ast::Identifier*
         * @param env object::Environment*
         * @return object::Object*
         */
        static object::Object * evalIdentifier(ast::Identifier* node, object::Environment*& env);

        /**
         * �������ʽ�飬�����ض����� ( ���纯������ add(2 + 2, 3 + 34); 2 + 2, 3 + 34���Ǳ��ʽ
         * @param exps vector<ast::Expression*>
         * @param env object::Environment*
         * @return vector <object::Object*>
         */
        static vector<object::Object*> evalExpressions(
                const vector<ast::Expression*>& exps,
                object::Environment* env
                );

        /**
         * �� �������� ������ֵ����
         * @param fn object::Object*
         * @param args vector<object::Object*>
         * @return object::Object*
         */
        static object::Object* applyFunction(object::Object* fn, const vector<object::Object*>& args);

        /**
         * ����������Ļ����͵��ú����Ļ�������һ�� ������չ
         * @param fn
         * @param args
         * @return object::Environment*
         * @instruction �����������ʵ�ֻ�������չ�����⣺
         * ��������չ�ǽ��������õĻ�����չ����������Ļ�����
         * �����������ǿ���ʵ�ֺ����հ��ȹ��ܡ����� fn(x) {fn(y) {x + y};} ��Ҫ���ⲿxӦ�õ���������ڲ���
         */
        static object::Environment* extendedFunctionEnv(object::Function* fn, const vector<object::Object*>& args);

        /**
         * ��������� return ���ͣ��ͷ��� return ���� ��ֵ , ���򷵻ض�����
         * @param obj
         * @return
         */
        static object::Object* unwrapReturnValue(object::Object* obj);
    public:

        /**
        * �� �ڵ� ������ֵ
        * @param node ast::Node * , ָ�� node
        * @param env object::Environment * & ,��Ҫ�������ã������޸Ļ���
        * @return Object* ������ֵ����
        */
        static object::Object *Eval(ast::Node *node, object::Environment *& env);

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
