//
// Created by Huwensong on 2025/5/28.
//

#ifndef SIMULATOR_FONTTTF_H
#define SIMULATOR_FONTTTF_H

#include "UIBase.h"

namespace UI
{
    class FontTTF : public Base
    {
    public:
        FontTTF();

    private:
        bool _initialize() override;

    private:
        lv_obj_t *label_30;
        lv_obj_t *label_80;
        lv_font_t *font_30 = nullptr;
        lv_font_t *font_80 = nullptr;
    };
}



#endif //SIMULATOR_FONTTTF_H
