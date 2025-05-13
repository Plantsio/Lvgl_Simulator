cmake_minimum_required(VERSION 3.10)

set(GUI_PATH  ${PROJECT_SOURCE_DIR}/gui)

list(APPEND GUI_INCLUDES  ${GUI_PATH}/components ${GUI_PATH}/theme ${GUI_PATH}/ui)

file(GLOB_RECURSE GUI_SOURCES  ${GUI_PATH}/components/*.cpp  ${GUI_PATH}/theme/*.cpp  ${GUI_PATH}/ui/*.cpp)