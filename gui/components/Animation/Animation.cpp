//
// Created by Huwensong on 2024/11/8.
//

#include "Animation.h"

#include <utility>
#include "esp32-hal-log.h"

namespace UI
{
	void Animation::anim_fade_in(lv_obj_t *obj,uint32_t duration,uint32_t delay)
	{
		anim_create(obj,anim_fade_cb,LV_OPA_TRANSP,LV_OPA_COVER,
					duration,delay);
	}

	void Animation::anim_fade_out(lv_obj_t *obj,uint32_t duration,uint32_t delay)
	{
		anim_create(obj,anim_fade_cb,LV_OPA_COVER,LV_OPA_TRANSP,
					duration,delay);
	}

	static void flash_in_cb(void *param,int32_t value)
	{
		lv_obj_set_style_opa((lv_obj_t *) param, value, 0);
	}

	static void flash_out_cb(void *param,int32_t value)
	{
		lv_obj_set_style_opa((lv_obj_t *) param, value, 0);
	}

	static void flash_hidden_cb(lv_anim_t *anim)
	{
		lv_obj_clear_flag((lv_obj_t *)anim->var,LV_OBJ_FLAG_HIDDEN);
	}

	void Animation::anim_create(lv_obj_t *obj,lv_anim_exec_xcb_t exec_cb, int value_start, int value_end,uint32_t duration,
								uint32_t delay,uint32_t playback_time,uint32_t playback_delay,uint32_t repeat_cnt,
								lv_anim_ready_cb_t read_cb,void *user_data, lv_anim_path_cb_t path)
	{
		lv_anim_t anim = anim_create_internal(obj,exec_cb, value_start, value_end,duration,
				delay,playback_time,playback_delay,repeat_cnt,
				read_cb,user_data,path);
		lv_anim_start(&anim);
	}

	void Animation::anim_fade_cb(void *param,int32_t value)
	{
		lv_obj_set_style_opa((lv_obj_t *) param, value, 0);
	}

	lv_anim_t Animation::anim_create_internal(lv_obj_t *obj,lv_anim_exec_xcb_t exec_cb, int value_start, int value_end,
										 uint32_t duration,uint32_t delay,uint32_t playback_time,uint32_t playback_delay,
										 uint32_t repeat_cnt ,lv_anim_ready_cb_t ready_cb ,
										 void *user_data, lv_anim_path_cb_t path)
	{
		lv_anim_t animation;
		lv_anim_init(&animation);
		lv_anim_set_var(&animation, obj);
		lv_anim_set_exec_cb(&animation, exec_cb);
		lv_anim_set_values(&animation, value_start, value_end);
		lv_anim_set_time(&animation, duration);

		if (delay)
		{
			lv_anim_set_delay(&animation, delay);
		}
		if (playback_time)
		{
			lv_anim_set_playback_time(&animation, playback_time);
			lv_anim_set_playback_delay(&animation, playback_delay);
		}
		if (ready_cb)
		{
			lv_anim_set_ready_cb(&animation,ready_cb);
		}

		lv_anim_set_path_cb(&animation, path);
		lv_anim_set_user_data(&animation, user_data);
		lv_anim_set_repeat_count(&animation, repeat_cnt);
		return animation;
	}
}

