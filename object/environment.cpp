//
// Created by 86158 on 2024/4/6.
//

#include <iostream>
#include "environment.h"


monkey::object::Object* monkey::object::Environment::Set(const string &name, monkey::object::Object *val) {
    store[name] = val;

    return val;
}

monkey::object::Object *monkey::object::Environment::Get(const string &name) {
    auto it = store.find(name);
    if (it != store.end()) {
        return it->second;
    }
    else {
        return nullptr;
    }
}
