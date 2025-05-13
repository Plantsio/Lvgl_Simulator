//
// Created by Gordon on 2022/11/9.
//

#ifndef PLANTSIO_IVY_UITESTAGING_H
#define PLANTSIO_IVY_UITESTAGING_H

#include "UIBase.h"

namespace UI {


    class UITestAging : public Base {
    public:
        explicit UITestAging(ObjPtr obj);
        void update(uint32_t time, int progress, uint32_t time_left, const char *content, lv_color_t bck_color = lv_color_black(),
                    lv_color_t text_color = lv_color_white());

        void routine() override;

        void update_confirm_progress(int progress);

    private:
        bool _initialize() override;

    private:
        lv_obj_t *m_time_label;
        lv_obj_t *m_test_anim;
        lv_obj_t *m_charging_bar;
        lv_obj_t *m_charging_label;
        lv_obj_t *m_timeout_label;
        lv_obj_t *m_confirm_bar;
        lv_obj_t *m_confirm_label;

        bool is_confirmed = false;
    };

}


#endif //PLANTSIO_IVY_UITESTAGING_H
