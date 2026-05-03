cmake_minimum_required(VERSION 3.16)
project(granite LANGUAGES C CXX)

# ====================================================
# Configuration
# ====================================================
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# ====================================================
# Dependencies
# ====================================================
find_package(PkgConfig REQUIRED)
pkg_check_modules(BULLET REQUIRED bullet)

# ====================================================
# Directories
# ====================================================
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    SYSTEM ${CMAKE_SOURCE_DIR}/third-party/include
    ${BULLET_INCLUDE_DIRS}
)

add_definitions(-DGLFW_INCLUDE_NONE)

# ====================================================
# Sources (your code only)
# ====================================================
file(GLOB_RECURSE SRC_CPP ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE SRC_C   ${SRC_DIR}/*.c)

# ====================================================
# Third-party sources (ONLY what must compile)
# ====================================================
set(THIRD_CPP
    ${CMAKE_SOURCE_DIR}/third-party/src/tiny_obj_loader/tiny_obj_loader.cpp
)

set(THIRD_C
    ${CMAKE_SOURCE_DIR}/third-party/src/glad/glad.c
    ${CMAKE_SOURCE_DIR}/third-party/src/stb_image/stb_image.c
)

# ====================================================
# Static Library
# ====================================================
add_library(granite STATIC
    ${SRC_CPP}
    ${SRC_C}
    ${THIRD_CPP}
    ${THIRD_C}
)

# ====================================================
# Flags
# ====================================================
target_compile_options(granite PRIVATE
    -Wall -Wextra
)

# Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(granite PRIVATE
        -O0 -g3
        -fno-omit-frame-pointer
        -D_GLIBCXX_ASSERTIONS
    )
endif()

# Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(granite PRIVATE
        -O3 -DNDEBUG
    )
endif()

# CI
if(CMAKE_BUILD_TYPE STREQUAL "CI")
    target_compile_options(granite PRIVATE
        -Wall -Wextra -Wpedantic -Werror
        -Wconversion -Wsign-conversion -Wshadow
        -Wformat=2 -Wundef
        -fsanitize=address,undefined
        -fno-omit-frame-pointer -g3
    )
endif()

# ====================================================
# Link
# ====================================================
target_link_libraries(granite
    ${BULLET_LIBRARIES}
)

# ====================================================
# Install library
# ====================================================
install(TARGETS granite
    EXPORT graniteTargets
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

# ====================================================
# Install headers
# ====================================================
install(DIRECTORY include/
    DESTINATION include
)

# ====================================================
# pkg-config
# ====================================================
install(FILES granite.pc
    DESTINATION lib/pkgconfig
)
