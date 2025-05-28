//
// Created by Huwensong on 2023/6/8.
//

#include "UITomatoClock.h"
//#include "tools.h"
#include "ThemeInterface.h"

#include "Lang.h"
#include "log.h"

//LV_IMG_DECLARE(TomatoFruit)
//LV_IMG_DECLARE(TomatoLeaf)
//LV_IMG_DECLARE(TomatoTotal)
//LV_IMG_DECLARE(TomatoSum)

#define TIME_BASE      60

#define CLOCK_MINUTES  25
#define CLOCK_SECONDS  0

#define GROWTH_PERIOD  ((CLOCK_MINUTES * TIME_BASE + CLOCK_SECONDS) * 1000)

#define STAGE_GROWTH_PERIOD_MINUTES 5
#define STAGE_GROWTH_PERIOD_SECONDS 0

#define REST_PERIOD_MINUTES         5
#define REST_PERIOD_SECONDS         0

#define TIMER_PERIOD   1000

namespace UI
{
    UITomatoClock::UITomatoClock(ObjPtr obj) : Base(std::move(obj)),
    mIndicator(m_scr),
    m_img_tomato_fruit(lv_img_create(m_scr)),
    m_img_tomato_leaf(lv_img_create(m_scr)),
    time_label(lv_label_create(m_scr)),
    m_top_text(m_scr),
    m_bottom_text(m_scr)
    {
//        lv_img_set_src(m_img_tomato_fruit,&TomatoFruit);
        lv_obj_align(m_img_tomato_fruit, LV_ALIGN_CENTER, 0, 0);

//        lv_img_set_src(m_img_tomato_leaf,&TomatoLeaf);
        lv_obj_align_to(m_img_tomato_leaf,m_img_tomato_fruit,LV_ALIGN_TOP_RIGHT,-19,-8);


        lv_obj_set_width(time_label,160);
        lv_obj_align_to(time_label,m_img_tomato_fruit,LV_ALIGN_CENTER,0,-10);
        lv_obj_set_style_text_font(time_label,&lv_font_montserrat_48,0);
        lv_obj_set_style_text_color(time_label,lv_color_white(),0);
        lv_obj_set_style_text_align(time_label,LV_TEXT_ALIGN_CENTER,0);

        //lv_obj_align(m_top_text.get_origin_obj(),LV_ALIGN_TOP_MID,0,10);
		//THEME_SET_FONT_SIZE(m_top_text.get_origin_obj(),16);

        //lv_obj_align(m_bottom_text.get_origin_obj(),LV_ALIGN_BOTTOM_MID,0,-15);
        //lv_obj_set_width(m_bottom_text.get_origin_obj(),200);
        //lv_label_set_long_mode(m_bottom_text.get_origin_obj(),LV_LABEL_LONG_SCROLL_CIRCULAR);
		//THEME_SET_FONT_SIZE(m_bottom_text.get_origin_obj(),16);

        m_clock_seconds = CLOCK_MINUTES * TIME_BASE + CLOCK_SECONDS;

        check_tomato_num();

        mIndicator.enable_visible(Indicator::ALL);
    }

    UITomatoClock::~UITomatoClock()
    {

    }

    void UITomatoClock::routine()
    {
        update_time();
        update_text();
    }

    bool UITomatoClock::_handleInput(InputEvtType &&input)
    {
        return true;
    }


    int UITomatoClock::last_start_sync_time = 0;

    void UITomatoClock::set_sync_time(int time)
    {
        last_start_sync_time = time;
    }


    void UITomatoClock::init_color_list()
    {
        m_color_list = {
                {lv_color_make(128,224,45)},
                {lv_color_make(179,221,47)},
                {lv_color_make(237,231,38)},
                {lv_color_make(255,206,51)},
                {lv_color_make(255,104,51)},
                {lv_color_make(255,51,51)},
        };
    }

    void UITomatoClock::color_anim_create(int32_t value_start,int32_t value_end,uint32_t ms)
    {
        lv_anim_init(&m_color_anim);
        lv_anim_set_var(&m_color_anim, this);
        lv_anim_set_exec_cb(&m_color_anim, color_grad_wrapper);
        lv_anim_set_values(&m_color_anim, value_start, value_end);
        lv_anim_set_time(&m_color_anim, ms);

        lv_anim_set_path_cb(&m_color_anim,lv_anim_path_linear);

        lv_anim_set_user_data(&m_color_anim, this);

        lv_anim_set_repeat_count(&m_color_anim, m_color_list.size() - 1);

    }

