//
// Created by Gordon on 2021/12/2.
//

#ifndef PLANTSIO_IVY_UISYSINFO_H
#define PLANTSIO_IVY_UISYSINFO_H

#include "UIBase.h"
namespace UI {
#define SYS_UI_HEIGHT       200

    class UISysInfo : public Base {
    public:
        explicit UISysInfo(ObjPtr obj);

        void update_info();

    private:
        bool _handleInput(InputEvtType &&input) override;

    private:
        lv_obj_t *m_table;

    };
}

#endif //PLANTSIO_IVY_UISYSINFO_H
