//
// Created by Huwensong on 2024/11/15.
//

#ifndef FIRMWARE_INDICATOR_H
#define FIRMWARE_INDICATOR_H

#include "lvgl.h"

namespace UI
{
	class Indicator
	{
	public:
		enum IndicatorPart
		{
			LEFT,
			RIGHT,
			ALL,
		} ;

	public:
		explicit Indicator(lv_obj_t *parent);

        ~Indicator();

	public:
		void enable_visible(IndicatorPart part,bool anim_on = false,uint32_t duration = 0,uint32_t delay = 0);

		void disable_visible(IndicatorPart part,bool anim_on = false,uint32_t duration = 0,uint32_t delay = 0);

		void enable_visible_flashing(IndicatorPart part,
									 uint32_t duration,uint32_t delay,uint32_t playback_time,uint32_t playback_delay,
									 uint32_t repeat);

	private:
		lv_obj_t *m_routable_indicators[2] = {};
	};
}




#endif //FIRMWARE_INDICATOR_H
