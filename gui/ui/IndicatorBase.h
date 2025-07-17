//
// Created by sungaoran on 2025/7/17.
//

#ifndef SIMULATOR_INDICATORBASE_H
#define SIMULATOR_INDICATORBASE_H

#include "lvgl.h"

#define LV_INDI_FADE_T      200

namespace UI {
    class IndicatorBase {
    public:
        enum IndState {
            IndHidden,
            IndShow,
            IndFlash,
            IndLoading,
        };
        enum IndAnimState {
            IndAnimNotReady,
            IndAnimDotOff,
            IndAnimDotOn,
            IndAnimLoadingOff,
            IndAnimLoadingOn,
        };
    public:
        explicit IndicatorBase(lv_obj_t *parent);

        void align(lv_align_t align, int32_t x_ofs, int32_t y_ofs);

    public:
        void setState(IndState state);

    private:
        void _setState(IndState state);

        void _setAnimState(IndAnimState readyState) {
            m_animState = readyState;
        }

        [[nodiscard]] IndAnimState _getReadyState() const {
            return m_animState;
        }

    private:
        lv_obj_t *m_obj;
        lv_obj_t *m_loading;

        IndState m_state = IndHidden;
        IndAnimState m_animState = IndAnimNotReady;

        lv_anim_t *m_anim = nullptr;

    };
}

#endif //SIMULATOR_INDICATORBASE_H
