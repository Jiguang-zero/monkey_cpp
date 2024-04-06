//
// Created by 86158 on 2024/4/6.
//

#ifndef MONKEY_ENVIRONMENT_H
#define MONKEY_ENVIRONMENT_H

#include "object.h"
#include <map>


using std::map;

namespace monkey::object {
    /**
     * ������ ������ֵ����Ӧ�Ŀռ�
     */
    class Environment {
    private:
        map<string, Object*> store;
        Environment* outer;

        /**
         * ���캯����Ϊ˽�У� �޷����ⲿ���ʣ�ֻ��ͨ�� NewEnvironment ����
         */
        Environment() : outer(nullptr) {};

    public:
        /**
         * ���ⲿ������������
         * @return Environment*
         */
        static Environment* NewEnvironment() {
            return new Environment();
        }

        /**
         * ���ⲿ���� outer
         * @param o Environment* outer
         */
        [[maybe_unused]] void setOuter(Environment* o) {
            outer = o;
        }

        /**
         * �������еĻ��������µĻ���
         * @param outer Environment*
         * @return Environment*
         */
        static Environment* NewEnclosedEnvironment(Environment* outer);

        /**
         * �ӻ��� map �洢�� ��ȡ����
         * @param name  const string&
         * @return Object* ����Ҳ�������ô���ؿ�ָ��
         */
        Object* Get(const string& name);

        /**
         * ���ⲿ�洢���󵽻�����
         * @param name const string&
         * @param val  Object*
         * @return val ���ش洢�Ķ���
         */
        Object* Set(const string& name, Object*  val);
    };

}

#endif //MONKEY_ENVIRONMENT_H
