//
// Created by Gordon on 2022/11/25.
//

#ifndef PLANTSIO_IVY_UI__H
#define PLANTSIO_IVY_UI__H

#include "UIBase.h"
#include "tools.h"

#include "Progress.h"
#include "UISysInfo.h"
#include "UIFluid.h"
#include "UIFluidAssist.h"
#include "DoubleProgress.h"
#include "UIActionBox.h"
#include "UIQrCode.h"
#include "UILoading.h"
#include "UIPlantStatus.h"
#include "UITest.h"
#include "UIPopUp.h"
#include "UIBoot.h"
#include "UITutorial.h"
#include "UITestError.h"
#include "UITestAging.h"
#include "UILangSelect.h"
#include "UITomatoClock.h"
#include "UIDate.h"
#include "UIWeather.h"
#include "UITime.h"
#include "UIProvTip.h"

namespace UI {

    typedef std::function<std::shared_ptr<UI::Base>()> UIBuilder;

    std::unordered_map<int, UIBuilder> &get_builders();

//    template<typename T>
//    UIBuilder create_ui_builder(Gui::GuiType id) {
//        return [id]() {
//            //std::lock_guard<std::recursive_timed_mutex> _lock(IvyAnim::instance().get_mutex());
//            auto ui = std::make_shared<T>();
//            ui->set_index(id);
//            return ui;
//        };
//    }
//
//    template<typename T>
//    void register_ui(Gui::GuiType id) {
//        get_builders().insert({id, create_ui_builder<T>(id)});
//    }

    template<typename T>
    std::shared_ptr<T> build_ui(int id) {
        auto builder = get_builders().find(id);
        if (builder != get_builders().end()) {
            auto ui = std::dynamic_pointer_cast<T>(builder->second());
            if (ui != nullptr) {
                return ui;
            }
        }
        return nullptr;
    }
}
#endif //PLANTSIO_IVY_UI__H
