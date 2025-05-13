//
// Created by sungaoran on 2021/11/16.
//

#include "Progress.h"

#include "ThemeInterface.h"

namespace UI
{
    Progress::Progress(lv_obj_t *_parent)
    {
        m_bar = lv_bar_create(_parent);

        static lv_style_t style_bar_bg;
        lv_style_init(&style_bar_bg);
        lv_style_set_border_color(&style_bar_bg, Theme::getPaletteRGB(Theme::palette_success));
        lv_style_set_border_width(&style_bar_bg, 2);
        lv_style_set_pad_all(&style_bar_bg, 6); /*To make the indicator smaller*/
        lv_style_set_radius(&style_bar_bg, 6);
        lv_style_set_anim_time(&style_bar_bg, 1000);

        static lv_style_t style_bar_indic;
        lv_style_set_bg_opa(&style_bar_indic, LV_OPA_COVER);
        lv_style_set_bg_color(&style_bar_indic, Theme::getPaletteRGB(Theme::palette_success));
        lv_style_set_radius(&style_bar_indic, 3);

        lv_obj_add_style(m_bar, &style_bar_bg, 0);
        lv_obj_add_style(m_bar, &style_bar_indic, LV_PART_INDICATOR);

        lv_obj_set_size(m_bar, 200, 20);
        lv_obj_center(m_bar);

        m_title_label = lv_label_create(_parent);
        lv_obj_set_width(m_title_label, 200);
		THEME_SET_FONT_SIZE(m_title_label,30);
        lv_obj_align_to(m_title_label, m_bar, LV_ALIGN_OUT_TOP_MID, 0, -30);
        lv_obj_set_style_text_align(m_title_label,LV_TEXT_ALIGN_CENTER,0);

        m_status_label = lv_label_create(_parent);
        lv_obj_set_width(m_status_label, 240);
		THEME_SET_FONT_SIZE(m_status_label,16);
        lv_obj_align_to(m_status_label, m_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        lv_label_set_recolor(m_status_label, true);
        lv_obj_set_style_text_align(m_status_label,LV_TEXT_ALIGN_CENTER,0);

        m_desc_label = lv_label_create(_parent);
        lv_obj_set_width(m_desc_label, 280);
		THEME_SET_FONT_SIZE(m_desc_label,16);
        lv_label_set_recolor(m_desc_label, true);
        lv_obj_align_to(m_desc_label, m_status_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
        lv_label_set_text(m_desc_label, "");
        lv_obj_set_style_text_align(m_desc_label,LV_TEXT_ALIGN_CENTER,0);

    }

    void Progress::update_title(const std::string &value)
    {
        lv_label_set_text(m_title_label, value.c_str());
    }

    void Progress::update_status(const std::string &value)
    {
        lv_label_set_text(m_status_label, value.c_str());
    }

    void Progress::update_progress(uint8_t progress)
    {
        lv_bar_set_value(m_bar, progress, LV_ANIM_OFF);
    }

    void Progress::update_desc(const std::string &desc)
    {
        lv_label_set_text(m_desc_label, desc.c_str());
    }

    void Progress::update(const std::string &status, uint8_t progress)
    {
        if (std::string(status) != lv_label_get_text(m_status_label))
        {
            /* update status */
            update_status(status);
        }
        update_progress(progress);
    }

    void Progress::update(const std::string &title, const std::string &status, uint8_t progress)
    {
        if (std::string(title) != lv_label_get_text(m_title_label))
        {
            /* update title */
            update_title(title);

        }
        update(status, progress);
    }
}