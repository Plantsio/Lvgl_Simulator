//
// Created by Gordon on 2021/8/13.
//

#include "UIBase.h"

#include <utility>
#include "GuiDefines.h"

namespace UI {
    Base::Base(ObjPtr obj) :
            m_scr(lv_obj_create(nullptr)),
            m_obj(std::move(obj)) {
        //mIndev.attachIndev(m_scr,[&](InputEvtType &input){return _handleInput(std::forward<InputEvtType>(input));});
    }

    Base::~Base() {
        if (lv_scr_act() != m_scr) {
            lv_obj_del(m_scr);
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


    bool Base::externalClose(Gui::GuiDismissCb &&cb) {
//        return GuiManager::instance().sendAction(Gui::ActionDismiss{
//                .uid=m_obj->getUID(),
//                .cb = cb ? std::move(cb) : []() {
//                    if (auto sharedPtr = GuiManager::instance().getRouter().lock()) {
//                        sharedPtr->haltAndResume();
//                    }
//                .cb = std::move(cb)
//        });
    }
}
