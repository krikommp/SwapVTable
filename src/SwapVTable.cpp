//
// Created by chenyifei on 7/19/2023.
//

#include "SwapVTable.h"
#include "MemAccessor.h"

uint16_t get_v_fun_count(uintptr_t *vtable) {
    uint16_t v_count = 0;

    for(;;v_count++) {
        if (!IsValidPtr((void*)vtable[v_count]) || v_count > 500)
            break;
    }

    return v_count;
}

uintptr_t *swap_vtable(uint64_t class_ptr, uintptr_t *vtable, uint16_t num_v_func, const VFunMap& redirectMap) {
    bool status = false;
    mem_protect(class_ptr, num_v_func * sizeof(uintptr_t), ProtFlag(ProtFlag::R | ProtFlag::W), status);

    uintptr_t* new_vtable = new uintptr_t [num_v_func];
    memcpy(new_vtable, vtable, sizeof(uintptr_t) * num_v_func);

    for (const auto& p : redirectMap) {
        if (p.first < num_v_func) {
            new_vtable[p.first] = (uintptr_t)p.second;
        }
    }

    *(uintptr_t**)class_ptr = new_vtable;

    return new_vtable;
}

std::tuple<uint64_t, uintptr_t *> get_class_and_vtable(char *class_ptr, uint64_t offset) {
    uint64_t real_class_ptr = (uint64_t)(class_ptr + offset);
    uintptr_t* vtable = *(uintptr_t**)real_class_ptr;
    return std::make_tuple(real_class_ptr, vtable);
}

void foreach_call_vtable(uint64_t class_ptr, uintptr_t *vtable, uint16_t num_v_func) {
    typedef void (*Fun)(void*);
    for (int16_t i = 0; i < num_v_func; ++i)
    {
        uintptr_t p = vtable[i];
        Fun f = reinterpret_cast<Fun>(p);
        f((void*)class_ptr);
    }
}
