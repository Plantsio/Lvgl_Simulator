//
// Created by sungaoran on 2025/7/17.
//

#include "IndicatorBase.h"
#include "Animation.h"

namespace UI {
    IndicatorBase::IndicatorBase(lv_obj_t *parent)
            : m_obj(lv_obj_create(parent)),
              m_loading(lv_spinner_create(parent)) {
        lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_size(m_obj, 7, 7);
        lv_obj_set_style_radius(m_obj, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_border_width(m_obj, 0, 0);
        lv_obj_set_style_bg_color(m_obj, lv_color_make(245, 203, 38), 0);


        lv_obj_add_flag(m_loading, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_size(m_loading, 10, 10);
        lv_obj_set_style_arc_width(m_loading, 2, LV_PART_INDICATOR);
        lv_obj_set_style_arc_color(m_loading, lv_color_make(245, 203, 38), LV_PART_INDICATOR);
        lv_obj_set_style_arc_opa(m_loading, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_spinner_set_anim_params(m_loading, 1000, 260);
    }

    void IndicatorBase::setState(IndicatorBase::IndState state) {
        if (state == m_state) {
            return;
        }
        auto lastState = m_state;

        m_state = state;
        if (lastState == IndFlash && m_anim && _getReadyState() == IndAnimNotReady) {
            lv_anim_set_repeat_count(m_anim, 0);
            lv_anim_set_ready_cb(m_anim, [](lv_anim_t *anim) {
                auto instance = static_cast<IndicatorBase *>(lv_anim_get_user_data(anim));
                instance->_setAnimState(IndAnimDotOff);
                instance->_setState(instance->m_state);
            });
        } else {
            _setState(state);
        }
    }

    void IndicatorBase::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
        lv_obj_align(m_obj, align, x_ofs, y_ofs);
        lv_obj_align_to(m_loading, m_obj, LV_ALIGN_CENTER, 0, 0);
    }

    void IndicatorBase::_setState(IndicatorBase::IndState state) {
        if (state == IndLoading) {
            if (m_animState == IndAnimLoadingOn) {
                return;
            }
            if (m_animState == IndAnimDotOn) {
                _setState(IndHidden);
            }

            lv_obj_clear_flag(m_loading, LV_OBJ_FLAG_HIDDEN);
            m_anim = Animation::anim_fade_in(m_loading, LV_INDI_FADE_T, 0, [](lv_anim_t *anim) {
                auto instance = static_cast<IndicatorBase *>(anim->user_data);
                instance->_setAnimState(IndAnimLoadingOn);
            }, this);
        } else {
            if (m_animState == IndAnimLoadingOn) {
                m_anim = Animation::anim_fade_out(m_loading, LV_INDI_FADE_T, 0, [](lv_anim_t *anim) {
                    auto instance = static_cast<IndicatorBase *>(anim->user_data);
                    lv_obj_add_flag(instance->m_loading, LV_OBJ_FLAG_HIDDEN);
                    instance->_setAnimState(IndAnimLoadingOff);
                }, this);
            }

            if (state == IndHidden) {
                if (m_animState == IndAnimDotOff) {
                    return;
                }
                m_anim = Animation::anim_fade_out(m_obj, LV_INDI_FADE_T, 0, [](lv_anim_t *anim) {
                    auto instance = static_cast<IndicatorBase *>(anim->user_data);
                    lv_obj_add_flag(instance->m_obj, LV_OBJ_FLAG_HIDDEN);
                    instance->_setAnimState(IndAnimDotOff);
                }, this);
            } else if (state == IndShow) {
                if (m_animState == IndAnimDotOn) {
                    return;
                }
                lv_obj_clear_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
                m_anim = Animation::anim_fade_in(m_obj, LV_INDI_FADE_T, 0, [](lv_anim_t *anim) {
                    auto instance = static_cast<IndicatorBase *>(anim->user_data);
                    instance->_setAnimState(IndAnimDotOn);
                }, this);
            } else if (state == IndFlash) {
                lv_obj_clear_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
                m_anim = Animation::anim_create(m_obj, Animation::anim_fade_cb,
                                                LV_OPA_TRANSP, LV_OPA_COVER, LV_INDI_FADE_T, LV_INDI_FADE_T,
                                                LV_INDI_FADE_T,
                                                LV_INDI_FADE_T, -1,
                                                nullptr, this);
            }
        }
        _setAnimState(IndAnimNotReady);


    }
}