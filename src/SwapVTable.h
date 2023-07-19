//
// Created by chenyifei on 7/19/2023.
//

#ifndef SWAPVTABLE_SWAPVTABLE_H
#define SWAPVTABLE_SWAPVTABLE_H
#include "System.h"
inline bool IsValidPtr(void* p) { return (p >= (void*)0x10000) && (p < _PTR_MAX_VALUE) && p != nullptr; }

typedef std::map<int64_t, uintptr_t> VFunMap;

uint16_t get_v_fun_count(uintptr_t* vtable);

std::tuple<uint64_t, uintptr_t*> get_class_and_vtable(char* class_ptr, uint64_t offset);

uintptr_t* swap_vtable(uint64_t class_ptr, uintptr_t* vtable, uint16_t num_v_func, const VFunMap& redirectMap);

void foreach_call_vtable(uint64_t class_ptr, uintptr_t* vtable, uint16_t num_v_func);

template <typename T, typename U>
int64_t get_base_class_offset(U* class_ptr)
{
    T* base_ptr = dynamic_cast<T*>(class_ptr);
    if (base_ptr)
        return (uint64_t)base_ptr - (uint64_t)class_ptr;
    return 0;
}

#endif //SWAPVTABLE_SWAPVTABLE_H
