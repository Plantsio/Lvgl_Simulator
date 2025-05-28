//
// Created by Huwensong on 2025/5/28.
//

#include "FontTTF.h"
#include "filepath.h"
#include "log.h"

namespace UI
{
    FontTTF::FontTTF():
    Base(nullptr),
    label_30(lv_label_create(m_scr)),
    label_80(lv_label_create(m_scr))
    {}

    bool FontTTF::_initialize()
    {

        std::string fullPath = std::string("S:") + path({REPO_DIR,"ttf","LTProDefault.ttf"});
        log_d("fullPath = %s",fullPath.c_str());
        font_30 = lv_tiny_ttf_create_file(fullPath.c_str(),30);
        font_80 = lv_tiny_ttf_create_file(fullPath.c_str(),80);

        lv_obj_set_style_text_font(label_30,font_30,0);
        lv_obj_set_style_text_color(label_30,lv_color_white(),0);
        lv_obj_set_style_text_align(label_30,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_30,"这是(this) #30 字体");
        lv_obj_align(label_30,LV_ALIGN_TOP_MID,0,0);

        lv_obj_set_style_text_font(label_80,font_80,0);
        lv_obj_set_style_text_color(label_80,lv_color_white(),0);
        lv_obj_set_style_text_align(label_80,LV_TEXT_ALIGN_CENTER,0);
        lv_label_set_text(label_80,"80号字体：&*￥%@）￥（%");
        lv_obj_align(label_80,LV_ALIGN_BOTTOM_MID,0,0);
        return true;
    }
}
