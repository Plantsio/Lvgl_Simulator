//
// Created by Gordon on 2021/8/13.
//

#ifndef PLANTSIO_IVY_BASE_H
#define PLANTSIO_IVY_BASE_H

#include <vector>
#include <any>

#include "lvgl.h"
#include "gui_config.h"
#include "event_config.h"
#include "DeInitializable.h"
#include "GuiObj.h"

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

        void enable_input(uint32_t input);

        void disable_input(uint32_t input);

        void touch_input_handler(InputSource source, InputData data);

        bool load();

        void dismiss();

        [[nodiscard]] uint32_t get_input_mask() const;

        bool externalClose(Gui::GuiDismissCb &&cb = nullptr);

    protected:
        /* bound to touch events */
        virtual void left_input_cb(InputData data) {};

        virtual void right_input_cb(InputData data) {};

        virtual void front_input_cb(InputData data) {};

        virtual void back_input_cb(InputData data) {};

        virtual void plant_input_cb(InputData data) {};

    private:
        bool _initialize() override { return true; };

        bool _deInitialize() override { return true; }

    protected:
        lv_obj_t *m_scr = nullptr;

        uint32_t m_input_mask = 0;

        ObjPtr m_obj;    /* todo would it be better to use weakPtr */
    };

/* Default empty UI class */
    class UIDefault : public Base {
    public:
        using Base::Base;
    };
}
#endif //PLANTSIO_IVY_BASE_H
