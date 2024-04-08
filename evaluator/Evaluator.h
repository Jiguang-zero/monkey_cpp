//
// Created by 86158 on 2024/4/3.
// 求值
//

#ifndef MONKEY_EVALUATOR_H
#define MONKEY_EVALUATOR_H

#include "../object/environment.h"
#include "../ast/Program.h"


namespace monkey::evaluator {
    class Evaluator {
    private:
        /**
         * 对语句进行求值
         * @param stmts std::vector<ast::Statements*>
         * @param env object::Environment*&
         * @return object::Object*
         */
        static object::Object *evalProgram(const std::vector<ast::Statement *> &stmts, object::Environment *& env);


        /**
         * 解析语句块 {}
         * @param block  ast::BlockStatement*
         * @param env object::Environment *&
         * @return object::Object*
         */
        static object::Object *evalBlockStatement(ast::BlockStatement *block, object::Environment *& env);

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
         * 对中缀表达式 string类型 进行求值 left.Type() 和 right.Type() 理论上都得是 String_Obj
         * @param op operator
         * @param left
         * @param right
         * @return
         */
        static object::Object* evalInfixStringExpression(const string& op, object::Object* left, object::Object* right);

        /**
         * 解析 if 表达式
         * @param expression ast::IfExpression*
         * @param env object::Environment *&
         * @return object::Object *
         */
        static object::Object *evalIfExpression(ast::IfExpression *expression, object::Environment *& env);

        /**
         * 判断 对象 是否是 真(TRUE)
         * @param object object::Object*
         * @return bool
         */
        static bool isTruthy(object::Object* object);

        /**
         * 传入参数 error
         * @param error string
         * @return object::Error*
         */
        static object::Error* newError(const string& error);

        /**
         * 判断对象是否是错误对象
         * @param obj  object::Object*
         * @return  bool
         */
        static bool isError(object::Object* obj);

        /**
         * 解析 标识符
         * @param node ast::Identifier*
         * @param env object::Environment*
         * @return object::Object*
         */
        static object::Object * evalIdentifier(ast::Identifier* node, object::Environment*& env);

        /**
         * 解析表达式组，并返回对象组 ( 比如函数调用 add(2 + 2, 3 + 34); 2 + 2, 3 + 34都是表达式
         * @param exps vector<ast::Expression*>
         * @param env object::Environment*
         * @return vector <object::Object*>
         */
        static vector<object::Object*> evalExpressions(
                const vector<ast::Expression*>& exps,
                object::Environment* env
                );

        /**
         * 对 函数调用 进行求值解析
         * @param fn object::Object*
         * @param args vector<object::Object*>
         * @return object::Object*
         */
        static object::Object* applyFunction(object::Object* fn, const vector<object::Object*>& args);

        /**
         * 将函数定义的环境和调用函数的环境绑定在一块 进行拓展
         * @param fn
         * @param args
         * @return object::Environment*
         * @instruction 关于这个函数实现环境的拓展的问题：
         * 环境的拓展是将函数调用的环境拓展到环境定义的环境中
         * 这样的作用是可以实现函数闭包等功能。比如 fn(x) {fn(y) {x + y};} 需要将外部x应用到这个函数内部中
         */
        static object::Environment* extendedFunctionEnv(object::Function* fn, const vector<object::Object*>& args);

        /**
         * 如果对象是 return 类型，就返回 return 对象 的值 , 否则返回对象本身
         * @param obj
         * @return
         */
        static object::Object* unwrapReturnValue(object::Object* obj);
    public:

        /**
        * 对 节点 进行求值
        * @param node ast::Node * , 指向 node
        * @param env object::Environment * & ,需要传入引用，进行修改环境
        * @return Object* 返回求值对象
        */
        static object::Object *Eval(ast::Node *node, object::Environment *& env);

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
