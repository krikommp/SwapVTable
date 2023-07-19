//
// Created by chenyifei on 7/19/2023.
//

#ifndef SWAPVTABLE_CLASSTEMPLATE_H
#define SWAPVTABLE_CLASSTEMPLATE_H

#define FULL_CLASS_TEMPLATE(Class) \
public: \
    void func_##Class##_0() {\
        printf("%s::func_0, self address = [%p]\n", #Class, this);\
    } \
    virtual void func_##Class##_1() {\
        printf("%s::func_1, self address = [%p]\n", #Class, this);\
    } \
    static void func_##Class##_2() {\
        printf("%s::func_2\n", #Class);\
    } \
 private: \
    int v_##Class; \
    static int g_##Class;

class A
{
    FULL_CLASS_TEMPLATE(A)
};

class B
{
    FULL_CLASS_TEMPLATE(B)
};

class C : public A, public B
{
    FULL_CLASS_TEMPLATE(C)
};

class E : virtual public A, virtual public B
{
    FULL_CLASS_TEMPLATE(E)
};

#endif //SWAPVTABLE_CLASSTEMPLATE_H
