//
// Created by Huwensong on 2025/5/30.
//

#ifndef IVY2_0_LOTTIE_H
#define IVY2_0_LOTTIE_H

#include "UIBase.h"

namespace UI
{
    class Lottie : public Base
    {
    public:
        Lottie();

    private:
        bool _initialize() override;

    private:
        lv_obj_t *lottie;
    };
}






#endif //IVY2_0_LOTTIE_H
