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
    static std::string ui_boot_desc;
    static int ui_boot_progress = 0;

    UIBoot::UIBoot(ObjPtr obj) : Base(std::move(obj)), mProcess(m_scr)
    {}

    int UIBoot::get_current_progress()
    {
        return ui_boot_progress;
    }

    std::string UIBoot::get_current_desc()
    {
        return ui_boot_desc;
    }

    void UIBoot::set(const std::string &desc, int progress)
    {
        set_current_desc(desc);
        set_current_progress(progress);
    }

    void UIBoot::set_current_progress(int progress)
    {
        ui_boot_progress = progress;
    }

    void UIBoot::set_current_desc(const std::string &desc)
    {
        ui_boot_desc = desc;
    }

    bool UIBoot::_initialize()
    {
        mProcess.update_title(THEME_TEXT_CONTENT(Lang::ui_boot_waking_up));

        registerStepCB([&](){
            update_now(THEME_TEXT_CONTENT(Lang::ui_boot_connect_to_net), 5);
            check_routine([]() { return true; }, 10, 7000,
                          THEME_TEXT_CONTENT(Lang::ui_boot_connect_succ),
                          THEME_TEXT_CONTENT(Lang::ui_boot_connect_fail));
            return true;
        });

        registerStepCB([&](){
            update_now(THEME_TEXT_CONTENT(Lang::ui_boot_activating), 15);
            check_routine([]() { return true; }, 20, 10000,
                          THEME_TEXT_CONTENT(Lang::ui_boot_activated),
                          THEME_TEXT_CONTENT(Lang::ui_boot_activate_fail));
            return true;
        });

        registerStepCB([&](){
            update_now(THEME_TEXT_CONTENT(Lang::ui_boot_tuning_time), 25);
            check_routine([]() { return true; }, 30, 5000,
                          THEME_TEXT_CONTENT(Lang::ui_boot_tune_succ),
                          THEME_TEXT_CONTENT(Lang::ui_boot_tune_fail));
            return true;
        });

        registerStepCB([&](){
            refresh_timeout();
            update_now(THEME_TEXT_CONTENT(Lang::ui_boot_locating), 35);
            check_routine([]() { return true; }, 40, 10000,
                          THEME_TEXT_CONTENT(Lang::ui_boot_locate_succ),
                          THEME_TEXT_CONTENT(Lang::ui_boot_locate_fail));

            return true;
        });

        registerStepCB([&](){
            update_now(THEME_TEXT_CONTENT(Lang::ui_boot_physical_check), 45);
            check_routine([]() { return true; }, 50, 5000,
                          THEME_TEXT_CONTENT(Lang::ui_boot_physical_succ),
                          THEME_TEXT_CONTENT(Lang::ui_boot_physical_fail));
            return true;
        });


        //IvyBody::instance().unregister_button_tap();
        return true;
    }

    void UIBoot::update_now(const std::string &desc, int progress)
    {
        mProcess.update_status(desc);
        mProcess.update_progress(progress);
    }

    bool UIBoot::check_routine(const ConditionCb &condition, int progress, int timeout,
                  const HandleCb &success_handler,
                  const HandleCb &fail_handler)
    {
        if (condition())
        {
            update_now(success_desc, progress);
            log_v("debug-boot %s", success_desc.c_str());
            updateStepPeriod( 500);
            return true;
        }
        else
        {
            if (SysTime::millis() - m_current_start_t > timeout)
            {
                update_now(Theme::getPaletteText(fail_desc, Theme::palette_warning), progress);
                over_time = true;
                log_v("debug-boot %s", fail_desc.c_str());
                updateStepPeriod( 800);
                return false;
            }
        }
        return false;
    }

    void UIBoot::refresh_timeout()
    {
        m_current_start_t = SysTime::millis();
        updateStepPeriod( 100);
    }

    void UIBoot::next()
    {
        /*
         * 1.check routine   true success, false fail
         * 2.timeout
         * 3.允许step跳转
         * */

        switch (m_current_step)
        {
            case 0:
            {
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_connect_to_net), 5);
                refresh_timeout();
                break;
            }
            case 1:
                if (!check_routine([]() { return true; }, 10, 7000,
                                   THEME_TEXT_CONTENT(Lang::ui_boot_connect_succ),
                                   THEME_TEXT_CONTENT(Lang::ui_boot_connect_fail)))
                {

                    if (over_time)
                    {
                        m_current_step = 8;
                        over_time = false;
                    }
                    return;
                }
                break;
            case 2:
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_activating), 15);
                refresh_timeout();
                break;
            case 3:
                if (!check_routine([]() { return true; }, 20, 10000,
                                   THEME_TEXT_CONTENT(Lang::ui_boot_activated),
                                   THEME_TEXT_CONTENT(Lang::ui_boot_activate_fail)))
                {
                    if (over_time)
                    {
                        m_current_step = 8;
                        over_time = false;
                    }
                    return;
                }
                break;
            case 4:
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_tuning_time), 25);
                refresh_timeout();
                break;
            case 5:
                if (!check_routine([]() { return true; }, 30, 5000,
                                   THEME_TEXT_CONTENT(Lang::ui_boot_tune_succ),
                                   THEME_TEXT_CONTENT(Lang::ui_boot_tune_fail)))
                {
                    if (over_time)
                    {
                        m_current_step = 8;
                        over_time = false;
                    }
                    return;
                }
                break;
            case 6:
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_locating), 35);
                refresh_timeout();
                break;
            case 7:
                if (!check_routine([]() { return true; }, 40, 10000,
                                   THEME_TEXT_CONTENT(Lang::ui_boot_locate_succ),
                                   THEME_TEXT_CONTENT(Lang::ui_boot_locate_fail)))
                {
                    if (over_time)
                    {
                        m_current_step = 8;
                        over_time = false;
                    }
                    return;
                }
                break;
            case 8:
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_physical_check), 45);
                refresh_timeout();
                break;
            case 9:
                if (!check_routine([]() { return true; },
                                   50, 5000,
                                   THEME_TEXT_CONTENT(Lang::ui_boot_physical_succ),
                                   THEME_TEXT_CONTENT(Lang::ui_boot_physical_fail)))
                {
                    if (over_time)
                    {
                        m_current_step = 10;
                        over_time = false;
                    }
                    return;
                }
                break;
            case 10:
