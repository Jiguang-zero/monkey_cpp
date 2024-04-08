//
// Created by 86158 on 2024/4/8.
//

#ifndef MONKEY_BUILTIN_H
#define MONKEY_BUILTIN_H

#include "../object/object.h"
#include "Evaluator.h"


namespace monkey::evaluator {
    extern map<string, object::Builtin *> builtins;
}


#endif //MONKEY_BUILTIN_H
