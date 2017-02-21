//
// Created by lambor on 17-2-21.
//

#ifndef ANDROIDCPPEXAMPLES_CLASS_H
#define ANDROIDCPPEXAMPLES_CLASS_H


class A {
public:
    A();
    A(int value);
    ~A();

    void print();

    int value;
private:
    void reset();
};


#endif //ANDROIDCPPEXAMPLES_CLASS_H
