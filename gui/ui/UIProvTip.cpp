//
// Created by Huwensong on 2023/2/6.
//

#include "UIProvTip.h"
//#include "tools.h"
#include "ThemeInterface.h"
//#include "Boot.h"
//#include "SysOld.h"
#include "gui_data_defines.h"

namespace UI
{
//    ProvTip::ProvTip(lv_obj_t *_parent, const Gui::ObjData &data) :
//            TimerStepBase(_parent, nullptr, nullptr),
//            m_sub_text(_parent, 280, 800),
//            m_title_text(_parent, 280, 800),
//            m_image(_parent),
//            mData(data)
//    {
//        m_sub_text.set_font_size(16);
//        m_title_text.set_font_size(30);
//
//        lv_obj_set_width(m_bottom_text.get_origin_obj(), 160);
//        lv_label_set_long_mode(m_bottom_text.get_origin_obj(), LV_LABEL_LONG_SCROLL_CIRCULAR);
//    }
//
//    bool ProvTip::checkError()
//    {
//        auto option = getInfo();
//        if (option)
//        {
//            auto info = option->get();
//            if (info.status >= IOTManager::IOT_DISCONNECTED)
//            {
//                if (mCurrentError != info.errCode)
//                {
//                    switch (info.errCode)
//                    {
//                        case WIFI_REASON_NO_AP_FOUND:
//                            m_sub_text.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tuya_tip_reason_ap_not_found,
//                                                                      Theme::palette_failure));
//                            m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_fail));
//                            break;
//                        case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
//                        case WIFI_REASON_HANDSHAKE_TIMEOUT:
//                            m_sub_text.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tuya_tip_reason_pwd_error,
//                                                                      Theme::palette_failure));
//                            m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_fail));
//                            break;
//                        default:
//                            m_sub_text.update(
//                                    THEME_TEXT_APPEND_COLOR(Lang::ui_tuya_tip_fail_reset, Theme::palette_failure));
//                            break;
//                    }
//                    mCurrentError = info.errCode;
//                }
//                return true;
//            }
//            else
//                return false;
//        }
//        else
//            return false;
//
//    }
//
//    bool ProvTip::reach_status(IOTManager::NetStatus target, const std::string &processing_text)
//    {
//        auto option = getInfo();
//        if (option)
//        {
//            auto info = option->get();
//            log_d("info.status === %d",info.status);
//            if (info.status >= target)
//                return true;
//        }
//
//        if (mCurrentProcessingText != processing_text && !processing_text.empty())
//        {
//            m_sub_text.update(processing_text);
//            mCurrentProcessingText = processing_text;
//        }
//
//        return false;
//    }
//
//    void ProvTip::next()
//    {
//        if (checkError())
//        {
//            /* error occurred, skipping normal process */
//            return;
//        }
//
//        switch (m_current_step)
//        {
//            case 0:
//            {
//                lv_timer_set_period(m_timer, 10000);  /* check status every 100ms */
//                m_top_text.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_open_tuya_app, Theme::palette_notice));
//                m_sub_text.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_tuya_add_device, Theme::palette_notice));
//                lv_obj_align_to(m_sub_text.get_origin_obj(), m_top_text.get_origin_obj(), LV_ALIGN_OUT_BOTTOM_MID, 0,
//                                10);
//                lv_obj_align_to(m_title_text.get_origin_obj(), m_top_text.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 20);
//
//                std::string asst_name = "prov";
//                m_image.update(asst_name);
//
//                lv_obj_align(m_image.get_image_obj(), LV_ALIGN_CENTER, 0, 15);
//
//                m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_popup), true, 2000);
//                break;
//            }
//            case 1:
//            {
//                m_top_text.update("");
//                m_sub_text.update("");
//                m_bottom_text.update("");
//                m_title_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_configuring));
//                lv_obj_align(m_sub_text.get_origin_obj(), LV_ALIGN_CENTER, 0, 0);
//                Animation::anim_fade_out(m_image.get_image_obj(), 400, 0);
//                lv_timer_set_period(m_timer, 100);
//                break;
//            }
//            case 2:
//            {
//                if (!reach_status(IOTManager::IOT_CONNECTED, THEME_TEXT_CONTENT(Lang::ui_prov_connecting_router)))
//                {
//                    return;
//                }
//
//                m_sub_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_router_success), true, 200);
//
//                break;
//            }
//            case 3:
//            {
//                if (!reach_status(IOTManager::IOT_ACTIVATED, THEME_TEXT_CONTENT(Lang::ui_prov_activating)))
//                {
//                    return;
//                }
//                m_sub_text.update(THEME_TEXT_CONTENT(Lang::ui_prov_act_success));
//                break;
//            }
//            case 4:
//            {
//                end();
//                break;
//            }
//        }
//        m_current_step++;
//    }
//
//    std::optional<std::reference_wrapper<const Gui::ProvInfo>> ProvTip::getInfo()
//    {
//        return std::visit([](auto &arg) -> std::optional<std::reference_wrapper<const Gui::ProvInfo>>
//                          {
//                              using T = std::decay_t<decltype(arg)>;
//                              if constexpr (std::is_same_v<T, Gui::ProvInfo>)
//                              {
//                                  return std::ref(arg);
//                              }
//                              else
//                              {
//                                  return std::nullopt;
//                              }
//                          }, mData);
//    }
//
//    bool UIProvTip::_initialize()
//    {
//        start();
//        return true;
//    }


}