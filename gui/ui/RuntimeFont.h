//
// Created by Huwensong on 2025/6/13.
//

#ifndef SIMULATOR_RUNTIMEFONT_H
#define SIMULATOR_RUNTIMEFONT_H

#include "UIBase.h"

namespace UI
{
    class RuntimeFont : public Base
    {
    public:
        RuntimeFont();

    private:
        bool _initialize() override;

    private:
        lv_obj_t *label_runtime1;
        lv_obj_t *label_runtime2;
        lv_obj_t *label_font;
    };
}

#endif //SIMULATOR_RUNTIMEFONT_H
