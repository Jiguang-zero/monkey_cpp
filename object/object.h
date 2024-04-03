//
// Created by 86158 on 2024/4/3.
//

#ifndef MONKEY_OBJECT_H
#define MONKEY_OBJECT_H

#include <string>


using std::string;


// �������
namespace monkey::object {
    typedef string ObjectType;


    // �������� Integer
    extern const ObjectType INTEGER_OBJ;
    // ����ֵ
    extern const ObjectType BOOLEAN_OBJ;
    // ��ֵ
    extern const ObjectType NULL_OBJ;
}


namespace monkey::object {
    /**
     * ���� Object
     */
    class Object {
    public:
        Object() = default;

        // Object ������
        virtual ObjectType Type() {return "";};

        // Object �Ӳ�(ֵ�� string ��ʾ)
        virtual string Inspect() {return "";};
    };


    class Integer : virtual public Object {
    private:
        long long Value = 0;

    public:
        /**
         * ���캯��
         * @param value ���ⲿ��ȡ value
         */
        explicit Integer(long long value) : Value(value) {};

        [[maybe_unused]] [[maybe_unused]] void setValue(long long value);

        [[maybe_unused]] [[nodiscard]] long long getValue() const;

        string Inspect() override;
        ObjectType Type() override;

    };

    class Boolean : virtual public Object {
    private:
        // Ĭ���� true
        bool Value = true;

    public:
        // ���캯��
        explicit Boolean(bool value) : Value(value) {};

        // ���ⲿ���� Boolean �� Value
        [[maybe_unused]] [[maybe_unused]] void setValue(bool value);

        // ���ⲿ��ȡ Value, �޷����ⲿ�޸�
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
