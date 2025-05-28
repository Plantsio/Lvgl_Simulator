//
// Created by Huwensong on 2024/11/11.
//

#include "Image.h"

#include <fstream>
#include "log.h"
#include "interface.h"
#include "Animation.h"
#include "filepath.h"

namespace UI
{
	Image::Image(lv_obj_t *parent) :
    m_image(lv_img_create(parent))
	{
        lv_obj_align(m_image, LV_ALIGN_CENTER, 0, 0);
    }

	Image::~Image()
	{
		clear();
		lv_obj_del(m_image);
	}

	void Image::update(const std::string &name,bool anim_on,uint32_t duration,uint32_t delay)
	{
		clear();

        log_d("str == %s",ASSETS_DIR.c_str());
		if (!load_image(path({ASSETS_DIR, name + ".mjpeg"})))
		{
			log_e("Failed to load image");
			return;
		}

		if (!decode_image())
		{
			log_e("Failed to decode image");
			return;
		}

		m_img_desc.header.cf = LV_COLOR_FORMAT_NATIVE;
		m_img_desc.header.h  = m_mjpeg_dec.getHeight();
		m_img_desc.header.w  = m_mjpeg_dec.getWidth();
		m_img_desc.data      = (uint8_t *)m_draw_cache;

		lv_img_set_src(m_image, &m_img_desc);

		if (anim_on)
		{
			Animation::anim_fade_in(m_image,duration,delay);
		}

		lv_obj_invalidate(m_image);
	}

	lv_obj_t *Image::get_image_obj() const
	{
		return m_image;
	}

	bool Image::load_image(const std::string &path)
	{
		log_d("path = %s",path.c_str());

        std::ifstream file(path,std::ios::binary);
        if (!file.is_open())
        {
            log_e("Image: Failed to open file");
            return false;
        }

		//read size of one frame
		file.read((char *)&m_frame_size,sizeof(uint32_t));

		if(file.gcount() < 0 || m_frame_size == 0)
		{
			log_e("read frame_size error");
			return false;
		}

		m_load_cache = (uint8_t *) heap_caps_malloc(m_frame_size,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

		if (!m_load_cache)
		{
			log_e("frame data malloc error");
			return false;
		}

        file.read((char *)m_load_cache,m_frame_size);

		if (file.gcount() != m_frame_size)
		{
			log_e("read frame error");
			free(m_load_cache);
			return false;
		}

		log_d("load image success");

		file.close();

		return true;
	}

	bool Image::decode_image()
	{
		m_mjpeg_dec.openRAM(m_load_cache,(int)m_frame_size, image_draw_wrapper, nullptr, nullptr);/* fixme */
		m_mjpeg_dec.setMaxOutputSize(5);
		m_mjpeg_dec.setUserPointer(this);
		m_frame_width  = m_mjpeg_dec.getWidth();
		m_frame_height = m_mjpeg_dec.getHeight();
		m_draw_cache = (uint16_t *) heap_caps_malloc(m_frame_height * m_frame_width * sizeof(uint16_t),MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
		if (!m_draw_cache)
			return false;
		m_mjpeg_dec.decode(0,0,0);
		m_mjpeg_dec.close();

		free(m_load_cache);
		m_load_cache = nullptr;

		log_d("decode success");
		return true;
	}

	int Image::image_draw_wrapper(JPEGDRAW *draw)
	{
		static_cast<Image *>(draw->pUser)->image_draw_cb(draw);
		return true;
	}

	void Image::image_draw_cb(JPEGDRAW *draw)
	{
		for (int i = 0; i < draw->iHeight;i ++)
		{
			memcpy((m_draw_cache + m_frame_width * (i + draw->y) + draw->x),(draw->pPixels + draw->iWidth * i),draw->iWidth * sizeof(uint16_t));
		}
	}

	std::string Image::get_assets_path(const std::string &name)
	{
		return std::string("/sd/assets/") + name + ".mjpeg";
	}

	void Image::clear()
	{
		if (m_load_cache)
			free(m_load_cache);
		if (m_draw_cache)
			free(m_draw_cache);
	}
}

