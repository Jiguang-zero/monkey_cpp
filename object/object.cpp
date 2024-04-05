//
// Created by 86158 on 2024/4/3.
//

#include "object.h"

namespace monkey::object {

    string Integer::Inspect() {
        return std::to_string(Value);
    }

    ObjectType Integer::Type() {
        return INTEGER_OBJ;
    }

    [[maybe_unused]] void Integer::setValue(long long int value) {
        Value = value;
    }

    long long Integer::getValue() const {
        return Value;
    }

    string Boolean::Inspect() {
        return Value ? "true" : "false";
    }

    ObjectType Boolean::Type() {
        return BOOLEAN_OBJ;
    }

    [[maybe_unused]] void Boolean::setValue(bool value) {
        Value = value;
    }

    bool Boolean::getValue() const {
        return Value;
    }

    ObjectType Null::Type() {
        return NULL_OBJ;
    }

    string Null::Inspect() {
        return "null";
    }

    ObjectType ReturnValue::Type() {
        return RETURN_VALUE_OBJ;
    }

    string ReturnValue::Inspect() {
        return Value->Inspect();
    }

    [[maybe_unused]] object::Object *&ReturnValue::getValue() {
        return Value;
    }

    [[maybe_unused]] void ReturnValue::setValue(Object *&value) {
        Value = value;
    }
}

// 定义变量
namespace monkey::object {
    const ObjectType INTEGER_OBJ = "INTEGER";
    const ObjectType BOOLEAN_OBJ = "BOOLEAN";
    const ObjectType NULL_OBJ = "NULL";
    const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
}