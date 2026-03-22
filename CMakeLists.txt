cmake_minimum_required(VERSION 3.15)
project(CPPMultithreading VERSION 1.0.0 LANGUAGES CXX)

# C++标准设置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译器选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
elseif(MSVC)
    add_compile_options(/W4 /O2)
endif()

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 源文件分组
# 01_basics
add_executable(hello_threads src/01_basics/hello_threads.cpp)
target_compile_features(hello_threads PRIVATE cxx_std_17)

# 02_concurrency
add_executable(thread_pool src/02_concurrency/thread_pool.cpp)
target_compile_features(thread_pool PRIVATE cxx_std_17)

# 03_sync
add_executable(thread_sync src/03_sync/thread_sync.cpp)
target_compile_features(thread_sync PRIVATE cxx_std_17)

# 04_patterns
add_executable(producer_consumer src/04_patterns/producer_consumer.cpp)
target_compile_features(producer_consumer PRIVATE cxx_std_17)

# 05_projects
add_executable(log_analyzer_project src/05_projects/log_analyzer_project.cpp)
target_compile_features(log_analyzer_project PRIVATE cxx_std_17)

# 测试
enable_testing()
add_test(NAME hello_threads COMMAND hello_threads)
add_test(NAME thread_sync COMMAND thread_sync)
add_test(NAME producer_consumer COMMAND producer_consumer)
