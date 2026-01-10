cmake_minimum_required(VERSION 3.10)

project(notepad)

add_subdirectory(src/ui)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(src/ui-tests)
endif()
