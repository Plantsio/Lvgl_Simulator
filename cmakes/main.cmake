cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/simulator.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/gui.cmake)

set(MAIN_PATH  ${PROJECT_SOURCE_DIR}/main)

find_package(SDL2 REQUIRED)

add_executable(main ${MAIN_PATH}/main.cpp  ${SIM_SOURCES} ${GUI_SOURCES})

target_include_directories(main  PRIVATE  ${SDL2_INCLUDE_DIRS}  ${SIM_INCLUDES} ${GUI_INCLUDES})
target_link_libraries(main PRIVATE SDL2::SDL2 )
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)

add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)