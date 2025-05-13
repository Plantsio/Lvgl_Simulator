//
// Created by Huwensong on 2023/3/31.
//

#ifndef LVGL_UILANGSELECT_H
#define LVGL_UILANGSELECT_H

#include <string>
#include "UIBase.h"

namespace UI
{
    class UILangSelect : public Base
    {
    public:
        explicit UILangSelect(ObjPtr obj);

        ~UILangSelect();

    public:
        void routine() override;

        void left_input_cb(InputData data) override;

		void right_input_cb(InputData data) override;

    private:
        bool _initialize() override;

        void set_roller_style();

        void set_text_style();

        void front_input_cb(InputData data) override;

        void update();

        bool store_factory();

    private:
        lv_obj_t *m_roller;

        lv_obj_t *m_title;

        lv_obj_t *m_ok;

        lv_obj_t *m_front_touch;

        std::string m_lang_list;

        int m_lang_index = 1;
    };
}

#endif //LVGL_UILANGSELECT_H
