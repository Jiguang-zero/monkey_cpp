//
// Created by 86158 on 2024/4/8.
//

#ifndef MONKEY_OBJECT_TEST_H
#define MONKEY_OBJECT_TEST_H

#include "object.h"


void testStringHashKey() {
    cout << "Test testStringHashKey() START:" << endl;

    bool flag(true);

    auto * hello1 = new object::String("Hello World!");
    auto * hello2 = new object::String("Hello World!");

    auto * diff1 = new object::String("My name is johnny");
    auto * diff2 = new object::String("My name is johnny");

    if (hello1->HashKey() != hello2->HashKey()) {
        cout << "hello1 hello2 are not the same." << endl;
        flag = false;
    }

    if (diff1->HashKey() != diff2->HashKey()) {
        cout << "diff1 diff2 are not the same." << endl;
        flag = false;
    }

    if (hello1->HashKey() == diff1->HashKey()) {
        cout << "hello1 is the same with diff1" << endl;
        flag = false;
    }


    cout << "Test testStringHashKey() END: " << (flag ? "PASS" : "FAIL") << endl;
}


#endif //MONKEY_OBJECT_TEST_H
