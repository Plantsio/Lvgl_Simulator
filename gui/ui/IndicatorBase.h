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
    public:
        explicit IndicatorBase(lv_obj_t *parent);

        [[nodiscard]] lv_obj_t *getObj() const {
            return m_obj;
        }

        void align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) const;

    public:
        void setState(IndState state);

    private:
        lv_obj_t *m_obj;

        IndState m_state = IndHidden;

    };
}

#endif //SIMULATOR_INDICATORBASE_H
