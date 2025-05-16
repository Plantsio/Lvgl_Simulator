//
// Created by Huwensong on 2025/5/14.
//

#include "interface.h"
#include <cstdlib>

void *heap_caps_malloc(size_t size, char caps)
{
    return malloc(size);
}

void line_set_style(lv_obj_t *line, lv_coord_t width, lv_color_t color, bool is_rounded) {
    lv_obj_set_style_line_width(line, width, 0);
    lv_obj_set_style_line_color(line, color, 0);
    lv_obj_set_style_line_rounded(line, is_rounded, 0);
}
void label_set_style(lv_obj_t *label, const lv_font_t *font, lv_color_t color, lv_text_align_t align) {
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, color, 0);
    lv_obj_set_style_text_align(label, align, 0);
}

void bar_set_default_style(lv_obj_t *bar) {
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
}

namespace Sys
{
    void shutdown(){}
}
