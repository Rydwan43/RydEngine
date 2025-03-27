cmake_minimum_required(VERSION 3.10)
project(RydEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib
find_package(raylib REQUIRED)

# Add ImGui and rlImGui source files
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)
set(RLIMGUI_DIR ${PROJECT_SOURCE_DIR}/vendor/rlImGui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${RLIMGUI_DIR}/rlImGui.cpp
)

# Add source files
add_executable(RydEngine 
    src/RydEngine/main.cpp
    ${IMGUI_SOURCES}
)

# Include directories
target_include_directories(RydEngine PRIVATE 
    ${PROJECT_SOURCE_DIR}/src/RydEngine
    ${IMGUI_DIR}
    ${RLIMGUI_DIR}
)

# Link libraries
target_link_libraries(RydEngine PRIVATE 
    raylib
)

# Copy assets to build directory
file(COPY ${PROJECT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR}) 