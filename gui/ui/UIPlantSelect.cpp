//
// Created by Huwensong on 2024/11/15.
//

#include "UIPlantSelect.h"
#include "ThemeInterface.h"
#include "Lang.h"

namespace UI
{
	UIPlantSelect::UIPlantSelect(ObjPtr obj):
	Base(std::move(obj)),
	m_top_text(m_scr,280),
	m_middle_text(m_scr),
	m_bottom_text(m_scr),
	m_indicator(m_scr)
	{
		m_middle_text.set_font_size(30);
		//lv_obj_align(m_middle_text.get_origin_obj(), LV_ALIGN_CENTER, 0, -30);
		//lv_obj_align_to(m_top_text.get_origin_obj(), m_middle_text.get_origin_obj(), LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
	}

	bool UIPlantSelect::_initialize()
	{
		m_middle_text.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_plant_select_app,Theme::palette_notice));
		m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_plant_select_switch));
		m_indicator.enable_visible(Indicator::RIGHT, true,1000,4500);
        return true;
	}

    bool UIPlantSelect::_handleInput(InputEvtType &&input)
    {
        return true;
    }

	void UIPlantSelect::routine()
	{
		//todo: 检测到植物或者检测到右触结束UIPlantSelect
	}
}