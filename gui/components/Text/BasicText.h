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

		void update(const std::string &content, bool anim_on = true, uint32_t delay = 0);

		[[nodiscard]] lv_obj_t *get_origin_obj() const;

	private:
		static void fade_in(lv_anim_t *anim);

		void update();

	private:
		lv_obj_t *m_base_label= nullptr;

		uint32_t m_fade_in_t;
		uint32_t m_fade_out_t;

		std::string m_content;
	};
}




#endif //FIRMWARE_BASICTEXT_H
