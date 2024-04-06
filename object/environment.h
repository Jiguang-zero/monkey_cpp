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
     * 环境， 变量的值所对应的空间
     */
    class Environment {
    private:
        map<string, Object*> store;

        /**
         * 构造函数设为私有， 无法从外部访问，只能通过 NewEnvironment 访问
         */
        Environment() = default;

    public:
        /**
         * 从外部创建环境对象
         * @return Environment*
         */
        static Environment* NewEnvironment() {
            return new Environment();
        }

        /**
         * 从环境 map 存储中 获取对象
         * @param name  const string&
         * @return Object* 如果找不到，那么返回空指针
         */
        Object* Get(const string& name);

        /**
         * 从外部存储对象到环境中
         * @param name const string&
         * @param val  Object*
         * @return val 返回存储的对象
         */
        Object* Set(const string& name, Object*  val);
    };

}

#endif //MONKEY_ENVIRONMENT_H
