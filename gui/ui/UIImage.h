//
// Created by Huwensong on 2025/7/7.
//

#ifndef IVY2_0_UIIMAGE_H
#define IVY2_0_UIIMAGE_H

#include "UIBase.h"

namespace UI
{
    class UIImage : public Base
    {
    public:
        explicit UIImage();

    private:
        bool _initialize() override;

    private:
        lv_obj_t *image_1;
        lv_obj_t *image_2;
    };

} // UI

#endif //IVY2_0_UIIMAGE_H
