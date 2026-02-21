cmake_minimum_required(VERSION 3.10)

project(notepadng)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Modules")

add_subdirectory(src/ui)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(src/ui-tests)
endif()
