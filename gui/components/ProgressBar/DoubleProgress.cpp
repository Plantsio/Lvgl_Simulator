//
// Created by sungaoran on 2021/11/16.
//

#include "DoubleProgress.h"
#include "string"
#include "tools.h"

#include "Lang.h"
#include "ThemeInterface.h"

namespace UI
{
    DoubleProgress::DoubleProgress(lv_obj_t *_parent)
    {
        m_bar_1 = lv_bar_create(_parent);
        bar_set_default_style(m_bar_1);

        m_bar_2 = lv_bar_create(_parent);
        bar_set_default_style(m_bar_2);

        lv_obj_set_size(m_bar_1, 200, 20);
        lv_obj_align(m_bar_1, LV_ALIGN_CENTER, 0, -20);

        lv_obj_set_size(m_bar_2, 200, 20);
        lv_obj_align_to(m_bar_2, m_bar_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

        m_title_label = lv_label_create(_parent);
		THEME_SET_FONT_SIZE(m_title_label,30);

        lv_obj_align_to(m_title_label, m_bar_1, LV_ALIGN_OUT_TOP_MID, 0, -20);

        m_status_label_1 = lv_label_create(_parent);
        lv_obj_set_width(m_status_label_1, 300);
        lv_label_set_recolor(m_status_label_1,true);
		THEME_SET_FONT_SIZE(m_status_label_1,16);
        lv_obj_align_to(m_status_label_1, m_bar_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        m_status_label_2 = lv_label_create(_parent);
		THEME_SET_FONT_SIZE(m_status_label_2,16);
        lv_obj_align_to(m_status_label_2, m_bar_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

        m_desc_label = lv_label_create(_parent);
        lv_obj_set_width(m_desc_label, 280);
		THEME_SET_FONT_SIZE(m_desc_label,16);
        lv_label_set_recolor(m_desc_label, true);
        lv_obj_align_to(m_desc_label, m_status_label_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    }


    void DoubleProgress::update(const char *title, const char *status_1, uint8_t progress_1, const char *status_2,
								uint8_t progress_2)
    {
        if (std::string(title) != lv_label_get_text(m_title_label))
        {
            /* update title */
            lv_label_set_text(m_title_label, title);
            lv_obj_align_to(m_title_label, m_bar_1, LV_ALIGN_OUT_TOP_MID, 0, -20);

        }
        update_progress_1(status_1, progress_1);
        update_progress_2(status_2, progress_2);
    }

    void DoubleProgress::update_progress_1(const char *status_1, uint8_t progress_1)
    {
        if (std::string(status_1) != lv_label_get_text(m_status_label_1))
        {
            /* update status */
            lv_label_set_text(m_status_label_1, status_1);
            lv_obj_align_to(m_status_label_1, m_bar_1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        }
        lv_bar_set_value(m_bar_1, progress_1, LV_ANIM_OFF);
    }

    void DoubleProgress::update_progress_2(const char *status_2, uint8_t progress_2)
    {
        if (std::string(status_2) != lv_label_get_text(m_status_label_2))
        {
            /* update status */
            lv_label_set_text(m_status_label_2, status_2);
            lv_obj_align_to(m_status_label_2, m_bar_2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        }
        lv_bar_set_value(m_bar_2, progress_2, LV_ANIM_OFF);
    }

    void DoubleProgress::update_desc(const char *desc)
    {
        lv_label_set_text(m_desc_label, desc);
    }
}