//
// Created by sungaoran on 2022/4/30.
//

#ifndef LVGL_UITEST_H
#define LVGL_UITEST_H

#include "UIBase.h"

namespace UI {
    class UITest : public Base {
    public:
        explicit UITest(ObjPtr obj);

        bool _handleInput(InputEvtType &&input) override;

        void update_step(const char *step);

        void update_desc(const char *desc);

        void update_value(const char *value);

        void update_progress(int progress, int anim_t = 0);

        void update_color(lv_color_t back_color, lv_color_t text_color = lv_color_white());

        void reset_progress();

		void left_input_cb(InputData data) override;

		void right_input_cb(InputData data) override;

        void update(int progress, const char *step, const char* desc, lv_color_t color);

        bool right_touched = false;
        bool left_touched = false;

    private:
        lv_obj_t *m_step_label;
        lv_obj_t *m_value_label;
        lv_obj_t *m_desc_label;
        lv_obj_t *m_progress_bar;
    };
}

#endif //LVGL_UITEST_H
