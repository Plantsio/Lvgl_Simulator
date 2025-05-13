//
// Created by Gordon on 2022/3/7.
//

#ifndef LVGL_UILOADING_H
#define LVGL_UILOADING_H

#include "UIBase.h"

namespace UI {
    class UILoading : public Base {
    public:
        explicit UILoading(ObjPtr obj);

        void update(const char *title, const char *desc);

        void update_desc(const char *desc);

    private:
        lv_obj_t *m_title_label;
        lv_obj_t *m_loading_spinner;
        lv_obj_t *m_desc_label;
    };
}

#endif //LVGL_UILOADING_H
