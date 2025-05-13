//
// Created by Gordon on 2025/4/24.
//

#ifndef IVY2_0_UIQUIT_H
#define IVY2_0_UIQUIT_H

#include "UIBase.h"

namespace UI {
    class UIQuit: public Base{
    public:
        explicit UIQuit(Base::ObjPtr obj);

        bool _initialize() override;

    };
}

#endif //IVY2_0_UIQUIT_H
