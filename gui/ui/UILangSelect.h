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

    private:
        bool _initialize() override;

        bool _handleInput(InputEvtType &&input) override;

        void set_roller_style();

        void set_text_style();

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
