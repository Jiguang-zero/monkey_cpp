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
        static object::Object* evalProgram(const std::vector<ast::Statement *> &stmts);


        /**
         * 解析语句块 {}
         * @param block  ast::BlockStatement*
         * @return object::Object*
         */
        static object::Object* evalBlockStatement(ast::BlockStatement* block);

        /**
         * 获取 布尔值对象 (要么为真，要么为假)
         * @param value bool
         * @return object::Boolean*
         */
        static object::Boolean* nativeBoolToBooleanObject(bool value);

        /**
         * 获取 前缀表达式 的值
         * @param op string operator
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalPrefixExpression(const string& op, object::Object* right);

        /**
         * 对 "!" 前缀表达式进行求值
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalBangOperatorExpression(object::Object* right);

        /**
         * 对 '-' 前缀表达式进行求值
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalMinusPrefixOperatorExpression(object::Object* right);

        /**
         * 对中缀表达式进行求值
         * @param op operator, string +, -, *, /, >, <, ==, !=
         * @param left object::Object*
         * @param right object::Object*
         * @return object::Object*
         */
        static object::Object* evalInfixExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * 对中缀表达式 整数类型 进行求值 left.Type() 和 right.Type() 理论上都得是 Integer_Obj
         * @param op
         * @param left
         * @param right
         * @return
         */
        static object::Object* evalInfixIntegerExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * 解析 if 表达式
         * @param expression ast::IfExpression*
         * @return object::Object *
         */
        static object::Object* evalIfExpression(ast::IfExpression * expression);

        /**
         * 判断 对象 是否是 真(TRUE)
         * @param object object::Object*
         * @return bool
         */
        static bool isTruthy(object::Object* object);

    public:

        /**
        * 对 节点 进行求值
        * @param node ast::Node * , 指向 node  的指针
        * @return Object* 返回求值对象
        */
        static object::Object* Eval(ast::Node* node);

    // 成员
    public:
        // 布尔值为真只需要一个对象就行了
        // 这个对象不应该的指针不应该被释放 ！！！
        static const object::Boolean TRUE;
        // 布尔值为假的对象同样只需要一个就行了
        static const object::Boolean FALSE;

        // 空值也应该值创建一个
        static const object::Null MY_NULL;
    };
}

#endif //MONKEY_EVALUATOR_H
