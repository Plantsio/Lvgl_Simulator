//
// Created by Gordon on 2022/11/4.
//

#include "UITestError.h"
#include "tools.h"

namespace UI {
    static void draw_cb(lv_event_t *e) {
//        auto *dsc = (lv_obj_draw_part_dsc_t *) lv_event_get_param(e);
//        if (dsc->part == LV_PART_ITEMS) {
//            dsc->label_dsc->font = &ba_16;
//            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
//            dsc->label_dsc->color = lv_color_make(0, 255, 0);
//            dsc->rect_dsc->border_color = lv_color_make(255, 255, 255);
//            dsc->rect_dsc->border_side = LV_BORDER_SIDE_FULL;
//        }
    }

    UITestError::UITestError(ObjPtr obj)
            : Base(std::move(obj)),
              m_title_label(lv_label_create(m_scr)),
              m_error_table(lv_table_create(m_scr)) {
        label_set_style(m_title_label, &ba_30);
        lv_obj_align(m_title_label, LV_ALIGN_TOP_MID, 0, 10);

        lv_obj_set_style_text_font(m_error_table, &ba_16, 0);
        lv_obj_set_style_text_color(m_error_table, lv_color_white(), 0);
        lv_obj_align(m_error_table, LV_ALIGN_CENTER, 0, 12);
        lv_obj_clear_flag(m_error_table, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_opa(m_error_table, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(m_error_table, LV_OPA_TRANSP, 0);
        lv_obj_set_style_pad_ver(m_error_table, 5, LV_PART_ITEMS);
        lv_obj_set_style_bg_opa(m_error_table, LV_OPA_TRANSP, LV_PART_ITEMS);
        //lv_obj_add_event_cb(m_error_table, draw_cb, LV_EVENT_DRAW_PART_BEGIN, nullptr);

        lv_obj_set_style_bg_color(m_scr, lv_color_make(255, 0, 0), 0);
    }

    void UITestError::set_errors(std::vector<int> &list) {
        std::string err_str = std::to_string(list.size()) + " errors!";
        lv_label_set_text(m_title_label, err_str.c_str());
        int cnt = 0;
        for (auto err: list) {
            lv_table_set_cell_value(m_error_table, cnt % 6, cnt / 6, std::to_string(err).c_str());
            cnt++;
        }
    }
}