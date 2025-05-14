//
// Created by Huwensong on 2024/11/8.
//

#ifndef FIRMWARE_ANIMATION_H
#define FIRMWARE_ANIMATION_H

#include <functional>
#include "lvgl.h"

namespace UI
{
	class Animation
	{
	public:
		static void anim_fade_in(lv_obj_t *obj,uint32_t duration,uint32_t delay);

		static void anim_fade_out(lv_obj_t *obj,uint32_t duration,uint32_t delay);

		static void anim_create(lv_obj_t *obj,lv_anim_exec_xcb_t exec_cb, int value_start, int value_end, uint32_t duration,
						 uint32_t delay = 0,uint32_t playback_time = 0,uint32_t playback_delay = 0,uint32_t repeat_cnt = 0,
						 lv_anim_ready_cb_t read_cb = nullptr,
						 void *user_data = nullptr, lv_anim_path_cb_t path = lv_anim_path_ease_out);

		static void anim_fade_cb(void *,int32_t);

	private:
		static lv_anim_t anim_create_internal(lv_obj_t *obj,lv_anim_exec_xcb_t exec_cb, int value_start, int value_end, uint32_t duration,
										 uint32_t delay = 0,uint32_t playback_time = 0,uint32_t playback_delay = 0,uint32_t repeat_cnt = 0,
										 lv_anim_ready_cb_t ready_cb = nullptr,
										 void *user_data = nullptr, lv_anim_path_cb_t path = lv_anim_path_ease_out);
	};
}




#endif //FIRMWARE_ANIMATION_H
