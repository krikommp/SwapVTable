//
// Created by chenyifei on 7/19/2023.
//

#include <iostream>
#include "SwapVTable.h"

#include "ClassTemplate.h"

void g_func(void *Self) {
    printf("This is replace function, self address [%p]\n", Self);
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    E *a = new E;

    uint64_t offset = get_base_class_offset<E>(a);
    printf("Base Offset: %lld\n", offset);

    auto [real_class_ptr, old_vtable] = get_class_and_vtable((char *) a, offset);

    uint16_t v_count = get_v_fun_count(old_vtable);
    printf("vfunc count: %d\n", v_count);

    VFunMap map = {
            {0, (uintptr_t) g_func}
    };

    uintptr_t *new_vtable = swap_vtable(real_class_ptr, old_vtable, v_count, map);

    printf("========== old vtable ==========\n");
    foreach_call_vtable(real_class_ptr, old_vtable, v_count);
    printf("========== new vtable ==========\n");
    foreach_call_vtable(real_class_ptr, new_vtable, v_count);

    printf("========== A ==========\n");
    a->func_A_0();
    a->func_A_1();
    a->func_A_2();
    printf("========== B ==========\n");
    a->func_B_0();
    a->func_B_1();
    a->func_B_2();
    printf("========== E ==========\n");
    a->func_E_0();
    a->func_E_1();
    a->func_E_2();
    printf("========== END ==========\n");

    return 0;
}