cmake_minimum_required(VERSION 3.10)
project(campus_gui C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(campus_gui
    campus_gui.c
    core/services.c
)

target_include_directories(campus_gui PRIVATE 
    ${GTK4_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(campus_gui PRIVATE ${GTK4_LIBRARY_DIRS})
target_link_libraries(campus_gui PRIVATE ${GTK4_LIBRARIES})
target_compile_options(campus_gui PRIVATE ${GTK4_CFLAGS_OTHER})