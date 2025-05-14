cmake_minimum_required(VERSION 3.10)

set(GUI_PATH  ${PROJECT_SOURCE_DIR}/gui)

list(APPEND GUI_INCLUDES
        ${GUI_PATH}
        ${GUI_PATH}/components/Animation
        ${GUI_PATH}/components/Base
        ${GUI_PATH}/components/Image
        ${GUI_PATH}/components/Indicator
        ${GUI_PATH}/components/ProgressBar
        ${GUI_PATH}/components/Text
        ${GUI_PATH}/miscellaneous
        ${GUI_PATH}/interface
        ${GUI_PATH}/theme
        ${GUI_PATH}/ui)

file(GLOB_RECURSE GUI_SOURCES  ${GUI_PATH}/*.cpp ${GUI_PATH}/components/*.cpp  ${GUI_PATH}/theme/*.cpp  ${GUI_PATH}/ui/*.cpp)