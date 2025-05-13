//
// Created by Huwensong on 2025/1/14.
//

#ifndef IVY2_0_UIOTA_H
#define IVY2_0_UIOTA_H

#include "UIBase.h"
#include "Progress.h"

#include "base_event.h"

namespace UI
{
    class UIOTA : public Base
    {
    public:
        explicit UIOTA(ObjPtr obj);

    private:
        bool _initialize() override;

        void routine() override;

        void prevFinish();

    private:
        Progress mProgress;
    };
}




#endif //IVY2_0_UIOTA_H