    void UITomatoClock::color_grad(uint32_t value)
    {
        lv_color_t start_color;
        lv_color_t target_color;

        lv_anim_t *anim_p = lv_anim_get(this,color_grad_wrapper);

        if (anim_p || m_tomato_status == tomato_status_start)
        {

            m_tomato_stage = m_color_list.size() - 1 - anim_p->repeat_cnt;


            start_color = m_color_list.at(m_tomato_stage);
            target_color = m_color_list.at(m_tomato_stage + 1);

            lv_color_t grad_color = lv_color_mix(start_color,target_color,value);

            lv_obj_set_style_img_recolor(m_img_tomato_fruit, grad_color, 0);
            lv_obj_set_style_img_recolor_opa(m_img_tomato_fruit, LV_OPA_COVER, 0);
        }
    }

    void UITomatoClock::color_grad_wrapper(void *var,int32_t value)
    {
        static_cast<UITomatoClock *>(var)->color_grad(value);
    }

    void UITomatoClock::timer_cb()
    {
        if (m_tomato_status == tomato_status_start)
        {
            m_clock_seconds --;
            if (m_clock_seconds == 0)
            {
                m_tomato_status = tomato_status_ripen;
                m_tomato_ripen_num ++;
//                Prop::set(Prop::tomato_today,m_tomato_ripen_num, false, false, true);
//                m_tomato_ripen_total ++;
//                Prop::set<int>(Prop::pomodoro,m_tomato_ripen_total, false, true, true);
               mIndicator.enable_visible(Indicator::ALL);
                set_clock_time(REST_PERIOD_MINUTES * TIME_BASE + REST_PERIOD_SECONDS);
            }

        }

        if (m_tomato_status == tomato_status_rest)
        {
            m_clock_seconds --;
            if (m_clock_seconds == 0)
            {
                stop_rest_clock();
            }
        }
    }

    void UITomatoClock::timer_wrapper(lv_timer_t *timer)
    {
        auto instance = static_cast<UITomatoClock *>(lv_timer_get_user_data(timer));

        instance->timer_cb();
    }

    void UITomatoClock::update_time()
    {
        char time_str[6];

        sprintf(time_str,"%02u:%02u",m_clock_seconds / TIME_BASE,m_clock_seconds % TIME_BASE);

        lv_label_set_text(time_label,time_str);
    }

