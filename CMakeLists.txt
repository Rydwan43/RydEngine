cmake_minimum_required(VERSION 3.10)
project(RydEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

file(COPY ${PROJECT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})

# Add shared include paths for all subprojects
include_directories(
    ${PROJECT_SOURCE_DIR}/vendor/entt
    ${PROJECT_SOURCE_DIR}/vendor/imgui
    ${PROJECT_SOURCE_DIR}/vendor/rlImGui
)

file(GLOB_RECURSE ENGINE_HEADERS
    ${PROJECT_SOURCE_DIR}/Engine/*.h
    ${PROJECT_SOURCE_DIR}/Engine/*.hpp
)

# Add subprojects
add_subdirectory(Editor)
add_subdirectory(Game)