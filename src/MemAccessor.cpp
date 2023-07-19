//
// Created by chenyifei on 7/19/2023.
//
#include "MemAccessor.h"
#include "System.h"

#if defined(SWAP_VTABLE_OS_WINDOWS)

int TranslateProtection(const ProtFlag flags) {
    int NativeFlag = 0;
    if (flags == ProtFlag::X)
        NativeFlag = PAGE_EXECUTE;
    if (flags == ProtFlag::R)
        NativeFlag = PAGE_READONLY;
    if (flags == ProtFlag::W || (flags == (ProtFlag::R | ProtFlag::W)))
        NativeFlag = PAGE_READWRITE;
    if ((flags & ProtFlag::X) && (flags & ProtFlag::R))
        NativeFlag = PAGE_EXECUTE_READ;
    if ((flags & ProtFlag::X) && (flags & ProtFlag::W))
        NativeFlag = PAGE_EXECUTE_READWRITE;
    if (flags & ProtFlag::NONE)
        NativeFlag = PAGE_NOACCESS;
    return NativeFlag;
}

ProtFlag TranslateProtection(const int prot) {
    ProtFlag flags = ProtFlag::UNSET;
    switch (prot) {
        case PAGE_EXECUTE:
            flags = ProtFlag(flags | ProtFlag::X);
            break;
        case PAGE_READONLY:
            flags = ProtFlag(flags | ProtFlag::R);
            break;
        case PAGE_READWRITE:
            flags = ProtFlag(flags | ProtFlag::W);
            flags = ProtFlag(flags | ProtFlag::R);
            break;
        case PAGE_EXECUTE_READWRITE:
            flags = ProtFlag(flags | ProtFlag::X);
            flags = ProtFlag(flags | ProtFlag::R);
            flags = ProtFlag(flags | ProtFlag::W);
            break;
        case PAGE_EXECUTE_READ:
            flags = ProtFlag(flags | ProtFlag::X);
            flags = ProtFlag(flags | ProtFlag::R);
            break;
        case PAGE_NOACCESS:
            flags = ProtFlag(flags | ProtFlag::NONE);
            break;
    }
    return flags;
}

ProtFlag mem_protect(uint64_t dest, uint64_t size, ProtFlag prot, bool &status) {
    DWORD orig;
    status = VirtualProtect((char*)dest, (SIZE_T)size, TranslateProtection(prot), &orig) != 0;
    return TranslateProtection(orig);
}

#endif


