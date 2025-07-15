//
// Created by Gordon on 2022/5/26.
//

#include "UIBoot.h"

#include <utility>
#include "Lang.h"
#include "ThemeInterface.h"
#include "log.h"
#include "SysTime.h"

namespace UI
{

    UIBoot::UIBoot(ObjPtr obj) : Base(std::move(obj)), mProcess(m_scr)
    {}

    bool UIBoot::_initialize()
    {
        auto commonTrue = [](){return true;};

        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_waking_up), "");

        //region connect
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_connect_to_net), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_connect_succ),
                     THEME_TEXT_CONTENT(Lang::ui_boot_connect_fail), 10);
        //endregion

        //region Online
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_activating), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_activated),
                     THEME_TEXT_CONTENT(Lang::ui_boot_activate_fail), 10);
        //endregion

        //region Tuning time
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_tuning_time), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_tune_succ),
                     THEME_TEXT_CONTENT(Lang::ui_boot_tune_fail), 10);
        //endregion

        //region locating
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_locating), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_locate_succ),
                     THEME_TEXT_CONTENT(Lang::ui_boot_locate_fail), 10);
        //endregion

        //region physical check
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_physical_check), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_physical_succ),
                     THEME_TEXT_CONTENT(Lang::ui_boot_physical_fail));
        //endregion

        //region plant detect
        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_plant_check), "");
        registerStep(commonTrue, 7000, THEME_TEXT_CONTENT(Lang::ui_boot_plant_checked),
                     THEME_TEXT_CONTENT(Lang::ui_boot_plant_fail));
        //endregion

        registerStep(commonTrue, 0, THEME_TEXT_CONTENT(Lang::ui_boot_waked_up), "");

        return true;
    }

    void UIBoot::update_now(const std::string &desc, int progress)
    {
        mProcess.update_status(desc);
        mProcess.update_progress(progress);
    }

    void UIBoot::registerStep(ConditionCb condition,uint32_t conditionTimeout,const std::string &successDsc,const std::string &failDsc,int failToJump)
    {
        mStepHandler.registerStepUnit([&](uint32_t &timeout)
                                      {
                                          timeout = conditionTimeout;
                                          return condition();
                                      },
                                      [&]()
                                      {
                                          update_now(successDsc,mStepHandler.getStepPercent());
                                      },
                                      [&](int &jump_step)
                                      {
                                          jump_step = failToJump;
                                          update_now(failDsc,mStepHandler.getStepPercent());
                                      });
    }
}