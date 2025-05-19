//
// Created by Gordon on 2021/8/13.
//

#ifndef PLANTSIO_IVY_BASE_H
#define PLANTSIO_IVY_BASE_H

/**todo:  将程序移植回工程时不要复制头文件**/

#include <vector>
#include <any>

#include "lvgl.h"
#include "DeInitializable.h"
#include "GuiDefines.h"

#include "Indev.h"

namespace UI {
    class Base : public DeInitializable {
    protected:
        using ObjPtr = std::shared_ptr<Gui::GuiObj>;
    public:
        /* every derived UI class shall implement its obj creation in constructor */
        explicit Base(ObjPtr obj);

        ~Base() override;

    public:
        /* called every frame */
        virtual void routine() {};

        virtual bool _handleInput(InputEvtType &&input) { return true; };

        bool load();

        void dismiss();

        bool externalClose(Gui::GuiDismissCb &&cb = nullptr);

    private:
        bool _initialize() override { return true; };

        bool _deInitialize() override { return true; }

    protected:
        lv_obj_t *m_scr = nullptr;

        uint32_t m_input_mask = 0;

        ObjPtr m_obj;    /* todo would it be better to use weakPtr */

        Indev mIndev;
    };

/* Default empty UI class */
    class UIDefault : public Base {
    public:
        using Base::Base;
    };
}
#endif //PLANTSIO_IVY_BASE_H
