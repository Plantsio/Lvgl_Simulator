//
// Created by sungaoran on 2022/1/19.
//

#include "UITime.h"
#include "interface.h"
#include "log.h"
#include "Lang.h"

#include "ThemeInterface.h"

namespace UI {
    UITime::UITime(ObjPtr obj) : Base(std::move(obj)), mIndicator(m_scr) {
        weather_anim = lv_canvas_create(m_scr);
//    lv_obj_set_pos(weather_anim, 200, 120);

        time_label = lv_label_create(m_scr);
		THEME_SET_FONT_SIZE(time_label,120);

        day_label = lv_label_create(m_scr);
		THEME_SET_FONT_SIZE(day_label,30);

        date_label = lv_label_create(m_scr);
		THEME_SET_FONT_SIZE(date_label,30);

        sep_line = lv_line_create(m_scr);
        lv_obj_set_style_line_color(sep_line, lv_color_make(255, 255, 255), 0);
        lv_line_set_points(sep_line, sep_line_points, 2);
        lv_obj_set_style_line_width(sep_line, 1, 0);
        lv_obj_set_pos(sep_line, 180, 135);

        am_label = lv_label_create(m_scr);
        label_set_style(am_label, &ba_16);

//        if (!Prop::get<bool>(Prop::use_12h_mode)) {
//            /* hide am label if using 24H mode */
//            lv_obj_add_flag(am_label, LV_OBJ_FLAG_HIDDEN);
//            lv_obj_align(day_label, LV_ALIGN_BOTTOM_LEFT, 50, -65);
//        }
//        else
//        {
//            lv_obj_align(day_label, LV_ALIGN_BOTTOM_LEFT, 40, -65);
//        }

        mIndicator.enable_visible(Indicator::ALL);
    }

    bool UITime::_initialize() {
        update_info(true);
        lv_obj_align_to(weather_anim, sep_line, LV_ALIGN_OUT_RIGHT_MID, 20, -6);
        lv_obj_align_to(am_label, day_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, -5);

		log_d("ui time start routine");
        return true;
    }

    void UITime::update_info(bool force) {
//    anim_canvas_bind_asset(weather_anim, weather);
//        const weather_t &weather = SysOld::get_geo_info().weather;
//
//
//        std::string name = SysOld::get_weather_by_lang((Lang::lang_t)Prop::get<int>(Prop::language));
//        if (weather_name != name || force) {
//            weather_name = name;
//            anim_canvas_reset_asset();
//            anim_canvas_bind_asset(weather_anim, get_weather_asset_name(weather), cus_tu,force);
//        }

        time_t curtime;
        time(&curtime);
        tm *info = localtime(&curtime);

        /* "15:23", "FRI", "21st Jan", "weather" */

        if (force || last_tm.tm_hour != info->tm_hour || last_tm.tm_min != info->tm_min) {
            last_tm.tm_hour = info->tm_hour;
            last_tm.tm_min = info->tm_min;
            char time_str[6];
            int hour = info->tm_hour;
            int min = info->tm_min;
//            if (Prop::get<bool>(Prop::use_12h_mode)) {
//                std::string am = "AM";
//                if (hour > 11 && hour != 24) {
//                    am = "PM";
//                }
//                if (hour == 0) {
//                    hour = 12;
//                } else if (hour > 12) {
//                    hour = hour - 12;
//                }
//                lv_label_set_text(am_label, am.c_str());
//            }
            sprintf(time_str, "%02d:%02d", hour, min);
            lv_label_set_text(time_label, time_str);
            lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -50);
        }

        if (force || last_tm.tm_wday != info->tm_wday) {
            last_tm.tm_wday = info->tm_wday;
            lv_label_set_text(day_label, THEME_TEXT_OPTION(Lang::ui_day, info->tm_wday).c_str());
        }

        if (force || last_tm.tm_mday != info->tm_mday || last_tm.tm_mon != info->tm_mon) {
            last_tm.tm_mday = info->tm_mday;
            last_tm.tm_mon = info->tm_mon;
            lv_label_set_text(date_label, get_date_str(info).c_str());
            lv_obj_align(date_label, LV_ALIGN_BOTTOM_LEFT, 40, -20);
        }
    }

    void UITime::routine() {
        update_info();
//        anim_canvas_update(weather_anim);
    }

    std::string UITime::get_date_str(tm *tm_info) {
        std::string date_str;
//        switch (Lang::Language::instance().get_language()) {
//            case Lang::zh:
//            case Lang::jp:
//                date_str = std::to_string(tm_info->tm_mon + 1) + THEME_TEXT_CONTENT(Lang::ui_month) + std::to_string(tm_info->tm_mday) +
//						THEME_TEXT_CONTENT(Lang::ui_date);
//                break;
//            case Lang::en:
//            case Lang::fr:
//                date_str = get_mday_str(tm_info->tm_mday) + std::string(" ") + THEME_TEXT_OPTION(Lang::ui_month,tm_info->tm_mon);
//            default:
//                break;
//        }
        return date_str;
    }

}