//
// Created by 86158 on 2024/4/6.
//

#include "environment.h"


monkey::object::Object* monkey::object::Environment::Set(const string &name, monkey::object::Object *val) {
    store[name] = val;

    return val;
}

monkey::object::Object *monkey::object::Environment::Get(const string &name) { // NOLINT(*-no-recursion)
    auto it = store.find(name);
    if (it != store.end()) {
        return it->second;
    }
    else if (it == store.end() && outer != nullptr) {
        return outer->Get(name);
    }
    else {
        return nullptr;
    }
}

monkey::object::Environment *monkey::object::Environment::NewEnclosedEnvironment(monkey::object::Environment *outer) {
    auto * env = NewEnvironment();
    env->setOuter(outer);
    return env;
}
