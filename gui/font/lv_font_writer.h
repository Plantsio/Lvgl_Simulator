//
// Created by Huwensong on 2025/6/17.
//

#ifndef SIMULATOR_LV_FONT_WRITER_H
#define SIMULATOR_LV_FONT_WRITER_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

bool lv_font_write(const lv_font_t * font, const char *path);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //SIMULATOR_LV_FONT_WRITER_H
