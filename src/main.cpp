//
// Created by chenyifei on 7/19/2023.
//

#include <iostream>
#include "SwapVTable.h"
#include "MemAccessor.h"
#include "ClassTemplate.h"

void g_func(void *Self) {
    printf("This is replace function, self address [%p]\n", Self);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    C *a = new C;

    uint64_t offset = get_base_class_offset<B>(a);
    printf("Base Offset: %lld\n", offset);

    auto [real_class_ptr, old_vtable] = get_class_and_vtable((char *) a, offset);
    printf("old vtable address: %p\n", old_vtable);

    uint16_t v_count = get_v_fun_count(old_vtable);
    printf("vfunc count: %d\n", v_count);

    VFunMap map = {
            {0, (uintptr_t) g_func},
            //{1, (uintptr_t) g_func}
    };

    swap_vtable_for_instance(real_class_ptr, old_vtable, v_count, map);


    printf("========== A ==========\n");
    a->func_A_0();
    a->func_A_1();
    a->func_A_2();
    printf("========== B ==========\n");
    a->func_B_0();
    a->func_B_1();
    a->func_B_2();
    printf("========== C ==========\n");
    a->func_C_0();
    a->func_C_1();
    a->func_C_2();
    printf("========== END ==========\n");

    C* c = new C;
    printf("========== A ==========\n");
    c->func_A_0();
    c->func_A_1();
    c->func_A_2();
    printf("========== B ==========\n");
    c->func_B_0();
    c->func_B_1();
    c->func_B_2();
    printf("========== C ==========\n");
    c->func_C_0();
    c->func_C_1();
    c->func_C_2();
    printf("========== END ==========\n");

    A* o = new A;
    printf("========== A ==========\n");
    o->func_A_0();
    o->func_A_1();
    o->func_A_2();
    
    return 0;
}