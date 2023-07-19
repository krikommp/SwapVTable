//
// Created by chenyifei on 7/19/2023.
//

#ifndef SWAPVTABLE_MEMACCESSOR_H
#define SWAPVTABLE_MEMACCESSOR_H

enum ProtFlag
{
    UNSET = 0,
    X = 1 << 1,
    R = 1 << 2,
    W = 1 << 3,
    S = 1 << 4,
    P = 1 << 5,
    NONE = 1 << 6,
    RWX = R | W | X
};

#if defined(SWAP_VTABLE_OS_WINDOWS)

int TranslateProtection(const ProtFlag flags);

ProtFlag TranslateProtection(const int prot);

ProtFlag mem_protect(uint64_t dest, uint64_t size, ProtFlag prot, bool& status);

#endif

#endif //SWAPVTABLE_MEMACCESSOR_H
