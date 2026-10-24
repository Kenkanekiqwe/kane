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

# Give the browser a lightweight KANE start page instead of the upstream
# sample's Google homepage. The page is embedded as a data URI, so packaging
# does not need another nested assets directory or extra disk reads.
set(KANE_CEF_SAMPLE_APP "${KANE_CEF_SAMPLE_DIR}/simple_app.cc")
file(READ "${KANE_CEF_SAMPLE_APP}" KANE_CEF_SAMPLE_APP_CONTENT)
string(FIND "${KANE_CEF_SAMPLE_APP_CONTENT}" "url = \"https://www.google.com\";" KANE_DEFAULT_URL_POSITION)
if(KANE_DEFAULT_URL_POSITION EQUAL -1)
    message(FATAL_ERROR "Could not patch the pinned CEF sample's default start URL.")
endif()
string(REPLACE
    "#include \"tests/cefsimple/simple_app.h\""
    "#include \"tests/cefsimple/simple_app.h\"\n#include \"include/cef_parser.h\"\n#include \"src/kane_home_page.h\""
    KANE_CEF_SAMPLE_APP_CONTENT "${KANE_CEF_SAMPLE_APP_CONTENT}")
string(REPLACE
    "url = \"https://www.google.com\";"
    "url = \"data:text/html;base64,\" + CefBase64Encode(kane::ui::kHomePageHtml, sizeof(kane::ui::kHomePageHtml) - 1).ToString();"
    KANE_CEF_SAMPLE_APP_CONTENT "${KANE_CEF_SAMPLE_APP_CONTENT}")
# Prefer CEF's native Chrome runtime by default. Unlike the lightweight Alloy
# runtime, Chrome runtime provides real browser chrome (including tabs and
# address/navigation UI) instead of a bare content window. Alloy remains
# available as an explicit opt-out for debugging with --use-alloy-style.
string(REPLACE
    "cef_runtime_style_t runtime_style = CEF_RUNTIME_STYLE_DEFAULT;"
    "cef_runtime_style_t runtime_style = CEF_RUNTIME_STYLE_CHROME;"
    KANE_CEF_SAMPLE_APP_CONTENT "${KANE_CEF_SAMPLE_APP_CONTENT}")
file(WRITE "${KANE_CEF_SAMPLE_APP}" "${KANE_CEF_SAMPLE_APP_CONTENT}")

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