    void UITomatoClock::update_text()
    {
        switch (m_tomato_status)
        {
            case tomato_status_stop:
            case tomato_status_init:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_ready));
                m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_start));
                break;
            case tomato_status_start:
                update_stage_text();
                m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_stop));
                break;
            case tomato_status_ripen:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_finish));
                m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_rest));
                break;
            case tomato_status_rest:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_relax));
                m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_skip));
                break;
            default:
                break;
        }
    }

    void UITomatoClock::update_stage_text()
    {
        switch (m_tomato_stage)
        {
            case 0:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_attention));
                break;
            case 1:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_focus));
                break;
            case 2:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_keep));
                break;
            case 3:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_come));
                break;
            case 4:
                m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tomato_last));
                break;
            default:
                break;
        }
    }


    void UITomatoClock::clock_start()
    {
        m_tomato_status = tomato_status_start;

        //m_front_used = false;

        //switch_enable = false;

//        SysOld::unset_condition(SysOld::SLEEP_ENABLE);
//
//        IvyBody::instance().unregister_button_tap(BUTTON_DELAY_TAP);

        set_clock_time(CLOCK_MINUTES * TIME_BASE + CLOCK_SECONDS);

        mIndicator.disable_visible(Indicator::LEFT);

        uint32_t anim_time = (STAGE_GROWTH_PERIOD_MINUTES * TIME_BASE + STAGE_GROWTH_PERIOD_SECONDS) * 1000;

        color_anim_create(LV_OPA_COVER,LV_OPA_TRANSP,anim_time);

        lv_anim_start(&m_color_anim);
    }

    void UITomatoClock::clock_stop()
    {
        m_tomato_status = tomato_status_stop;

        set_clock_time(CLOCK_MINUTES * TIME_BASE + CLOCK_SECONDS);

        mIndicator.enable_visible(Indicator::ALL);

        lv_obj_set_style_img_recolor(m_img_tomato_fruit, m_color_list.at(0), 0);
        lv_obj_set_style_img_recolor_opa(m_img_tomato_fruit, LV_OPA_COVER, 0);

        lv_anim_del(this,color_grad_wrapper);

        //m_front_used = true;

       // switch_enable = true;

//        IvyEngine::instance().refresh_last_active_t();
//
//        SysOld::set_condition(SysOld::SLEEP_ENABLE);
//
//        IvyBody::instance().register_button_tap(BUTTON_DELAY_TAP);
    }

    void UITomatoClock::start_rest_clock()
    {
        m_tomato_status = tomato_status_rest;

        mIndicator.disable_visible(Indicator::LEFT);
    }

    void UITomatoClock::stop_rest_clock()
    {
        m_tomato_status = tomato_status_init;

        //m_front_used = true;

        //switch_enable = true;

//        IvyEngine::instance().refresh_last_active_t();
//        SysOld::set_condition(SysOld::SLEEP_ENABLE);

        set_clock_time(CLOCK_MINUTES * TIME_BASE + CLOCK_SECONDS);
        lv_obj_set_style_img_recolor(m_img_tomato_fruit, m_color_list.at(0), 0);
        lv_obj_set_style_img_recolor_opa(m_img_tomato_fruit, LV_OPA_COVER, 0);
    }

    void UITomatoClock::set_clock_time(uint32_t time)
    {
        m_clock_seconds = time;
    }

    void UITomatoClock::check_tomato_num()
    {
        int start_time,cur_time;

//        cur_time = get_ymd(get_epoch());
//
//        start_time = Prop::get<int>(Prop::start_sync_time);
//
//        log_d(" last time = %d, start time = %d, cur time = %d",last_start_sync_time,start_time,cur_time);
//
//        if (cur_time > start_time || cur_time > last_start_sync_time)
//        {
//            m_tomato_ripen_num = 0;
//            Prop::set<int>(Prop::tomato_today,m_tomato_ripen_num, false, false,true);
//        }
//
//        m_tomato_ripen_num = Prop::get<int>(Prop::tomato_today);
//
//        m_tomato_ripen_total = Prop::get<int>(Prop::pomodoro);
    }

    bool UITomatoClock::_initialize()
    {
        init_color_list();

        m_timer = lv_timer_create(timer_wrapper,TIMER_PERIOD, this);
        lv_timer_ready(m_timer);
        return true;
    }

    bool UITomatoClock::_deInitialize()
    {
        if (m_timer != nullptr)
            lv_timer_del(m_timer);

        lv_anim_del(this,color_grad_wrapper);

        //m_front_used = true;

        //switch_enable = true;

        return true;
    }

    UITomatoStatistics::UITomatoStatistics(ObjPtr obj) : Base(std::move(obj)),
    mIndicator(m_scr),
    m_lay_total(lv_obj_create(m_scr)),
    m_img_tomato_total(lv_img_create(m_lay_total)),
    m_img_tomato_sum(lv_img_create(m_scr)),
    m_label_tomato_today(lv_label_create(m_img_tomato_sum)),
    m_label_top_text(lv_label_create(m_lay_total)),
    m_label_top_total(lv_label_create(m_lay_total)),
    m_label_bottom_text(lv_label_create(m_scr))
    {
        lv_obj_set_size(m_lay_total, 185, 35);
        lv_obj_set_style_bg_opa(m_lay_total, LV_OPA_20, 0);
		lv_obj_clear_flag(m_lay_total, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(m_lay_total, 10, 0);
        lv_obj_align(m_lay_total, LV_ALIGN_TOP_MID, 0, 20);

//        lv_img_set_src(m_img_tomato_total,&TomatoTotal);
        lv_obj_align(m_img_tomato_total, LV_ALIGN_CENTER, 20, 0);

//        lv_img_set_src(m_img_tomato_sum,&TomatoSum);
        lv_obj_align(m_img_tomato_sum, LV_ALIGN_CENTER, -5, 10);

        lv_obj_align(m_label_tomato_today, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_text_font(m_label_tomato_today,&lv_font_montserrat_48,0);
        lv_obj_set_style_text_color(m_label_tomato_today,lv_color_white(),0);
        lv_obj_set_style_text_align(m_label_tomato_today,LV_TEXT_ALIGN_CENTER,0);

        lv_obj_align(m_label_top_text, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_align(m_label_top_total, LV_ALIGN_RIGHT_MID, -5, 0);


		THEME_SET_FONT_SIZE(m_label_top_text,14);
		THEME_SET_FONT_SIZE(m_label_top_total,16);


        lv_obj_align(m_label_bottom_text, LV_ALIGN_BOTTOM_MID, 0, -10);
		THEME_SET_FONT_SIZE(m_label_bottom_text,30);

        //m_input_used = true;
        mIndicator.enable_visible(Indicator::RIGHT);
    }

    bool UITomatoStatistics::_initialize()
    {
//        m_tomato_today = Prop::get<int>(Prop::tomato_today);  // 获取番茄总数
//
//        m_tomato_total = Prop::get<int>(Prop::pomodoro);  // 获取历史番茄总数

        update_text();

        return true;
    }

    bool UITomatoStatistics::_handleInput(InputEvtType &&input)
    {
        return true;
    }

    void UITomatoStatistics::update_text()
    {
        std::string top_text = THEME_TEXT_CONTENT(Lang::ui_tomato_total) + " :";

        std::string top_total = "x " + std::to_string(m_tomato_total);

        std::string tomato_num = std::to_string(m_tomato_today);

        lv_label_set_text(m_label_top_text,top_text.c_str());

        lv_label_set_text(m_label_top_total,top_total.c_str());

        lv_label_set_text(m_label_tomato_today,tomato_num.c_str());

        lv_label_set_text(m_label_bottom_text, THEME_TEXT_CONTENT(Lang::ui_tomato_today).c_str());
    }


}

