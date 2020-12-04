cmake_minimum_required(VERSION 3.16)

if (UNIX)
    message("UNIX => Setting Compiler to gcc-10")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++-10")
    set(CMAKE_C_COMPILER "/usr/bin/gcc-10")
endif()


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

if (WIN32)
    set(Pythonic_DIR D:/pythonic)
else()
    set(Pythonic_DIR "$ENV{HOME}/dev/pythonic")
endif()

find_package(Pythonic)

include_directories(.)
include_directories(${pythonic_include_directories})

add_library(GA
        GA/ga_base.h

        GA/operations/mutation.cpp
        GA/operations/mutation.h
        GA/operations/matting.cpp
        GA/operations/matting.h
        GA/operations/population_generation.cpp
        GA/operations/population_generation.h
        GA/operations/util.cpp
        GA/operations/util.h
        GA/operations/crossover.cpp
        GA/operations/crossover.h


        GA/old_GA.cpp
        GA/old_GA.h

        GA/GA_optimizer.cpp
        GA/GA_optimizer.h

        performance_testing/performance_distribution_maker.cpp
        performance_testing/performance_distribution_maker.h

        other_optimization/local_optimization.cpp
        other_optimization/combi_optimizing.cpp
        other_optimization/combi_optimizing.h

        GA/base_GA_params.h
        GA/operations/operations.h
        Annealing/annealing_optimize.cpp
        Annealing/annealing_optimize.h
        Annealing/default_annealing_operations.h
        Annealing/default_annealing_operations.cpp

        lib/libfort/fort.c
        lib/libfort/fort.h
        lib/libfort/fort.hpp
        )

# add_debug_release_library(GA D:/pythonic/cmake-build-debug/pythonic.lib D:/pythonic/cmake-build-release/pythonic.lib)

target_link_libraries(GA ${pythonic_static_libraries})

add_executable(GA_test
        main.cpp

        tests/local_testing.h
        tests/test_functions.h
        tests/test_functions.cpp
        tests/ga_util_tests.h
        tests/lib_test.cpp
        tests/quantity_tests.h
        tests/old_GA_test.h

        tests/ga_test.h
        tests/annealing_tests.h

        problem_examples/chess_annealing.cpp
        problem_examples/chess_annealing.h


        )
# add_debug_release_library(GA_test D:/pythonic/cmake-build-debug/pythonic.lib D:/pythonic/cmake-build-release/pythonic.lib)
target_link_libraries(GA_test ${pythonic_static_libraries})
target_link_libraries(GA_test GA)

cotire(GA)
cotire(GA_test)
