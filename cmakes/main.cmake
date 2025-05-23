cmake_minimum_required(VERSION 3.10)

include(${CMAKE_CURRENT_LIST_DIR}/simulator.cmake)

set(MAIN_PATH  ${PROJECT_SOURCE_DIR}/main)

find_package(SDL2 REQUIRED)

add_executable(main ${MAIN_PATH}/main.c  ${SIM_SOURCES})
message(${SIM_INCLUDES})
target_include_directories(main  PRIVATE  ${SDL2_INCLUDE_DIRS}  ${SIM_INCLUDES})
target_link_libraries(main PRIVATE SDL2::SDL2 )
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)

add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)