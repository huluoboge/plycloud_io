cmake_minimum_required(VERSION 3.1)

project(plycloud_io C CXX)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.")
endif()

if(CMAKE_BUILD_TYPE)
    message(STATUS "Build type specified as ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Build type not specified, using Release")
    set(CMAKE_BUILD_TYPE Release)
endif()


# Set build path
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

if(UNIX)
    set(CMAKE_C_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# ==============================================================================
# Check C++11 support
# ==============================================================================
# 检查C++11支持
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    message(STATUS "The compiler supports C++11.")
    # 设置C++11标准
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(FATAL_ERROR "The compiler does NOT support C++11. Please use a compiler that supports C++11.")
endif()

find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # target_link_libraries(YourExecutable ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find Boost libraries!")
endif()
add_subdirectory(src)