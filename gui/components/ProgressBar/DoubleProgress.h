//
// Created by Gordon on 2022/2/24.
//

#ifndef LVGL_UIDOUBLEPROGRESS_H
#define LVGL_UIDOUBLEPROGRESS_H

#include "UIBase.h"
namespace UI {
    class DoubleProgress
    {
    public:
        explicit DoubleProgress(lv_obj_t *_parent);

        void
        update(const char *title, const char *status_1, uint8_t progress_1, const char *status_2, uint8_t progress_2);

        void update_progress_1(const char *status_1, uint8_t progress_1);

        void update_progress_2(const char *status_2, uint8_t progress_2);

        void update_desc(const char *desc);

    private:
        lv_obj_t *m_bar_1;
        lv_obj_t *m_bar_2;
        lv_obj_t *m_title_label;
        lv_obj_t *m_status_label_1;
        lv_obj_t *m_status_label_2;
        lv_obj_t *m_desc_label;

    };
}
#endif //LVGL_UIDOUBLEPROGRESS_H
