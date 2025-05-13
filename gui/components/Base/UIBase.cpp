//
// Created by Gordon on 2021/8/13.
//

#include "UIBase.h"

#include <utility>
#include "GuiManager.h"

namespace UI {
    Base::Base(ObjPtr obj) :
            m_scr(lv_obj_create(nullptr)),
            m_obj(std::move(obj)) {
        lv_obj_set_style_bg_color(m_scr, lv_color_black(), 0);
    }

    Base::~Base() {
        if (lv_scr_act() != m_scr) {
            lv_obj_del(m_scr);
        }
    }

    void Base::enable_input(uint32_t input) {
        m_input_mask = input;
    }

    void Base::disable_input(uint32_t input) {
        m_input_mask &= ~input;
    }

    void Base::touch_input_handler(InputSource source, InputData data) {
        /* fixme */
        switch (m_input_mask & (1 << source)) {
            case INPUT_SRC_MASK(BtnSrcLeft):
                left_input_cb(data);
                break;
            case INPUT_SRC_MASK(BtnSrcRight):
                right_input_cb(data);
                break;
            case INPUT_SRC_MASK(BtnSrcFront):
                front_input_cb(data);
                break;
            default:
                break;
        }
    }

    bool Base::load() {
        if (initialize()) {
            lv_scr_load(m_scr);
            return true;
        }
        return false;
    }

    void Base::dismiss() {
        deInitialize();
    }

    uint32_t Base::get_input_mask() const {
        return m_input_mask;
    }

    bool Base::externalClose(Gui::GuiDismissCb &&cb) {
        return GuiManager::instance().sendAction(Gui::ActionDismiss{
                .uid=m_obj->getUID(),
//                .cb = cb ? std::move(cb) : []() {
//                    if (auto sharedPtr = GuiManager::instance().getRouter().lock()) {
//                        sharedPtr->haltAndResume();
//                    }
                .cb = std::move(cb)
        });
    }
}
