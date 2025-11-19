cmake_minimum_required(VERSION 3.14)
project(lab1 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    src/utils.c
)

add_executable(lab1 
    main.c
    ${SRC_FILES}
)

target_include_directories(lab1 PRIVATE include)

if(MSVC)
    target_compile_options(lab1 PRIVATE /W4 /WX)
else()
    target_compile_options(lab1 PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

cmake_policy(SET CMP0135 NEW)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_disable_pthreads ON CACHE BOOL "Disable pthreads for Windows" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/tests.cpp
    ${SRC_FILES}
)

target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest_main)

if(MSVC)
    target_compile_options(tests PRIVATE /W4 /WX)
else()
    target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(gtest_disable_pthread ON)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-D__USE_MINGW_ANSI_STDIO=1)
endif()

include(GoogleTest)
gtest_discover_tests(tests)