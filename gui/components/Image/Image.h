//
// Created by Huwensong on 2024/11/11.
//

#ifndef FIRMWARE_IMAGE_H
#define FIRMWARE_IMAGE_H

#include <string>

#include "lvgl.h"
#include "JPEGDEC.h"

namespace UI
{
	class Image
	{
	public:
		explicit Image(lv_obj_t *parent);
		~Image();

	public:
		void update(const std::string &name, bool anim_on = true,uint32_t duration = 1200,uint32_t delay = 1000);

		[[nodiscard]] lv_obj_t *get_image_obj() const;

	private:
		bool load_image(const std::string &path);

		bool decode_image();

		static int image_draw_wrapper(JPEGDRAW *draw);

		void image_draw_cb(JPEGDRAW *draw);

		static std::string get_assets_path(const std::string &name);

		void clear();

	private:
		lv_obj_t *m_image;

		JPEGDEC m_mjpeg_dec;

		lv_img_dsc_t m_img_desc{};

		uint8_t  *m_load_cache = nullptr;
		uint16_t *m_draw_cache = nullptr;

		uint32_t m_frame_height = 0;
		uint32_t m_frame_width  = 0;
		uint32_t m_frame_size = 0;
	};
}

#endif //FIRMWARE_IMAGE_H
