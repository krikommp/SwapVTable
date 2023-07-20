//
// Created by chenyifei on 7/19/2023.
//

#ifndef SWAPVTABLE_SYSTEM_H
#define SWAPVTABLE_SYSTEM_H

#if defined(WIN64) || defined(_WIN64) || defined(__MINGW64__)
#define SWAP_VTABLE_OS_WINDOWS
#elif defined(WIN32) || defined(_WIN32) || defined(__MINGW32__)
#define SWAP_VTABLE_OS_WINDOWS
#elif defined(__linux__) || defined(linux)
#define SWAP_VTABLE_OS_LINUX
#elif defined(__APPLE__)
#define SWAP_VTABLE_OS_APPLE
#endif

#if defined(SWAP_VTABLE_OS_WINDOWS)
#include <Windows.h>
#elif defined(SWAP_VTABLE_OS_LINUX)
#include <unistd.h>
#include <sys/mman.h>
#elif defined(SWAP_VTABLE_OS_APPLE)
#include <unistd.h>
#include <sys/mman.h>
#include <mach/mach_init.h>
#include <mach/mach_vm.h>
#include <mach/vm_prot.h>
#include <mach/vm_map.h>
#endif

#if defined(SWAP_VTABLE_OS_WINDOWS)
#define _PTR_MAX_VALUE ((void*)0x000F'0000'0000'0000)
#else
#define _PTR_MAX_VALUE ((void*)0x0000'7FFF'FFFF'FFFF)
#endif

// 0x0000'7fbb'7241'afe0

#include <cstdint>
#include <cstring>
#include <tuple>
#include <functional>
#include <map>
#include <fstream>

#endif //SWAPVTABLE_SYSTEM_H
