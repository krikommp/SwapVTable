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

#elif defined(SWAP_VTABLE_OS_LINUX)

struct region_t {
    uint64_t start;
    uint64_t end;
    ProtFlag prot;
};

static size_t getPageSize() {
    return static_cast<uint64_t>(sysconf(_SC_PAGESIZE));;
}

static region_t get_region_from_addr(uint64_t addr) {
    region_t res{};

    std::ifstream f("/proc/self/maps");
    std::string s;
    while (std::getline(f, s)) {
        if (!s.empty() && s.find("vdso") == std::string::npos && s.find("vsyscall") == std::string::npos) {
            char *strend = &s[0];
            uint64_t start = strtoul(strend, &strend, 16);
            uint64_t end = strtoul(strend + 1, &strend, 16);
            if (start != 0 && end != 0 && start <= addr && addr < end) {
                res.start = start;
                res.end = end;

                ++strend;
                if (strend[0] == 'r')
                    res.prot = ProtFlag(res.prot | ProtFlag::R);

                if (strend[1] == 'w')
                    res.prot = ProtFlag(res.prot | ProtFlag::W);

                if (strend[2] == 'x')
                    res.prot = ProtFlag(res.prot | ProtFlag::X);

                if (res.prot == ProtFlag::UNSET)
                    res.prot = ProtFlag::NONE;

                break;
            }
        }
    }
    return res;
}

int TranslateProtection(const ProtFlag flags) {
    int NativeFlag = PROT_NONE;
    if (flags & ProtFlag::X)
        NativeFlag |= PROT_EXEC;

    if (flags & ProtFlag::R)
        NativeFlag |= PROT_READ;

    if (flags & ProtFlag::W)
        NativeFlag |= PROT_WRITE;

    if (flags & ProtFlag::NONE)
        NativeFlag = PROT_NONE;
    return NativeFlag;
}

ProtFlag TranslateProtection(const int prot) {
    ProtFlag flags = ProtFlag::UNSET;

    if (prot & PROT_EXEC)
        flags = ProtFlag(flags | ProtFlag::X);

    if (prot & PROT_READ)
        flags = ProtFlag(flags | ProtFlag::R);

    if (prot & PROT_WRITE)
        flags = ProtFlag(flags | ProtFlag::W);

    if (prot == PROT_NONE)
        flags = ProtFlag(flags | ProtFlag::NONE);

    return flags;
}

ProtFlag mem_protect(uint64_t dest, uint64_t size, ProtFlag prot, bool &status) {
    region_t region_infos = get_region_from_addr(dest);
    uint64_t aligned_dest = MEMORY_ROUND(dest, getPageSize());
    uint64_t aligned_size = MEMORY_ROUND_UP(size, getPageSize());
    status = mprotect((void *) aligned_dest, aligned_size, TranslateProtection(prot)) == 0;
    return region_infos.prot;
}

#elif defined(SWAP_VTABLE_OS_APPLE)

size_t PLH::getPageSize()
{
	return static_cast<uint64_t>(sysconf(_SC_PAGESIZE));
}

int TranslateProtection(const ProtFlag flags) {
    int NativeFlag = VM_PROT_NONE;
    if (flags & ProtFlag::X)
        NativeFlag |= PROT_EXEC;

    if (flags & ProtFlag::R)
        NativeFlag |= PROT_READ;

    if (flags & ProtFlag::W)
        NativeFlag |= PROT_WRITE;

    if (flags & ProtFlag::NONE)
        NativeFlag = PROT_NONE;
    return NativeFlag;
}

ProtFlag TranslateProtection(const int prot) {
    ProtFlag flags = ProtFlag::UNSET;

    if (prot & PROT_EXEC)
        flags = ProtFlag(flags | ProtFlag::X);

    if (prot & PROT_READ)
        flags = ProtFlag(flags | ProtFlag::R);

    if (prot & PROT_WRITE)
        flags = ProtFlag(flags | ProtFlag::W);

    if (prot == PROT_NONE)
        flags = ProtFlag(flags | ProtFlag::NONE);

    return flags;
}

ProtFlag mem_protect(uint64_t dest, uint64_t size, ProtFlag prot, bool &status) {
    status = mach_vm_protect(mach_task_self(), (mach_vm_address_t)MEMORY_ROUND(dest, getPageSize()), (mach_vm_size_t)MEMORY_ROUND_UP(size, getPageSize()), FALSE, TranslateProtection(prot)) == KERN_SUCCESS;
    return ProtFlag(ProtFlag::R | ProtFlag::X);
}

#endif