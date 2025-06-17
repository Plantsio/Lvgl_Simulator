//
// Created by Huwensong on 2025/6/11.
//

#ifndef SIMULATOR_LV_FONT_RUNTIME_H
#define SIMULATOR_LV_FONT_RUNTIME_H
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

lv_font_t *lv_bin_runtime_create(const char* path);

void lv_bin_runtime_destroy(lv_font_t *font);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //SIMULATOR_LV_FONT_RUNTIME_H