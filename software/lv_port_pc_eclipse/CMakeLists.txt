cmake_minimum_required(VERSION 3.10)
project(lvgl)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
include_directories(smart-knob)
include_directories(smart-knob/Basic)
include_directories(smart-knob/GUI)
include_directories(smart-knob/lib/PageManager)
include_directories(smart-knob/Utils)
include_directories(smart-knob/Page)
include_directories(smart-knob/Image)
include_directories(smart-knob/Font)

file(GLOB_RECURSE INCLUDES "./*.h" )
file(GLOB_RECURSE SOURCES
        "smart-knob/Image/*.*"
        "smart-knob/Font/*.*"
        "smart-knob/Basic/*.*"
        "smart-knob/GUI/*.*"
        "smart-knob/lib/PageManager/*.*"
        "smart-knob/Utils/lv_ext/*.*"
        "smart-knob/Page/*.*"
        )

add_subdirectory(lvgl)
add_subdirectory(lv_drivers)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")

find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(main main.cpp mouse_cursor_icon.c ${SOURCES} ${INCLUDES})
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main PRIVATE lvgl lvgl::examples lvgl::demos lvgl::drivers ${SDL2_LIBRARIES})
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)