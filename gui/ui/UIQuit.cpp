//
// Created by Gordon on 2025/4/24.
//

#include "UIQuit.h"
#include "GuiDefines.h"
//#include "Store.h"

namespace UI {
    UI::UIQuit::UIQuit(UI::Base::ObjPtr obj) : Base(std::move(obj)) {
    }

    bool UIQuit::_initialize() {
//        return externalClose([]() {
//            VideoGroup group = {};
//            group.push(AnimInfo{"confused.a", 1});
////            group.push(AnimInfo{"confused.b", 3});
//            group.push(AnimInfo{"confused.c", 1});
//            GuiManager::instance().sendAction(
//                    Gui::ActionCreate{
//                            .guiType = Gui::EMO,
//                            .priority = 1,
//                            .data = group,
//                    }
//            );
//        });
        return true;
    }
}