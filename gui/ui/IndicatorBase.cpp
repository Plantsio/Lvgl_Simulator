//
// Created by sungaoran on 2025/7/17.
//

#include "IndicatorBase.h"
#include "Animation.h"

namespace UI {
    IndicatorBase::IndicatorBase(lv_obj_t *parent)
            : m_obj(lv_obj_create(parent)) {
        lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_size(m_obj, 7, 7);
        lv_obj_set_style_radius(m_obj, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(m_obj, 0, 0);
        lv_obj_set_style_bg_color(m_obj, lv_color_make(245, 203, 38), 0);
    }

    void IndicatorBase::setState(IndicatorBase::IndState state) {
        if (state == m_state) {
            return;
        }
        if (state == IndHidden) {
             Animation::anim_fade_out(m_obj, LV_INDI_FADE_T, 0, [](lv_anim_t *anim) {
                auto instance = static_cast<IndicatorBase *>(anim->user_data);
                lv_obj_clear_flag(instance->m_obj, LV_OBJ_FLAG_HIDDEN);
            }, this);
        } else if (state == IndShow) {
            lv_obj_clear_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
            Animation::anim_fade_in(m_obj, LV_INDI_FADE_T, 0);
        } else if (state == IndFlash){
            lv_obj_clear_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
			Animation::anim_create(m_obj,Animation::anim_fade_cb,
								   LV_OPA_TRANSP,LV_OPA_COVER,LV_INDI_FADE_T,LV_INDI_FADE_T,LV_INDI_FADE_T,LV_INDI_FADE_T,-1);

        }
        m_state = state;
    }

    void IndicatorBase::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) const {
        lv_obj_align(getObj(), align, x_ofs, y_ofs);
    }
}