//
// Created by 86158 on 2024/4/8.
//

#include "builtin.h"

namespace monkey::evaluator {
    map<string, object::Builtin *> builtins = {
            /**
             * len 函数
             * 参数为 string 或者 array
             */
            {
                    "len",
                    new object::Builtin(
                            [](const vector<object::Object *> &args) -> object::Object * {
                                if (args.size() != 1) {
                                    string msg =
                                            "wrong number of arguments. got=" +
                                            std::to_string(args.size()) +
                                            ", want=1";
                                    return Evaluator::newError(msg);
                                }

                                if (auto *stringObject = dynamic_cast<object::String *>(args[0])) {
                                    return new object::Integer(
                                            static_cast<long long>(stringObject->getValue().length()));
                                } else if (auto *arrayObject = dynamic_cast<object::Array *>(args[0])) {
                                    return new object::Integer(
                                            static_cast<long long>(arrayObject->getElements().size()));
                                } else {
                                    auto msg = "argument to `len` not supported, got " + args[0]->Type();
                                    return Evaluator::newError(msg);
                                }
                            }
                    ),
            },

            /**
              * fist 函数
              * 参数为 array
              */
            {
                    "first",
                    new object::Builtin(
                            [](const vector<object::Object *> &args) -> object::Object * {
                                if (args.size() != 1) {
                                    auto msg = "wrong number of arguments. got=" + std::to_string(args.size()) +
                                               ", want=1";
                                    return Evaluator::newError(msg);
                                }

                                if (args[0]->Type() != object::ARRAY_OBJ) {
                                    auto msg = "argument to `first` must be ARRAY, got " + args[0]->Type();
                                    return Evaluator::newError(msg);
                                }

                                auto *arr = dynamic_cast<object::Array *>(args[0]);
                                if (!arr->getElements().empty()) {
                                    return arr->getElements()[0];
                                }

                                return (object::Object * ) & Evaluator::MY_NULL;
                            }
                    )
            },

            /**
             * last 函数
             * 参数: 数组
             */
            {
                    "last",
                    new object::Builtin(
                            [](const vector<object::Object *> &args) -> object::Object * {
                                if (args.size() != 1) {
                                    auto msg = "wrong number of arguments. got=" + std::to_string(args.size()) +
                                               ", want=1";
                                    return Evaluator::newError(msg);
                                }

                                if (args[0]->Type() != object::ARRAY_OBJ) {
                                    auto msg = "argument to `last` must be ARRAY, got " + args[0]->Type();
                                    return Evaluator::newError(msg);
                                }

                                auto *arr = dynamic_cast<object::Array *>(args[0]);
                                auto length = arr->getElements().size();
                                if (length > 0) {
                                    return arr->getElements()[length - 1];
                                }

                                return (object::Object * ) & Evaluator::MY_NULL;
                            }
                    )
            },

            /**
             * rest 函数
             * 参数: 数组 获取数组除了第一个元素的剩余数组，如果输入长度为 1，那么返回空数组
             */
            {
                    "rest",
                    new object::Builtin{
                            [](const vector<object::Object *> &args) -> object::Object * {
                                if (args.size() != 1) {
                                    auto msg = "wrong number of arguments. got=" + std::to_string(args.size()) +
                                               ", want=1";
                                    return Evaluator::newError(msg);
                                }

                                if (args[0]->Type() != object::ARRAY_OBJ) {
                                    auto msg = "argument to `rest` must be ARRAY, got " + args[0]->Type();
                                    return Evaluator::newError(msg);
                                }

                                auto *arr = dynamic_cast<object::Array *>(args[0]);
                                auto length = arr->getElements().size();

                                if (length > 0) {
                                    if (length == 1) {
                                        return new object::Array({});
                                    }
                                    vector<object::Object *> newElements(length - 1);
                                    for (int i = 0; i < length - 1; i ++ ) {
                                        newElements[i] = arr->getElements()[i + 1];
                                    }

                                    return new object::Array(newElements);
                                }

                                return (object::Object * ) & Evaluator::MY_NULL;
                            }
                    }
            },

            /**
             * push 函数
             * 参数: (数组, 对象) 将对象添加到数组的末尾
             */
            {
                    "push",
                    new object::Builtin(
                            [](const vector<object::Object *> &args) -> object::Object * {
                                if (args.size() != 2) {
                                    auto msg = "wrong number of arguments. got=" + std::to_string(args.size()) +
                                               ", want=2";
                                    return Evaluator::newError(msg);
                                }

                                if (args[0]->Type() != object::ARRAY_OBJ) {
                                    auto msg = "argument to `push` must be ARRAY, got " + args[0]->Type();
                                    return Evaluator::newError(msg);
                                }

                                auto *arr = dynamic_cast<object::Array *>(args[0]);

                                auto newElements = arr->getElements();
                                newElements.emplace_back(args[1]);

                                return new object::Array(newElements);
                            }
                    )
            }
    };
}