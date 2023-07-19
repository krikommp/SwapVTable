cmake_minimum_required(VERSION 3.25)
project(SwapVTable)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_COMPILER "/usr/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()

# 生成可执行文件
add_executable(${PROJECT_NAME})
# 添加头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
# 添加源文件
target_sources(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/src/MemAccessor.cpp
        ${PROJECT_SOURCE_DIR}/src/SwapVTable.cpp
        ${PROJECT_SOURCE_DIR}/src/main.cpp)