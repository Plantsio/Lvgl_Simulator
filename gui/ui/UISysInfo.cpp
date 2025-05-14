//
// Created by Gordon on 2021/12/2.
//

#include "UISysInfo.h"
#include "WiFi.h"
#include "IvyNet.h"
#include "definitions.h"
#include "SysOld.h"
#include "ThemeInterface.h"
#include "Lang.h"

namespace UI
{
//    static void draw_cb(lv_event_t *e)
//    {
//        lv_obj_t *obj = lv_event_get_target(e);
//        auto *dsc = (lv_obj_draw_part_dsc_t *) lv_event_get_param(e);
//        if (dsc->part == LV_PART_ITEMS)
//        {
//            uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
//            uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);
//            if (col == 0)
//            {
//                dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
//            }
//            //dsc->label_dsc->font = &ba_16;
//            dsc->label_dsc->color = lv_color_white();
//            dsc->rect_dsc->border_color = lv_color_make(154, 215, 253);
//            dsc->rect_dsc->border_side = LV_BORDER_SIDE_FULL;
//        }
//    }

    UISysInfo::UISysInfo(ObjPtr obj)
            : Base(std::move(obj)),
              m_table(lv_table_create(m_scr))
    {
//        m_input_used = true;
//        set_routable_indicator_visible(true, right);

		THEME_SET_FONT_SIZE(m_table,16);
        update_info();
        lv_obj_set_size(m_table, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_center(m_table);
        lv_obj_clear_flag(m_table, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_bg_opa(m_table, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(m_table, LV_OPA_TRANSP, 0);
        lv_obj_set_style_pad_ver(m_table, 8, LV_PART_ITEMS);
        lv_obj_set_style_bg_opa(m_table, LV_OPA_TRANSP, LV_PART_ITEMS);
        //lv_obj_add_event_cb(m_table, draw_cb, LV_EVENT_DRAW_PART_BEGIN, nullptr);
    }

    void UISysInfo::update_info()
    {
        lv_table_set_cell_value(m_table, 0, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_wifi_name).c_str());

        lv_table_set_cell_value(m_table, 3, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_location).c_str());

        lv_table_set_cell_value(m_table, 1, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_public_ip).c_str());

        lv_table_set_cell_value(m_table, 2, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_local_ip).c_str());

        lv_table_set_cell_value(m_table, 4, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_ota_ver).c_str());

        lv_table_set_cell_value(m_table, 5, 0,
								THEME_TEXT_CONTENT(Lang::ui_sysinfo_evolve).c_str());


        lv_table_set_cell_value(m_table, 0, 1, WiFi.SSID().c_str());
        lv_table_set_cell_value(m_table, 3, 1, SysOld::get_geo_info().location.city_district.c_str());

        lv_table_set_cell_value(m_table, 1, 1, SysOld::get_geo_info().public_ip.c_str());
        lv_table_set_cell_value(m_table, 2, 1, WiFi.localIP().toString().c_str());
        lv_table_set_cell_value(m_table, 4, 1, SOFTWARE_VER);

        lv_obj_center(m_table);
        time_t t = Prop::get<uint32_t>(Prop::last_evolve_time);
        tm *info = localtime(&t);
        std::string t_str = std::to_string(info->tm_year + 1900) + "-" + std::to_string(info->tm_mon + 1) + "-" +
                            std::to_string(info->tm_mday);
        lv_table_set_cell_value(m_table, 5, 1, t_str.c_str());

    }

	void UISysInfo::left_input_cb(InputData data)
	{
			
	}

	void UISysInfo::right_input_cb(InputData data)
	{
		static lv_coord_t current_p = 0;
		if (current_p > lv_obj_get_height(m_table)) {
			current_p = 0;
		} else {
			current_p += SYS_UI_HEIGHT;
		}
		lv_obj_scroll_to_y(m_table, current_p, LV_ANIM_ON);
	}
}