//
// Created by Gordon on 2022/3/7.
//

#include "UILoading.h"
#include "tools.h"
#include "ThemeInterface.h"

namespace UI
{
    UILoading::UILoading(ObjPtr obj) : Base(std::move(obj))
    {
        m_title_label = lv_label_create(m_scr);

		THEME_SET_FONT_SIZE(m_title_label,30);
        lv_obj_set_width(m_title_label, 280);

        m_loading_spinner = lv_spinner_create(m_scr,1000, 60);
        lv_obj_set_style_arc_color(m_loading_spinner, lv_color_make(52, 192, 0), LV_PART_INDICATOR);
        lv_obj_set_size(m_loading_spinner, 60, 60);
        lv_obj_set_style_arc_width(m_loading_spinner, 3, 0);
        lv_obj_set_style_arc_width(m_loading_spinner, 3, LV_PART_INDICATOR);

        m_desc_label = lv_label_create(m_scr);
		THEME_SET_FONT_SIZE(m_desc_label,16);
        lv_obj_set_width(m_desc_label, 280);
        lv_label_set_recolor(m_desc_label, true);

        lv_obj_align(m_title_label, LV_ALIGN_CENTER, 0, -70);
        lv_obj_align_to(m_loading_spinner, m_title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
        lv_obj_align_to(m_desc_label, m_loading_spinner, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);
    }

    void UILoading::update(const char *title, const char *desc)
    {
        lv_label_set_text(m_title_label, title);
        update_desc(desc);
    }

    void UILoading::update_desc(const char *desc)
    {
        lv_label_set_text(m_desc_label, desc);
    }

}