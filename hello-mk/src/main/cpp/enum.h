//
// Created by lambor on 17-2-20.
//

#ifndef ANDROIDCPPEXAMPLES_ENUM_H
#define ANDROIDCPPEXAMPLES_ENUM_H

enum {ONE = 1, TWO = 2, THREE, FOUR};

//类型安全
enum NumbersSafe {ONE_SAFE = 1, TWO_SAFE = 2, THREE_SAFE, FOUR_SAFE};
//类型不安全
enum NumbersUnSafe {ONE_UNSAFE = 1, TWO_UNSAFE = 2, THREE_UNSAFE, FOUR_UNSAFE};
//java 枚举
enum NumbersJava {ONE_JAVA = 1, TWO_JAVA = 2, THREE_JAVA, FOUR_JAVA};

#endif //ANDROIDCPPEXAMPLES_ENUM_H
