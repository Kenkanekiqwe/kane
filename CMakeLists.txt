cmake_minimum_required(VERSION 3.24)

project(KANE VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "KANE currently targets Windows 10/11 x64.")
endif()

# Keep the CEF revision pinned so builds are repeatable and security updates
# can be reviewed as explicit version changes.
set(KANE_CEF_VERSION "154.0.34+g14c5a08+chromium-154.0.8037.98")
include(cmake/DownloadCEF.cmake)
# CEF publishes the standard Windows x64 package as windows64. The sandbox
# bootstrap is part of the newer CEF process architecture, not a package suffix.
kane_download_cef(windows64 "${KANE_CEF_VERSION}" "${CMAKE_BINARY_DIR}/third_party/cef")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CEF_ROOT}/cmake")
set(USE_SANDBOX ON CACHE BOOL "Build KANE with the CEF Windows sandbox" FORCE)
find_package(CEF REQUIRED)

add_subdirectory("${CEF_LIBCEF_DLL_WRAPPER_PATH}" "${CMAKE_BINARY_DIR}/cef_libcef_dll_wrapper")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_ROOT}")

# Use CEF's official sample as the initial process/lifecycle harness while
# building the browser module under KANE's DLL name.
set(KANE_CEF_SAMPLE_DIR "${CMAKE_BINARY_DIR}/cef_kane_sample")
file(COPY "${CEF_ROOT}/tests/cefsimple/" DESTINATION "${KANE_CEF_SAMPLE_DIR}")
if(NOT EXISTS "${KANE_CEF_SAMPLE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "The selected CEF distribution does not include the cefsimple sample sources.")
endif()
file(READ "${KANE_CEF_SAMPLE_DIR}/CMakeLists.txt" KANE_CEF_SAMPLE_CMAKE)
string(REPLACE "set(CEF_TARGET \"cefsimple\")" "set(CEF_TARGET \"kane_browser\")"
       KANE_CEF_SAMPLE_CMAKE "${KANE_CEF_SAMPLE_CMAKE}")
file(WRITE "${KANE_CEF_SAMPLE_DIR}/CMakeLists.txt" "${KANE_CEF_SAMPLE_CMAKE}")
add_subdirectory("${KANE_CEF_SAMPLE_DIR}" "${CMAKE_BINARY_DIR}/cef_kane_sample_build")

add_library(kane_core STATIC src/memory_policy.cpp)
target_include_directories(kane_core PUBLIC include)
target_compile_definitions(kane_core PUBLIC UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
if(MSVC)
    target_compile_options(kane_core PRIVATE /W4 /permissive- /EHsc)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(kane_core_tests tests/memory_policy_tests.cpp)
    target_link_libraries(kane_core_tests PRIVATE kane_core)
    add_test(NAME kane_core_tests COMMAND kane_core_tests)
endif()
