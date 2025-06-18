//
// Created by Huwensong on 2025/6/13.
//

#include "RuntimeFont.h"
#include "lv_font_runtime.h"

namespace UI
{
    RuntimeFont::RuntimeFont():
    Base(nullptr),
    label_runtime(lv_label_create(m_scr)),
    label_font(lv_label_create(m_scr))
    {
    }

    bool RuntimeFont::_initialize()
    {
        lv_font_t *font = lv_bin_runtime_create("S:..\\resources\\cbin_16.cbin");
        lv_obj_set_style_text_font(label_runtime,font,0);
        lv_obj_set_style_text_color(label_runtime,lv_color_white(),0);
        lv_obj_set_style_text_align(label_runtime,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_runtime,"中文测试");
        lv_obj_align(label_runtime,LV_ALIGN_TOP_MID,0,0);

        lv_obj_set_style_text_font(label_font,&cbin_16,0);
        lv_obj_set_style_text_color(label_font,lv_color_white(),0);
        lv_obj_set_style_text_align(label_font,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_font,"this is a test");
        lv_obj_align(label_font,LV_ALIGN_BOTTOM_MID,0,0);

        return true;
    }
}