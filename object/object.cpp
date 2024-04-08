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

    ObjectType Error::Type() {
        return ERROR_OBJ;
    }

    string Error::Inspect() {
        return "ERROR: " + Message;
    }

    [[maybe_unused]] string Error::getMessage() const {
        return Message;
    }

    ObjectType Function::Type() {
        return FUNCTION_OBJ;
    }

    string Function::Inspect() {
        std::ostringstream oss;

        vector<string> params;
        for (const auto & p : Parameters) {
            params.emplace_back(p->String());
        }

        oss << "fn";
        oss << "(";
        oss << std::accumulate(
                params.begin(),
                params.end(),
                string(),
                [] (const string& a, const string& b) -> string {
                    return a + (a.length() > 0 ? ", " : "") + b;
                }
        );
        oss << ") {\n";
        oss << Body->String();
        oss << "\n}";


        return oss.str();
    }

    [[maybe_unused]] vector<ast::Identifier *> Function::getParameters() {
        return Parameters;
    }

    [[maybe_unused]] ast::BlockStatement *Function::getBody() {
        return Body;
    }

    [[maybe_unused]] Environment *Function::getEnv() {
        return Env;
    }

    ObjectType String::Type() {
        return STRING_OBJ;
    }

    string String::Inspect() {
        return Value;
    }

    string String::getValue() const {
        return Value;
    }

    ObjectType Builtin::Type() {
        return BUILTIN_OBJ;
    }

    string Builtin::Inspect() {
        return "builtin function";
    }

    [[maybe_unused]] BuiltinFunction &Builtin::getFn() {
        return Fn;
    }

    ObjectType Array::Type() {
        return ARRAY_OBJ;
    }

    string Array::Inspect() {
        std::ostringstream oss;

        vector<string> elements;
        elements.reserve(Elements.size());
        for (const auto & e : Elements) {
            elements.emplace_back(e->Inspect());
        }

        auto elementsString = std::accumulate(
                elements.begin(),
                elements.end(),
                string(),
                [] (const string& a, const string& b) -> string {
                    return a + (a.empty()? "" : ", ") + b;
                }
                );

        oss << "[";
        oss << elementsString;
        oss << "]";

        return oss.str();
    }

    [[maybe_unused]] vector<Object *> Array::getElements() {
        return Elements;
    }
}

// 定义变量
namespace monkey::object {
    const ObjectType INTEGER_OBJ = "INTEGER";
    const ObjectType BOOLEAN_OBJ = "BOOLEAN";
    const ObjectType STRING_OBJ = "STRING";
    const ObjectType NULL_OBJ = "NULL";
    const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
    const ObjectType ERROR_OBJ = "ERROR";
    const ObjectType FUNCTION_OBJ = "FUNCTION";
    const ObjectType BUILTIN_OBJ = "BUILTIN";
    const ObjectType ARRAY_OBJ = "ARRAY";
}