//                if (Prop::get<bool>(Prop::plant_auto_detect))
//                {
//                    m_plant_detect_cnt = Sense::instance().get_plant_detect_cnt();
//                    Sense::instance().set_plant_force_detect_t(millis());
//                }
//                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_plant_check), 55);
//                refresh_timeout();
                log_d("case 10");
                break;
            case 11:
//                if (Prop::get<bool>(Prop::plant_auto_detect))
//                {
//                    if (!check_routine(
//                            [this]() { return true; },
//                            60, 10000,
//                            THEME_TEXT_CONTENT(Lang::ui_boot_plant_checked),
//                            THEME_TEXT_CONTENT(Lang::ui_boot_plant_fail)))
//                    {
//                        if (over_time)
//                        {
//                            m_current_step = 12;
//                            over_time = false;
//                        }
//                        return;
//                    }
//                }
                log_d("case 11");
                break;
            case 12:
                lv_timer_set_period(m_timer, 500);
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_calibrating), 80);
                //Skin::instance().get_tp_by_index(Skin::TP_INDEX_FRONT).set_disable_t(10);
                break;
            case 13:
                update_now(THEME_TEXT_CONTENT(Lang::ui_boot_waked_up), 100);
                lv_timer_set_period(m_timer, 100);
                break;
            case 14:
                log_d("ui boot complete");

                /* ensure releasing of gpio holding after wake up (for auto wake up) */
//                gpio_hold_dis((gpio_num_t) SCR_BCK_GPIO);
//
//                Boot::booted_cb();
//                SysOld::set_condition(SysOld::BOOT_OVER);
//                IvyBody::instance().register_button_tap();
                log_d("ui boot over test");
                break;
        }
        m_current_step++;
    }
}