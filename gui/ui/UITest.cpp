//
// Created by sungaoran on 2022/4/30.
//

#include "UITest.h"
#include "interface.h"
#include "log.h"
#include "ThemeInterface.h"

namespace UI {
    UITest::UITest(ObjPtr obj) : Base(std::move(obj)) {
        //m_input_used = true;
        m_step_label = lv_label_create(m_scr);
        m_value_label = lv_label_create(m_scr);
        m_desc_label = lv_label_create(m_scr);
        m_progress_bar = lv_bar_create(m_scr);


        THEME_SET_FONT_SIZE(m_step_label, 30);
        THEME_SET_FONT_SIZE(m_value_label, 40);
        THEME_SET_FONT_SIZE(m_desc_label, 16);
        lv_label_set_recolor(m_desc_label, true);

        bar_set_default_style(m_progress_bar);
        lv_obj_set_size(m_progress_bar, 200, 20);

        lv_obj_align(m_step_label, LV_ALIGN_TOP_MID, 0, 20);
        lv_obj_align(m_value_label, LV_ALIGN_TOP_MID, 0, 70);
        lv_obj_align(m_desc_label, LV_ALIGN_CENTER, 0, 60);
        lv_obj_align(m_progress_bar, LV_ALIGN_CENTER, 0, 20);
        lv_label_set_text(m_value_label, "");
    }

    void UITest::update_step(const char *step) {
        lv_label_set_text(m_step_label, step);
    }

    void UITest::update_desc(const char *desc) {
        lv_label_set_text(m_desc_label, desc);
    }

    void UITest::update_value(const char *value) {
        lv_label_set_text(m_value_label, value);
    }

    void UITest::update_progress(int progress, int anim_t) {
        if (anim_t) {
            lv_obj_set_style_anim_time(m_progress_bar, anim_t, 0);
            lv_bar_set_value(m_progress_bar, progress, LV_ANIM_ON);
            auto *bar = (lv_bar_t *) m_progress_bar;
            bar->cur_value_anim.anim_start = 0;
        } else {
            lv_bar_set_value(m_progress_bar, progress, LV_ANIM_OFF);
        }
    }

    void UITest::reset_progress() {
        auto *bar = (lv_bar_t *) m_progress_bar;
        bar->cur_value = 0;
    }

    void UITest::update_color(lv_color_t back_color, lv_color_t text_color) {
        lv_obj_set_style_bg_color(m_scr, back_color, 0);
        lv_obj_set_style_text_color(m_step_label, text_color, 0);
        lv_obj_set_style_text_color(m_value_label, text_color, 0);
        lv_obj_set_style_text_color(m_desc_label, text_color, 0);
    }

    void UITest::update(int progress, const char *step, const char *desc, lv_color_t color) {
        lv_bar_set_value(m_progress_bar, progress, LV_ANIM_OFF);
        lv_label_set_text(m_step_label, step);
        lv_label_set_text(m_desc_label, desc);
        lv_obj_set_style_bg_color(m_scr, color, 0);
    }

    bool UITest::_handleInput(InputEvtType &&input) {
        log_d("ui-test btn  %d | %d | %d", input.index, input.data.inputType, input.data.n);
        return true;
    }
}

