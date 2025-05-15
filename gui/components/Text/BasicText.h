//
// Created by Huwensong on 2024/11/8.
//

#ifndef FIRMWARE_BASICTEXT_H
#define FIRMWARE_BASICTEXT_H

#include <string>
#include "lvgl.h"
#include "Animation.h"
//#include "tools.h"
#include "ThemeInterface.h"

namespace UI
{
	class BasicText {
	public:
		explicit BasicText(lv_obj_t *parent,lv_coord_t width = 300, uint32_t fade_in_t = 1000,uint32_t fade_out_t = 1200);

		~BasicText();

	public:
		void set_font_size(uint32_t size);

        void align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

        void alignTo(const BasicText &base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

		void update(const std::string &content, bool anim_on = true, uint32_t delay = 0);

	private:
		static void fade_in(lv_anim_t *anim);

		void update();

	private:
		lv_obj_t *mBaseLabel= nullptr;

		uint32_t mFadeInTime;
		uint32_t mFadeOutTime;

		std::string mContent;
	};
}




#endif //FIRMWARE_BASICTEXT_H
