//
// Created by Huwensong on 2023/6/8.
//

#ifndef LVGL_UITOMATOCLOCK_H
#define LVGL_UITOMATOCLOCK_H

#include "lvgl.h"

#include "UIBase.h"
#include "BasicText.h"
#include "Indicator.h"

namespace UI
{
    typedef enum TomatoStatus
    {
        tomato_status_init,
        tomato_status_start,
        tomato_status_ripen,
        tomato_status_stop,
        tomato_status_rest,
    }tomato_status_t;

    class UITomatoClock : public Base
    {
    public:
        explicit UITomatoClock(ObjPtr obj);
        ~UITomatoClock();

    public:
        /* called every frame */
        void routine() override;

        /* bound to touch events */
		bool _handleInput(InputEvtType &&input) override;

        static void set_sync_time(int time);

    private:
        void init_color_list();

        void color_anim_create(int32_t value_start,int32_t value_end,uint32_t ms);

        void color_grad(uint32_t value);

        static void color_grad_wrapper(void *var,int32_t value);

        void timer_cb();

        static void timer_wrapper(lv_timer_t *timer);

        void update_time();

        void update_text();

        void update_stage_text();

        void clock_start();

        void clock_stop();

        void start_rest_clock();

        void stop_rest_clock();

        void set_clock_time(uint32_t time);

        void check_tomato_num();

        bool _initialize() override;

        bool _deInitialize() override;


    private:
        Indicator mIndicator;

        lv_obj_t *m_img_tomato_fruit;
        lv_obj_t *m_img_tomato_leaf;
        lv_obj_t *time_label;
        lv_anim_t m_color_anim;
        lv_timer_t *m_timer = nullptr;

        std::vector<lv_color_t> m_color_list;

        BasicText m_top_text;
        BasicText m_bottom_text;

        uint8_t m_tomato_stage = 0;
        uint8_t m_clock_seconds = 0;
        uint32_t m_tomato_ripen_num;
        uint32_t m_tomato_ripen_total;

        tomato_status_t m_tomato_status = tomato_status_init;
        static int last_start_sync_time;
    };

    class UITomatoStatistics : public Base
    {
    public:
        explicit UITomatoStatistics(ObjPtr obj);

    public:
		bool _handleInput(InputEvtType &&input) override;

    private:
        void update_text();

        bool _initialize() override;

    private:
        Indicator mIndicator;

        lv_obj_t *m_lay_total;
        lv_obj_t *m_img_tomato_total;
        lv_obj_t *m_img_tomato_sum;
        lv_obj_t *m_label_tomato_today;
        lv_obj_t *m_label_top_text;
        lv_obj_t *m_label_top_total;
        lv_obj_t *m_label_bottom_text;

        uint32_t m_tomato_total = 0;
        uint32_t m_tomato_today = 0;
    };
}


#endif //LVGL_UITOMATOCLOCK_H
