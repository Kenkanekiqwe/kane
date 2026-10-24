cmake_minimum_required(VERSION 3.24)

project(KANE VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "KANE's initial native shell currently targets Windows 10/11.")
endif()

add_library(kane_core STATIC
    src/memory_policy.cpp
)
target_include_directories(kane_core PUBLIC include)
target_compile_definitions(kane_core PUBLIC UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)

if(MSVC)
    target_compile_options(kane_core PRIVATE /W4 /permissive- /EHsc)
endif()

add_executable(kane_browser WIN32
    src/main.cpp
)
target_link_libraries(kane_browser PRIVATE
    kane_core
    d2d1
    dwrite
    windowscodecs
    dwmapi
    shcore
    comctl32
)
target_compile_definitions(kane_browser PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)

if(MSVC)
    target_compile_options(kane_browser PRIVATE /W4 /permissive- /EHsc)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(kane_core_tests tests/memory_policy_tests.cpp)
    target_link_libraries(kane_core_tests PRIVATE kane_core)
    add_test(NAME kane_core_tests COMMAND kane_core_tests)
endif()
