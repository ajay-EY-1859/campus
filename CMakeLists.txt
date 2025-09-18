cmake_minimum_required(VERSION 3.25)
project(campus C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)



# Source files (include all core logic, admin, student, api, core)
file(GLOB SOURCES
    "src/main/*.c"
    "src/student/*.c"
    "src/admin/user/*.c"
    "src/api/*.c"
    "src/core/*.c"
)


# Create executable
add_executable(campus ${SOURCES})



# Include directories
target_include_directories(campus PRIVATE include)
target_include_directories(campus PRIVATE include/curl)

# Link local curl library
target_link_directories(campus PRIVATE lib)
target_link_libraries(campus PRIVATE libcurl)

# Windows-specific setup
if(WIN32)
    target_include_directories(campus PRIVATE include/hpdf)
    target_link_directories(campus PRIVATE lib lib/hpdf)
    target_link_libraries(campus PRIVATE hpdf)
    target_compile_definitions(campus PRIVATE _CRT_SECURE_NO_WARNINGS)
    
    # Copy hpdf.dll to output directory
    add_custom_command(TARGET campus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/hpdf/hpdf.dll"
        "$<TARGET_FILE_DIR:campus>"
    )
    # Copy libcurl-x64.dll to output directory
    add_custom_command(TARGET campus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/curl-8.16.0_3-win64-mingw/bin/libcurl-x64.dll"
        "$<TARGET_FILE_DIR:campus>"
    )
add_custom_command(TARGET campus POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/curl-8.16.0_3-win64-mingw/bin/curl-ca-bundle.crt"
        "$<TARGET_FILE_DIR:campus>"                   )
endif()

# Linux-specific setup  
if(UNIX)
    # Try to find libharu using pkg-config first
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(HPDF QUIET libharu)
        if(HPDF_FOUND)
            target_link_libraries(campus PRIVATE ${HPDF_LIBRARIES})
            target_include_directories(campus PRIVATE ${HPDF_INCLUDE_DIRS})
            target_compile_options(campus PRIVATE ${HPDF_CFLAGS_OTHER})
        else()
            # Fallback to system library
            target_link_libraries(campus PRIVATE hpdf)
        endif()
    else()
        # Direct linking as fallback
        target_link_libraries(campus PRIVATE hpdf)
    endif()
endif()

# Create required directories at build time
add_custom_target(create_dirs ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/credentials
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/logs
)
add_dependencies(campus create_dirs)