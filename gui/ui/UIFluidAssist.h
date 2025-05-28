//
// Created by Gordon on 2022/5/23.
//

#ifndef LVGL_UIFLUIDASSIST_H
#define LVGL_UIFLUIDASSIST_H

#include "UIFluid.h"

namespace UI {
#define FLUID_BAR_WIDTH     8

    class UIFluidAssist : public UIFluid {
    public:
        explicit UIFluidAssist(ObjPtr obj, int tank_size=WATER_TANK_SIZE);

        void update_status(bool force) override;

        void render_wave() override;

        void stable_cb(bool stable) override;

        void update_assist_bar();

        virtual uint16_t get_target_level();

        virtual uint16_t get_target_range();

    private:
        bool _initialize() override;

    protected:
        lv_obj_t *m_bar_label;
        lv_obj_t *m_middle_label;
        lv_obj_t *m_bar_top;
        lv_obj_t *m_bar_bottom;
        lv_obj_t *m_bar_middle;
        lv_point_precise_t m_bar_points[4]{};

        lv_value_precise_t m_assist_top = 0;
        lv_value_precise_t m_assist_bottom = 0;
        lv_value_precise_t m_assist_start_y = 0;

    };
}


#endif //LVGL_UIFLUIDASSIST_H
