//
// Created by Huwensong on 2024/11/8.
//

#include "BasicText.h"
#include "ThemeInterface.h"

namespace UI
{
	BasicText::BasicText(lv_obj_t *parent, lv_coord_t width, uint32_t fade_in_t, uint32_t fade_out_t) :
    mBaseLabel(lv_label_create(parent)),
    mFadeInTime(fade_in_t),
    mFadeOutTime(fade_out_t)
	{
		lv_obj_set_width(mBaseLabel,width);
		lv_label_set_recolor(mBaseLabel, true);
		lv_label_set_text(mBaseLabel, "");
		lv_obj_set_style_text_color(mBaseLabel,lv_color_white(),0);
		lv_obj_set_style_text_align(mBaseLabel,LV_FLEX_ALIGN_CENTER,0);
	}

	BasicText::~BasicText()
	{
		lv_obj_del(mBaseLabel);
	}

	void BasicText::set_font_size(uint32_t size)
	{
		THEME_SET_FONT_SIZE(mBaseLabel,size);
	}

    void BasicText::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        lv_obj_align(mBaseLabel,align,x_ofs,y_ofs);
    }

    void BasicText::alignTo(const BasicText &base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {

        lv_obj_align_to(mBaseLabel,base.mBaseLabel,align,x_ofs,y_ofs);
    }

	void BasicText::update(const std::string &content, bool anim_on, uint32_t delay)
	{
		if (mContent.empty() || mContent != content)
		{
            mContent = content;

			if (anim_on)
			{
				Animation::anim_create(mBaseLabel, Animation::anim_fade,LV_OPA_COVER, LV_OPA_TRANSP,
									   mFadeOutTime, delay,0, 0, 0,
									   fade_in,this);
			}
			else
			{
				lv_label_set_text(mBaseLabel,content.c_str());
			}
		}
	}

	void BasicText::fade_in(lv_anim_t *anim)
	{
		static_cast<BasicText *>(anim->user_data)->update();
	}

	void BasicText::update()
	{
		lv_label_set_text(mBaseLabel,mContent.c_str());
		Animation::anim_create(mBaseLabel, Animation::anim_fade,LV_OPA_TRANSP, LV_OPA_COVER,
							   mFadeInTime);
	}
}