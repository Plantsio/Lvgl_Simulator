//
// Created by Huwensong on 2025/5/19.
//

#include "Indev.h"
#include "log.h"

void Indev::attachIndev(lv_obj_t *obj,MouseCB cb)
{
    log_d("111111111");
    lv_obj_add_event_cb(obj, mouse_cb, LV_EVENT_CLICKED, this);
    mCB = std::move(cb);
    if (mCB)
        log_d("cbcbcbcbcbcb");
    log_d("22222222");
}

void Indev::mouse_cb(lv_event_t *event)
{
    log_d("1111111111");
    lv_indev_t *indev = lv_indev_get_act();
    lv_indev_type_t indev_type = lv_indev_get_type(indev);

    if (indev_type == LV_INDEV_TYPE_POINTER)
    {
        lv_point_t point;
        lv_indev_get_point(indev, &point);
        auto input_cb = static_cast<Indev *>(event->user_data)->mCB;
        if (!input_cb)
            return;

        if (point.x < 100)
        {
            InputEvtType input = {BtnSrcLeft,{ButtonPress,1}};
            input_cb(input);
        }
        else if (point.x > 220)
        {
            InputEvtType input = {BtnSrcRight,{ButtonPress,1}};
            input_cb(input);
        }
        else if (point.x > 100 && point.x < 220)
        {
            InputEvtType input = {BtnSrcFront,{ButtonPress,1}};
            input_cb(input);
        }
        else
        {
            ; //
        }
    }
}