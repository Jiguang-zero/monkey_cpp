//
// Created by 86158 on 2024/4/3.
//

#ifndef MONKEY_OBJECT_H
#define MONKEY_OBJECT_H

#include <string>


using std::string;


// 定义变量
namespace monkey::object {
    typedef string ObjectType;


    // 整数类型 Integer
    extern const ObjectType INTEGER_OBJ;
    // 布尔值
    extern const ObjectType BOOLEAN_OBJ;
    // 空值
    extern const ObjectType NULL_OBJ;
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

}


#endif //MONKEY_OBJECT_H
