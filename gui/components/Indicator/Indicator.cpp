//
// Created by Huwensong on 2024/11/15.
//

#include "Indicator.h"
#include "Animation.h"

namespace UI
{
	Indicator::Indicator(lv_obj_t *parent)
	{
		/* init routable indicator */
		for (auto &indicator: m_routable_indicators) {
            indicator = lv_led_create(parent);
			lv_obj_add_flag(indicator, LV_OBJ_FLAG_HIDDEN);
			lv_obj_set_size(indicator, 7, 7);
			lv_obj_set_style_shadow_opa(indicator, LV_OPA_TRANSP, 0);
			lv_led_set_color(indicator, lv_color_make(245, 203, 38));
		}
		lv_obj_align(m_routable_indicators[0], LV_ALIGN_LEFT_MID, 10, 0);
		lv_obj_align(m_routable_indicators[1], LV_ALIGN_RIGHT_MID, -10, 0);
	}

    Indicator::~Indicator()
    {
        for (auto &indicator: m_routable_indicators)
        {
            lv_obj_del(indicator);
        }
    }

	void Indicator::enable_visible(IndicatorPart part,bool anim_on, uint32_t duration,uint32_t delay)
	{
		if (part == LEFT || part == ALL)
		{
			lv_obj_clear_flag(m_routable_indicators[0],LV_OBJ_FLAG_HIDDEN);

			if (anim_on)
				Animation::anim_fade_in(m_routable_indicators[0],duration,delay);
		}

		if (part == RIGHT || part == ALL)
		{
			lv_obj_clear_flag(m_routable_indicators[1],LV_OBJ_FLAG_HIDDEN);

			if (anim_on)
				Animation::anim_fade_in(m_routable_indicators[1],duration,delay);
		}
	}

	void Indicator::disable_visible(IndicatorPart part,bool anim_on, uint32_t duration,uint32_t delay)
	{
		if (part == LEFT || part == ALL)
		{
			lv_obj_add_flag(m_routable_indicators[0],LV_OBJ_FLAG_HIDDEN);

			if (anim_on)
				Animation::anim_fade_out(m_routable_indicators[0],duration,delay);
		}

		if (part == RIGHT || part == ALL)
		{
			lv_obj_add_flag(m_routable_indicators[1],LV_OBJ_FLAG_HIDDEN);

			if (anim_on)
				Animation::anim_fade_out(m_routable_indicators[1],duration,delay);
		}
	}

	void Indicator::enable_visible_flashing(IndicatorPart part,
								 uint32_t duration,uint32_t delay,uint32_t playback_time,uint32_t playback_delay,
								 uint32_t repeat)
	{
		if (part == LEFT || part == ALL)
		{
			lv_obj_clear_flag(m_routable_indicators[0],LV_OBJ_FLAG_HIDDEN);
			Animation::anim_create(m_routable_indicators[0],Animation::anim_fade_cb,
								   LV_OPA_TRANSP,LV_OPA_COVER,duration,delay,playback_time,playback_delay,repeat);
		}

		if (part == RIGHT || part == ALL)
		{
			lv_obj_clear_flag(m_routable_indicators[1],LV_OBJ_FLAG_HIDDEN);
			Animation::anim_create(m_routable_indicators[1],Animation::anim_fade_cb,
								   LV_OPA_TRANSP,LV_OPA_COVER,duration,delay,playback_time,playback_delay,repeat);
		}
	}
}