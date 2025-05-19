//
// Created by Huwensong on 2025/5/19.
//

#ifndef SIMULATOR_INDEV_H
#define SIMULATOR_INDEV_H

#include "lvgl.h"

#include <functional>
#include "GuiDefines.h"

class Indev
{
public:
    using MouseCB = std::function<bool(InputEvtType &input)>;

public:
    void attachIndev(lv_obj_t *obj,MouseCB cb);

private:
    static void mouse_cb(lv_event_t *event);

private:
    MouseCB mCB = nullptr;
};


#endif //SIMULATOR_INDEV_H
