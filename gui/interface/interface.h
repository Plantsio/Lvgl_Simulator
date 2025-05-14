//
// Created by Huwensong on 2025/5/14.
//

#ifndef SIMULATOR_INTERFACE_H
#define SIMULATOR_INTERFACE_H

#include <cstdio>

#include "lvgl.h"

#define MALLOC_CAP_SPIRAM 0
#define MALLOC_CAP_8BIT   0

void *heap_caps_malloc(size_t size, char caps);

void line_set_style(lv_obj_t *line, lv_coord_t width, lv_color_t color, bool is_rounded = true);

void label_set_style(lv_obj_t *label, const lv_font_t *font, lv_color_t color = lv_color_white(),lv_text_align_t align = LV_TEXT_ALIGN_CENTER);

void bar_set_default_style(lv_obj_t *bar);

#endif //SIMULATOR_INTERFACE_H
