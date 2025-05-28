cmake_minimum_required(VERSION 3.10)

set(SIMULATOR_PATH  ${PROJECT_SOURCE_DIR}/simulator)

set(SIMULATOR_CONFIG_PATH   ${PROJECT_SOURCE_DIR}/config)

list(APPEND SIM_INCLUDES
        ${SIMULATOR_PATH}/lv_drivers
        ${SIMULATOR_PATH}/lvgl/demos
        ${SIMULATOR_PATH}/lvgl
        ${SIMULATOR_PATH}/lvgl/src
        ${SIMULATOR_CONFIG_PATH})

file(GLOB_RECURSE SIM_SOURCES
        ${SIMULATOR_PATH}/lv_drivers/*.c
        ${SIMULATOR_PATH}/lvgl/demos/*.c
        ${SIMULATOR_PATH}/lvgl/src/*.c
        ${SIMULATOR_PATH}/lvgl/src/*.cpp)

list(APPEND SIM_SOURCES
        ${SIMULATOR_PATH}/mouse_cursor_icon.c)