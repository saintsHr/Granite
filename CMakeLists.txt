cmake_minimum_required(VERSION 3.16)

project(granite VERSION 1.0.0 LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX "d")

# -----------------------------
# Dependencies
# -----------------------------
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Bullet REQUIRED)
find_package(nlohmann_json REQUIRED)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(OPENAL REQUIRED openal)

# -----------------------------
# Sources
# -----------------------------
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(THIRD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third-party/src)

file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)
file(GLOB_RECURSE SRC_C   CONFIGURE_DEPENDS ${SRC_DIR}/*.c)

file(GLOB_RECURSE THIRD_CPP CONFIGURE_DEPENDS ${THIRD_SRC}/*.cpp)
file(GLOB_RECURSE THIRD_C   CONFIGURE_DEPENDS ${THIRD_SRC}/*.c)

# -----------------------------
# Library
# -----------------------------
add_library(granite STATIC
    ${SRC_CPP}
    ${SRC_C}
    ${THIRD_CPP}
    ${THIRD_C}
)

add_library(granite::granite ALIAS granite)

# -----------------------------
# Compile Options
# -----------------------------
target_compile_options(granite PRIVATE
    -Wall
    -Wextra
    -Werror

    $<$<CONFIG:Debug>:
        -O0
        -g3
        -fno-omit-frame-pointer
        -D_GLIBCXX_ASSERTIONS
    >

    $<$<CONFIG:Release>:
        -O3
        -DNDEBUG
    >
)

# -----------------------------
# Includes
# -----------------------------
target_include_directories(granite
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/include
        ${BULLET_INCLUDE_DIRS}
        ${SNDFILE_INCLUDE_DIRS}
)

# -----------------------------
# Defines
# -----------------------------
target_compile_definitions(granite PRIVATE GLFW_INCLUDE_NONE)

# -----------------------------
# Link Libraries
# -----------------------------
target_link_libraries(granite
    PRIVATE
        glfw
        OpenGL::GL
        glm::glm
        nlohmann_json::nlohmann_json
        ${BULLET_LIBRARIES}
        ${SNDFILE_LIBRARIES}
        ${OPENAL_LIBRARIES}
        $<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS}>
        $<$<PLATFORM_ID:Linux>:${CMAKE_THREAD_LIBS_INIT}>
)

# -----------------------------
# Install
# -----------------------------
install(TARGETS granite
    EXPORT graniteTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES granite.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

install(EXPORT graniteTargets
    FILE graniteTargets.cmake
    NAMESPACE granite::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/granite
)

# -----------------------------
# Package Configuration
# -----------------------------
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/graniteConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/graniteConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/granite
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/graniteConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/graniteConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/graniteConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/granite
)
