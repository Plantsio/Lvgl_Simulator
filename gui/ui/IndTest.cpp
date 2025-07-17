//
// Created by sungaoran on 2025/7/17.
//

#include "IndTest.h"
#include "Indicators.h"
#include "lv_font_runtime.h"
#include "log.h"

namespace UI {
    UI::IndTest::IndTest() :
            Base(nullptr),
            m_label(lv_label_create(m_scr)),
            m_indicators(m_scr) {
    }

    bool UI::IndTest::_initialize() {
        lv_font_t *font = lv_bin_runtime_create("S:..\\resources\\lt_pro_normal.cbin");
        lv_obj_set_style_text_font(m_label, font, 0);
        lv_obj_set_style_text_color(m_label, lv_color_white(), 0);
        lv_obj_align(m_label, LV_ALIGN_CENTER, 0, 0);
        lv_label_set_text(m_label, "测试");


        lv_obj_add_event_cb(m_scr, test, LV_EVENT_KEY, this);
        return true;
    }

    void IndTest::test(lv_event_t *e) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_indev_t *indev = lv_event_get_indev(e);
        auto *ui = (UI::IndTest *) lv_event_get_user_data(e);  // Cast back to your UI class

        if (code == LV_EVENT_KEY) {
            uint32_t key = lv_indev_get_type(indev);
            log_d("%d",key);

            if (key == LV_KEY_LEFT) {
                ui->m_indicators.setState(Indicators::IndPartLeft, IndicatorBase::IndShow);
            } else if (key == LV_KEY_RIGHT) {
                ui->m_indicators.setState(Indicators::IndPartRight, IndicatorBase::IndShow);
            } else if (key == LV_KEY_UP) {
                ui->m_indicators.setState(Indicators::IndPartLeft, IndicatorBase::IndHidden);
            } else if (key == LV_KEY_DOWN) {
                ui->m_indicators.setState(Indicators::IndPartRight, IndicatorBase::IndHidden);
            } else if (key == 106) {    /* j*/
                ui->m_indicators.setState(Indicators::IndPartAll, IndicatorBase::IndFlash);
            }else if (key == 107) {    /* k*/
                ui->m_indicators.setState(Indicators::IndPartLeft, IndicatorBase::IndLoading);
            }else if (key == 108) {    /* l*/
                ui->m_indicators.setState(Indicators::IndPartRight, IndicatorBase::IndLoading);
            }
        }

    }
}