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

		void left_input_cb(InputData data) override;

		void right_input_cb(InputData data) override;

    private:
        lv_obj_t *m_table;

    };
}

#endif //PLANTSIO_IVY_UISYSINFO_H
