//
// Created by lambor on 17-2-21.
//

#ifndef ANDROIDCPPEXAMPLES_ARGUMENT_H
#define ANDROIDCPPEXAMPLES_ARGUMENT_H

#include "point.h"
void drawByPointer(struct Point* p);
void drawByReference(struct Point& p);
void drawByValue(struct Point p);

void func(int a,int b,int c=3);

void func(double d);
void func(int i);


#endif //ANDROIDCPPEXAMPLES_ARGUMENT_H
