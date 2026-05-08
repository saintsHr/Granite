cmake_minimum_required(VERSION 3.16)
project(granite LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)

pkg_check_modules(SNDFILE REQUIRED sndfile)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(THIRD_SRC ${CMAKE_SOURCE_DIR}/third-party/src)

file(GLOB_RECURSE SRC_CPP ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE SRC_C   ${SRC_DIR}/*.c)

file(GLOB_RECURSE THIRD_CPP ${THIRD_SRC}/*.cpp)
file(GLOB_RECURSE THIRD_C   ${THIRD_SRC}/*.c)

add_library(granite STATIC
    ${SRC_CPP}
    ${SRC_C}
    ${THIRD_CPP}
    ${THIRD_C}
)

target_compile_options(granite PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Debug>:-O0 -g3 -fno-omit-frame-pointer -D_GLIBCXX_ASSERTIONS>
    $<$<CONFIG:Release>:-O3 -DNDEBUG>
)

target_include_directories(granite
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/third-party/include
        /usr/include/bullet
        ${SNDFILE_INCLUDE_DIRS}
)

target_compile_definitions(granite PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries(granite
    ${BULLET_LIBRARIES}
    glfw
    OpenGL::GL
    glm::glm
    ${SNDFILE_LIBRARIES}
)

install(TARGETS granite
    EXPORT graniteTargets
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(FILES granite.pc
    DESTINATION lib/pkgconfig
)
