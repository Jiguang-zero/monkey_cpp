//
// Created by 86158 on 2024/4/3.
//

#ifndef MONKEY_OBJECT_H
#define MONKEY_OBJECT_H

#include <string>
#include <utility>
#include <vector>
#include <numeric>

#include "../ast/Program.h"

using std::string;
using std::vector;


// 定义变量
namespace monkey::object {
    class Environment; // 声明

    typedef string ObjectType;


    // 整数类型 Integer
    extern const ObjectType INTEGER_OBJ;
    // 布尔值
    extern const ObjectType BOOLEAN_OBJ;
    // 空值
    extern const ObjectType NULL_OBJ;
    // return 值
    extern const ObjectType RETURN_VALUE_OBJ;
    // 错误
    extern const ObjectType ERROR_OBJ;

    // 函数
    extern const ObjectType FUNCTION_OBJ;
}


namespace monkey::object {
    /**
     * 虚类 Object
     */
    class Object {
    public:
        Object() = default;

        // Object 的类型
        virtual ObjectType Type() {return "";};

        // Object 视察(值的 string 显示)
        virtual string Inspect() {return "";};
    };


    class Integer : virtual public Object {
    private:
        long long Value = 0;

    public:
        /**
         * 构造函数
         * @param value 从外部获取 value
         */
        explicit Integer(long long value) : Value(value) {};

        [[maybe_unused]] [[maybe_unused]] void setValue(long long value);

        [[maybe_unused]] [[nodiscard]] long long getValue() const;

        string Inspect() override;
        ObjectType Type() override;

    };

    class Boolean : virtual public Object {
    private:
        // 默认是 true
        bool Value = true;

    public:
        // 构造函数
        explicit Boolean(bool value) : Value(value) {};

        // 从外部设置 Boolean 的 Value
        [[maybe_unused]] [[maybe_unused]] void setValue(bool value);

        // 从外部获取 Value, 无法从外部修改
        [[maybe_unused]] [[nodiscard]] bool getValue() const;

        string Inspect() override;
        ObjectType Type() override;

    };

    class Null : virtual public Object {
    public:
        ObjectType Type() override;

        string Inspect() override;

    };


    class ReturnValue : virtual public Object {
    private:
        Object* Value; // 返回的对象

    public:
        /**
         * 构造函数， 传入 value 对象
         * @param value
         */
        explicit ReturnValue(Object* value) : Value(value) {};

        ObjectType Type() override;

        string Inspect() override;

        /**
         * 从外部获取值
         * @return 返回引用: 可以从外部修改
         */
        [[maybe_unused]] object::Object*& getValue();

        /**
         * 从外部设置值
         * @param value object::Object* & 引用
         */
        [[maybe_unused]] void setValue(object::Object* & value);

    };

    /**
     * 错误对象
     */
    class Error : virtual public Object {
    private:
        string Message;

    public:
        /**
         * 构造函数，错误对象的构造只能传入 string 值
         * @param message
         */
        explicit Error(string message) : Message(std::move(message)) {}

        [[maybe_unused]] [[nodiscard]] string getMessage() const;

        ObjectType Type() override;

        string Inspect() override;

    };

    /**
     * 函数对象
     */
    class Function : virtual public Object {
    private:
        vector<ast::Identifier*> Parameters; // 参数
        ast::BlockStatement* Body; // 函数的内容
        Environment* Env; // 函数对应的环境

    public:
        /**
         * 构造函数
         * @param parameters vector<ast::Identifier*>
         * @param body ast::BlockStatement*
         * @param env Environment*
         */
        Function(vector<ast::Identifier*> parameters, ast::BlockStatement* body, Environment* env)
            : Parameters(std::move(parameters)), Body(body), Env(env) {};

        ObjectType Type() override;

        string Inspect() override;

        /**
         * 从外部获取参数
         * @return vector<ast::Identifier*>
         */
        [[maybe_unused]] vector<ast::Identifier*> getParameters();

        /**
         * 从外部获取 Body
         * @return ast::BlockStatement*
         */
        [[maybe_unused]] ast::BlockStatement* getBody();

        /**
         * 从外部获取函数的环境
         * @return Environment*
         */
        [[maybe_unused]] Environment* getEnv();
    };

}


#endif //MONKEY_OBJECT_H
