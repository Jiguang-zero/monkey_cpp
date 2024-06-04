//
// Created by 86158 on 2024/4/3.
//

#ifndef MONKEY_OBJECT_H
#define MONKEY_OBJECT_H

#include <string>
#include <utility>
#include <vector>
#include <numeric>
#include <unordered_map>

#include "../ast/Program.h"

using std::string;
using std::vector;


// �������
namespace monkey::object {
    class Environment; // ����

    typedef string ObjectType;


    // �������� Integer
    extern const ObjectType INTEGER_OBJ;
    // ����ֵ
    extern const ObjectType BOOLEAN_OBJ;
    // string
    extern const ObjectType STRING_OBJ;

    // ��ֵ
    extern const ObjectType NULL_OBJ;
    // return ֵ
    extern const ObjectType RETURN_VALUE_OBJ;

    // ����
    extern const ObjectType ERROR_OBJ;

    // ����
    extern const ObjectType FUNCTION_OBJ;

    // ���ú���
    extern const ObjectType BUILTIN_OBJ;

    // ����
    extern const ObjectType ARRAY_OBJ;

    // ��ϣ����
    extern const ObjectType HASH_OBJ;
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

    struct HashKey {
        ObjectType Type;
        size_t Value = 0;

        /**
         * ��������أ��ж����� HashKey �Ƿ����
         * @param other
         * @return
         */
        bool operator==(const HashKey& other) const;

        /**
         * ��������أ��ж����� HashKey �Ƿ����
         * @param other
         * @return
         */
        bool operator!=(const HashKey& other) const;

        /**
         * ���� < ����������� keywords ����
         * ��ʱֻ���� ��ϣֵ ���бȽ�
         * @param other
         * @return
         */
        bool operator<(const HashKey& other) const;
    };

    class Hashable {
    public:
        virtual HashKey HashKey() = 0;
    };


    class Integer : virtual public Object, public Hashable{
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

        struct HashKey HashKey() override;

    };

    class Boolean : virtual public Object, public Hashable{
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

        struct HashKey HashKey() override;

    };

    class Null : virtual public Object {
    public:
        ObjectType Type() override;

        string Inspect() override;

    };


    class ReturnValue : virtual public Object {
    private:
        Object* Value; // ���صĶ���

    public:
        /**
         * ���캯���� ���� value ����
         * @param value
         */
        explicit ReturnValue(Object* value) : Value(value) {};

        ObjectType Type() override;

        string Inspect() override;

        /**
         * ���ⲿ��ȡֵ
         * @return ��������: ���Դ��ⲿ�޸�
         */
        [[maybe_unused]] object::Object*& getValue();

        /**
         * ���ⲿ����ֵ
         * @param value object::Object* & ����
         */
        [[maybe_unused]] void setValue(object::Object* & value);

    };

    /**
     * �������
     */
    class Error : virtual public Object {
    private:
        string Message;

    public:
        /**
         * ���캯�����������Ĺ���ֻ�ܴ��� string ֵ
         * @param message
         */
        explicit Error(string message) : Message(std::move(message)) {}

        [[maybe_unused]] [[nodiscard]] string getMessage() const;

        ObjectType Type() override;

        string Inspect() override;

    };

    /**
     * ��������
     */
    class Function : virtual public Object {
    private:
        vector<ast::Identifier*> Parameters; // ����
        ast::BlockStatement* Body; // ����������
        Environment* Env; // ������Ӧ�Ļ���

    public:
        /**
         * ���캯��
         * @param parameters vector<ast::Identifier*>
         * @param body ast::BlockStatement*
         * @param env Environment*
         */
        Function(vector<ast::Identifier*> parameters, ast::BlockStatement* body, Environment* env)
            : Parameters(std::move(parameters)), Body(body), Env(env) {};

        ObjectType Type() override;

        string Inspect() override;

        /**
         * ���ⲿ��ȡ����
         * @return vector<ast::Identifier*>
         */
        [[maybe_unused]] vector<ast::Identifier*> getParameters();

        /**
         * ���ⲿ��ȡ Body
         * @return ast::BlockStatement*
         */
        [[maybe_unused]] ast::BlockStatement* getBody();

        /**
         * ���ⲿ��ȡ�����Ļ���
         * @return Environment*
         */
        [[maybe_unused]] Environment* getEnv();
    };

    class String : virtual public Object, public Hashable{
    private:
        string Value;

    public:
        explicit String(string value) : Value(std::move(value)) {}

        ObjectType Type() override;

        string Inspect() override;

        [[nodiscard]] string getValue() const;

        struct HashKey HashKey() override;
    };

    // �� ����Object*���ͣ�����Ϊvector<Object*>�ĺ�������Ϊ BuiltinFunction
    typedef Object* (*BuiltinFunction) (const vector<Object*>&);

    class Builtin : virtual public Object {
    private:
        BuiltinFunction Fn;

    public:
        // ���캯��
        explicit Builtin(BuiltinFunction fn) : Fn(fn) {}

        [[maybe_unused]] BuiltinFunction & getFn();

        ObjectType Type() override;

        string Inspect() override;

    };

    /**
     * �������
     */
    class Array : virtual public Object {
    private:
        vector<Object*> Elements;

    public:
        [[maybe_unused]] vector<Object*> getElements();

        explicit Array(vector<Object*> elements) : Elements(std::move(elements)) {}

        ObjectType Type() override;

        string Inspect() override;

    };

    struct HashPair {
        Object* Key;
        Object* Value;

        HashPair() : Key(nullptr), Value(nullptr) {}

        HashPair(Object* key, Object* value) : Key(key), Value(value) {}

        // Ĭ�ϸ��ƺ���
        HashPair(const HashPair& hashPair);
    };

    class Hash : virtual public Object {
    private:
        map<HashKey, HashPair> Pairs;

    public:
        /**
         * ���캯�������� pairs
         * @param pairs keywords<HashKey, HashPair>
         */
        explicit Hash(map<HashKey, HashPair> pairs) : Pairs(std::move(pairs)) {}

        ObjectType Type() override;

        string Inspect() override;

        /**
         * ���ⲿ��ȡ pairs
         * @return keywords<HashKey, HashPair>, �����ã��޷����ⲿ����
         */
        [[maybe_unused]] map<HashKey, HashPair> getPairs();
    };

}


#endif //MONKEY_OBJECT_H
