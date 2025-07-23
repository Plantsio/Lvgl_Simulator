//
// Created by Huwensong on 2025/6/13.
//

#include "RuntimeFont.h"
#include "lv_font_runtime.h"

namespace UI
{
    RuntimeFont::RuntimeFont():
    Base(nullptr),
    label_runtime1(lv_label_create(m_scr)),
    label_runtime2(lv_label_create(m_scr)),
    label_font(lv_label_create(m_scr))
    {
    }

    bool RuntimeFont::_initialize()
    {
        lv_font_t *font15 = lv_bin_runtime_create("S:..\\resources\\font_15.cbin");
        lv_obj_set_style_text_font(label_runtime1,font15,0);
        lv_obj_set_style_text_color(label_runtime1,lv_color_white(),0);
        lv_obj_set_style_text_align(label_runtime1,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_runtime1,"15: hjsdjhfisdiufhisudfui");
        lv_obj_align(label_runtime1,LV_ALIGN_CENTER,0,-7);

        lv_font_t *font21 = lv_bin_runtime_create("S:..\\resources\\font_21.cbin");
        lv_obj_set_style_text_font(label_runtime2,font21,0);
        lv_obj_set_style_text_color(label_runtime2,lv_color_white(),0);
        lv_obj_set_style_text_align(label_runtime2,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_runtime2,"21: hjsdjhfisdiufhisudfui");
        lv_obj_align_to(label_runtime2,label_runtime1,LV_ALIGN_OUT_TOP_MID,0,0);

        lv_obj_set_style_text_font(label_font,&ba_16,0);
        lv_obj_set_style_text_color(label_font,lv_color_white(),0);
        lv_obj_set_style_text_align(label_font,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_font,"ba: hjsdjhfisdiufhisudfui");
        lv_obj_align_to(label_font,label_runtime1,LV_ALIGN_OUT_BOTTOM_MID,0,0);

        return true;
    }
}