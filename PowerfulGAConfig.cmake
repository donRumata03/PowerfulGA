# Defines the following variables:
#
# PowerfulGA_static_library <- contains the path to the static library.
# PowerfulGA_include_directories <- contains a list of directories which you should include in order to use PowerfulGA.

set(actual_GA_dir "")
set(GA_dir_provided OFF)


set(possible_GA_dir_names

        PowerfulGA_DIR
        GA_DIR

        PowerfulGA_Dir
        GA_Dir

        PowerfulGA_dir
        GA_dir

        powerfulGA_dir
        ga_dir
        )

foreach(current_dir_name_variation ${possible_GA_dir_names})

    if (EXISTS ${${current_dir_name_variation}})

        set(GA_dir_provided ON)
        set(actual_GA_dir ${${current_dir_name_variation}})

    endif()

endforeach()

if(NOT ${GA_dir_provided})
    message(FATAL_ERROR "You didn`t provide a directory variable with any of supported names (${possible_GA_dir_names}) or the direcory you provided as pythonic_dir doesn`t actually exist!")
endif()



set(GA_release_build_dir ${actual_GA_dir}/cmake-build-release)
set(GA_debug_build_dir ${actual_GA_dir}/cmake-build-debug)

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(GA_build_dir ${GA_release_build_dir})
elseif(${CMAKE_BUILD_TYPE} STREQUAL Debug) # It`s Debug!
    set(GA_build_dir ${GA_debug_build_dir})
else()
    message(FATAL_ERROR "Bad CMAKE_BUILD_TYPE (${CMAKE_BUILD_TYPE}). It should be either Release or Debug")
endif()

# Choose static library name based on operation system:
if(UNIX)
    set(PGA_static_library_name libGA.a)
endif()
if(WIN32)
    set(PGA_static_library_name GA.lib)
endif()

set(deducted_PGA_lib_location "${GA_build_dir}/${PGA_static_library_name}")

if (EXISTS ${deducted_PGA_lib_location})
    set(PowerfulGA_static_libraries ${deducted_PGA_lib_location})
    message(STATUS "Found PowerfulGA static library here: ${PowerfulGA_static_libraries}")
else()
    message(FATAL_ERROR "Can`t find PowerfulGA static library file in corresponding directory!!! (${deducted_PGA_lib_location})")
endif()

set(PowerfulGA_include_directories ${actual_GA_dir})


