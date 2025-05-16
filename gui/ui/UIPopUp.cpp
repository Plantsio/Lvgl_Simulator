//
// Created by Huwensong on 2024/11/15.
//

#include "UIPopUp.h"

namespace UI {
    UIPopUp::UIPopUp(ObjPtr obj)
            : Base(std::move(obj)),
              m_text_main(m_scr),
              m_text_sub(m_scr) {

        m_text_main.set_font_size(30);
        m_text_sub.set_font_size(16);

        create_confirm_ui();

        //m_is_temp = true;
    }

    void UIPopUp::update(const char *main, const char *sub, bool anim_on) {
        update_main(main, anim_on);
        update_sub(sub, anim_on);
        realign();

//        auto a = anim_create(m_arc, anim_fade, LV_OPA_TRANSP, LV_OPA_COVER,
//                             500, 0, 1200, 1800,
//                             nullptr, nullptr, LV_ANIM_REPEAT_INFINITE);
//        lv_anim_start(&a);
    }

    void UIPopUp::update_main(const char *main, bool anim_on) {
        m_text_main.update(main, anim_on);
        realign();
    }

    void UIPopUp::update_sub(const char *sub, bool anim_on) {
        m_text_sub.update(sub, anim_on);
        realign();
    }

    void UIPopUp::realign() const {
        //lv_obj_align(m_text_main.get_origin_obj(), LV_ALIGN_CENTER, 0, -60);
        //lv_obj_align_to(m_text_sub.get_origin_obj(), m_text_main.get_origin_obj(), LV_ALIGN_OUT_BOTTOM_MID, 0, 40);
    }

    void UIPopUp::create_confirm_ui() {
        m_arc = lv_arc_create(m_scr);
        lv_obj_set_size(m_arc, 30, 30);
        lv_obj_set_style_arc_width(m_arc, 2, 0);
        lv_obj_set_style_arc_width(m_arc, 2, LV_PART_INDICATOR);
        lv_arc_set_rotation(m_arc, 0);
        lv_arc_set_value(m_arc, 100);
        lv_arc_set_bg_angles(m_arc, 0, 360);
        lv_obj_set_style_arc_color(m_arc, lv_color_black(), 0);
        lv_obj_set_style_arc_color(m_arc, lv_color_make(200, 200, 200), LV_PART_INDICATOR);
        lv_obj_remove_style(m_arc, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(m_arc, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_align(m_arc, LV_ALIGN_BOTTOM_MID, 0, -10);

        m_ok = lv_label_create(m_arc);
        lv_obj_set_style_text_color(m_ok, lv_color_make(245, 203, 38), 0);
        lv_obj_set_style_text_font(m_ok, &ba_16, 0);
        lv_label_set_text(m_ok, "OK");

        lv_obj_align_to(m_ok, m_arc, LV_ALIGN_CENTER, 0, 0);
    }
}