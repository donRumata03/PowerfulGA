cmake_minimum_required(VERSION 3.16)
project(GA)
project(GA_test)

set(CMAKE_CXX_STANDARD 20)

macro(add_debug_release_library target debug_path release_path)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        message(It`s\ Debug\ mode!\ =>\ Liking\ Debug\ static\ library\ versions...)
        target_link_libraries(${target} D:/pythonic/cmake-build-debug/pythonic.lib)
    else()
        if (CMAKE_BUILD_TYPE STREQUAL Release)
            message(It`s\ Release\ mode!\ =>\ Liking\ Release\ static\ library\ versions...)
            target_link_libraries(${target} D:/pythonic/cmake-build-release/pythonic.lib)
        else ()
            message(It`s\ an\ Unknown\ build\ mode!\ Can`t\ link\ the library!!!)
            exit()
        endif()

    endif()
endmacro()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

set(Pythonic_DIR D:/pythonic/)
find_package(Pythonic)

include_directories(.)
include_directories(${pythonic_include_directories})

add_library(GA
        GA/GA.cpp
        GA/GA.h
        GA/ga_util.cpp
        GA/ga_util.h
        other_optimization/local_optimization.cpp
        other_optimization/combi_optimizing.cpp other_optimization/combi_optimizing.h)

# add_debug_release_library(GA D:/pythonic/cmake-build-debug/pythonic.lib D:/pythonic/cmake-build-release/pythonic.lib)

target_link_libraries(GA ${pythonic_static_library})

add_executable(GA_test
        tests/ga_test.cpp
        tests/local_testing.h
        tests/test_functions.h
        tests/test_functions.cpp
)
# add_debug_release_library(GA_test D:/pythonic/cmake-build-debug/pythonic.lib D:/pythonic/cmake-build-release/pythonic.lib)
target_link_libraries(GA_test ${pythonic_static_library})
target_link_libraries(GA_test GA)

cotire(GA)
cotire(GA_test)
