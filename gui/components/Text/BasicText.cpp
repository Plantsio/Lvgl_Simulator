//
// Created by Huwensong on 2024/11/8.
//

#include "BasicText.h"
#include "ThemeInterface.h"

namespace UI
{
	BasicText::BasicText(lv_obj_t *parent, lv_coord_t width, uint32_t fade_in_t, uint32_t fade_out_t) :
	m_base_label(lv_label_create(parent)),
	m_fade_in_t(fade_in_t),
	m_fade_out_t(fade_out_t)
	{
		lv_obj_set_width(m_base_label,width);
		lv_label_set_recolor(m_base_label, true);
		lv_label_set_text(m_base_label, "");
		lv_obj_set_style_text_color(m_base_label,lv_color_white(),0);
		lv_obj_set_style_text_align(m_base_label,LV_FLEX_ALIGN_CENTER,0);
	}

	BasicText::~BasicText()
	{
		lv_obj_del(m_base_label);
	}

	void BasicText::set_font_size(uint32_t size)
	{
		THEME_SET_FONT_SIZE(m_base_label,size);
	}

	void BasicText::update(const std::string &content, bool anim_on, uint32_t delay)
	{
		if (m_content.empty() || m_content != content)
		{
			m_content = content;

			if (anim_on)
			{
//				Animation::anim_create(m_base_label, anim_fade,LV_OPA_COVER, LV_OPA_TRANSP,
//									   m_fade_out_t, delay,0, 0, 0,
//									   fade_in,this);
			}
			else
			{
				lv_label_set_text(m_base_label,content.c_str());
			}
		}
	}

	lv_obj_t *BasicText::get_origin_obj() const
	{
		return m_base_label;
	}

	void BasicText::fade_in(lv_anim_t *anim)
	{
		static_cast<BasicText *>(anim->user_data)->update();
	}

	void BasicText::update()
	{
		lv_label_set_text(m_base_label,m_content.c_str());
//		Animation::anim_create(m_base_label, anim_fade,LV_OPA_TRANSP, LV_OPA_COVER,
//							   m_fade_out_t);
	}
}