cmake_minimum_required(VERSION 3.27)
project(untitled C)
add_custom_target(mytarget ALL
        COMMAND mingw32-make --no-print-directory -f ${CMAKE_SOURCE_DIR}/Makefile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set(CMAKE_C_STANDARD 17)

add_executable(untitled
        src/main.c
        src/parser.c
        src/view.c)
