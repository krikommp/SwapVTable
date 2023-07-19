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
#define SWAP_VATBLE_OS_LINUX
#elif defined(__APPLE__)
#define SWAP_VTABLE_OS_APPLE
#endif

#if defined(SWAP_VTABLE_OS_WINDOWS)
#include <Windows.h>
#endif

#if defined(SWAP_VTABLE_OS_WINDOWS)
#define _PTR_MAX_VALUE ((void*)0x000F000000000000)
#else
#define _PTR_MAX_VALUE ((void*)0xFFF00000)
#endif

#include <cstdint>
#include <cstring>
#include <tuple>
#include <functional>
#include <map>

#endif //SWAPVTABLE_SYSTEM_H
