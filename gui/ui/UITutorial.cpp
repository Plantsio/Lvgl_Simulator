//
// Created by Gordon on 2022/5/6.
//

#include "UITutorial.h"

#include <memory>
#include "ThemeInterface.h"
#include "UIProvTip.h"
#include "Lang.h"
#include "log.h"



namespace UI
{
	//region intro
	TuIntro::TuIntro(lv_obj_t *parent) :
    TuBase(),
    mMainText(parent),
    mSubText(parent)
	{
        mMainText.set_font_size(30);
        mSubText.set_font_size(16);
        mMainText.align( LV_ALIGN_CENTER, 0, -20);
        mSubText.alignTo( mMainText, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	}

    bool TuIntro::_initialize()
    {
        registerStepCB([&](){
            update(THEME_TEXT_CONTENT(Lang::ui_tu_intro_hi),"");
            return true;
        });
        registerStepCB([&](){
            update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_ivy));
            return true;
        });
        registerStepCB([&](){
            update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_meet));
            return true;
        });
        registerStepCB([&](){
            update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_before_enter));
            return true;
        });
        registerStepCB([&](){
            update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_ready));
            return true;
        });
        registerStepCB([&](){
            update_sub(THEME_TEXT_IN_COLOR(Lang::ui_tu_intro_shutdown,Theme::palette_notice));
            return true;
        });

        registerStepCB([&](){
            update(THEME_TEXT_CONTENT(Lang::ui_tu_skip),"");
            return true;
        });

        registerStepCB([&](){
            update("", "");
            return true;
        });

        enableAutoStep();

        return true;
    }

	void TuIntro::update(const std::string &main, const std::string &sub)
	{
		update_main(main);
		update_sub(sub);
	}

	void TuIntro::update_main(const std::string &main)
	{
        mMainText.update(main);
	}

	void TuIntro::update_sub(const std::string &sub)
	{
        mSubText.update(sub);
	}

	//endregion

	TuTouchBar::TuTouchBar(lv_obj_t *parent) :
    TuBase(),
    mTopText(parent),
    mBottomText(parent),
    mIndicator(parent),
    mImage(parent)
	{
        mTopText.set_font_size(16);
        mBottomText.set_font_size(16);

        mTopText.align(LV_ALIGN_TOP_MID, 0, 10);
        mBottomText.align(LV_ALIGN_BOTTOM_MID, 0, -20);

		for (auto &circle: m_circles)
		{
			circle = lv_arc_create(parent);
			lv_obj_set_size(circle, 50, 50);
			lv_obj_set_style_arc_width(circle, 2, 0);
			lv_obj_set_style_arc_width(circle, 2, LV_PART_INDICATOR);
			lv_arc_set_rotation(circle, 0);
			lv_arc_set_value(circle, 0);
			lv_arc_set_bg_angles(circle, 0, 0);
			lv_obj_set_style_arc_color(circle, Theme::getPaletteRGB(Theme::palette_failure), 0);
			lv_obj_remove_style(circle, nullptr, LV_PART_KNOB);
			lv_obj_clear_flag(circle, LV_OBJ_FLAG_CLICKABLE);
		}
		lv_obj_align(m_circles[0], LV_ALIGN_CENTER, -80, 10);
		lv_obj_align(m_circles[1], LV_ALIGN_CENTER, 80, 10);
	}

    TuTouchBar::~TuTouchBar()
    {
        for (auto &circle: m_circles)
            lv_obj_del(circle);
    }

    bool TuTouchBar::_initialize()
    {
        registerStepCB([&](){
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_interact));
            updateStepPeriod(3000);
            return true;
        });

        registerStepCB([&](){
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_two));
            mImage.update("tu/touch_bar");
            return true;
        });

        registerStepCB([&](){
            for (auto &circle: m_circles)
            {
                Animation::anim_create(circle, reinterpret_cast<lv_anim_exec_xcb_t>(lv_arc_set_bg_end_angle), 0, 360,
                                       2000);
            }
            updateStepPeriod(COMMON_TIMER_DELAY);
            return true;
        });

        registerStepCB([&](){
            mTopText.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_touch_bar_yellow_dot,Theme::palette_notice));
            for (auto &circle: m_circles)
            {
                Animation::anim_fade_out(circle, 2000,0);
            }
            updateStepPeriod( 4000);
            return true;
        });

        registerStepCB([&](){
            mIndicator.enable_visible_flashing(Indicator::ALL,1000,0,1000,800,2);
            updateStepPeriod( 5000);
            return true;
        });

        registerStepCB([&](){
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_left));
            mIndicator.enable_visible(Indicator::LEFT,true,1000,3000);
            updateStepPeriod( 10);
            return true;
        });

        registerStepCB([&](){
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_right));
            mIndicator.disable_visible(Indicator::LEFT, true, 600);
            mIndicator.enable_visible(Indicator::RIGHT, true, 600);
            return true;
        },[&](){ return mInputEvtType.index == BtnSrcLeft;});

        registerStepCB([&](){
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_great));
            mIndicator.disable_visible(Indicator::RIGHT, true, 600);
            updateStepPeriod( 2000);
            return true;
        },[&](){ return mInputEvtType.index == BtnSrcRight;});

        enableAutoStep();

        return true;
    }

    void TuTouchBar::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuApp::TuApp(lv_obj_t *parent) :
    TuBase(),
    mTopText(parent),
    mSubText(parent, 300),
    mMidText(parent, 300, 800, 600),
    mBottomText(parent),
    mIndicator(parent),
    mImage(parent)
	{
        mTopText.set_font_size(16);
        mSubText.set_font_size(16);
        mMidText.set_font_size(30);
        mBottomText.set_font_size(16);

        mTopText.align(LV_ALIGN_TOP_MID, 0, 10);
        mSubText.alignTo(mTopText,LV_ALIGN_OUT_BOTTOM_MID, 0,10);
        mMidText.align(LV_ALIGN_CENTER, 0, -20);
        mBottomText.align(LV_ALIGN_BOTTOM_MID, 0, -20);
	}

    bool TuApp::_initialize()
    {
        registerStepCB([&](){
            mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_add_app));
            updateStepPeriod(3000);
            return true;
        });

        registerStepCB([&](){
            int vendor = 0;                               //todo: vendor 是否还有需求
            mMidText.update("");
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_prov_in_store));

            std::string download_app = THEME_TEXT_OPTION(Lang::ui_tu_download_app, vendor);
            mSubText.update(Theme::getPaletteText(download_app,Theme::palette_notice));

            mImage.update("tu/app_60");
            return true;
        });

        registerStepCB([&](){
            mIndicator.enable_visible(Indicator::RIGHT, true,1000,1200);
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next));
            updateStepPeriod(10);
            return true;
        });

        registerStepCB([&](){
            mIndicator.disable_visible(Indicator::RIGHT, true,500);
            mBottomText.update("");
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_in_system_setting));
            mSubText.update(THEME_TEXT_IN_COLOR(Lang::ui_enable_ble,Theme::palette_notice));
            std::string str = THEME_TEXT_IN_COLOR(Lang::ui_enable_ble,Theme::palette_notice);
            mImage.update("tu/bluetooth");
            updateStepPeriod( 3000);
            return true;
        },[&](){ return mInputEvtType.index == BtnSrcRight;});

        registerStepCB([&](){
            mIndicator.enable_visible(Indicator::RIGHT, true,1000,1200);
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_touch_right_config));
            return true;
        });

        enableAutoStep();
        return true;
    }

    void TuApp::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuWater::TuWater(lv_obj_t *parent):
    TuBase(),
    mTopText(parent),
    mIndicator(parent),
    mImage(parent)
	{
        mTopText.set_font_size(16);
        mTopText.align( LV_ALIGN_TOP_MID, 0, 15);
	}

    bool TuWater::_initialize()
    {
        registerStepCB([&](){
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_prepare_plant));
            updateStepPeriod(4500);
            return true;
        });

        registerStepCB([&](){
            mTopText.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tu_water_remove_pot,Theme::palette_notice));
            mImage.update("tu/add_water");
            return true;
        });

        registerStepCB([&](){
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next));
            mIndicator.enable_visible(Indicator::RIGHT,true,1000,1500);
            updateStepPeriod(10);
            return true;
        });

        registerStepCB([&](){
            mTopText.update("THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next)");
            mIndicator.disable_visible(Indicator::RIGHT,true,1000,1500);
            updateStepPeriod(10);
            return true;
        },[&](){ return mInputEvtType.index == BtnSrcRight;});

        enableAutoStep();
        return true;
    }

    void TuWater::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuWaterAssist::TuWaterAssist(lv_obj_t *parent) :
    TuBase(),
    mTopText(parent),
	mMidText(parent),
    mBottomText(parent),
    mIndicator(parent)
	{
        mTopText.set_font_size(16);
        mMidText.set_font_size(30);
        mBottomText.set_font_size(16);

        mTopText.align( LV_ALIGN_TOP_MID, 0, 15);
        mMidText.align(LV_ALIGN_CENTER, 0, -30);
        mBottomText.align(LV_ALIGN_BOTTOM_MID, 0, -20);
	}

    bool TuWaterAssist::_initialize()
    {
        registerStepCB([&](){
            mTopText.update(THEME_FORMAT_TEXT<int>(Lang::ui_fluid_add_about, {50}));
            return true;
        });

        registerStepCB([&](){
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
            mIndicator.enable_visible(Indicator::RIGHT,true,1000,4500);
            updateStepPeriod(10);
            return true;
        });

        auto curReady = [](){return true;};
        auto nextReady = [&](){
            bool isNext = waterLevel > 0 || mInputEvtType.index == BtnSrcRight;
            if (isNext)
                updateStepPeriod(COMMON_TIMER_DELAY);
            return isNext;
        };
        registerStepCB([&](){
            waterLevel = SystemStore::get<int>(DeviceDp::waterlevel);

            if (waterLevel <= 0)
                mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_no_detected));
            else
                mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_detected));

            return true;
        },curReady,nextReady);

        registerStepCB([&](){
            mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_assist_good));
            return true;
        });

        enableAutoStep();
        return true;
    }

    void TuWaterAssist::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuPlantDetect::TuPlantDetect(lv_obj_t *parent) :
    TuBase(),
    mTopText(parent),
    mIndicator(parent),
    mImage(parent)
	{
        mTopText.set_font_size(16);
        mTopText.align( LV_ALIGN_TOP_MID, 0, 15);
	}

    bool TuPlantDetect::_initialize()
    {
        auto curReady = [](){return true;};
        auto nextReady = [&](){
            return plantExist || mInputEvtType.index == BtnSrcRight;
        };
        registerStepCB([&](){
            plantExist = SystemStore::get<bool>(DeviceDp::plantExist);
            mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_plant_put_pot));
            mImage.update("tu/put_in");
            mIndicator.enable_visible(Indicator::RIGHT, true, 1000, 1500);
            updateStepPeriod(10);
            return true;
        },curReady,nextReady);

        enableAutoStep();
        return true;
    }

    void TuPlantDetect::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuFinal::TuFinal(lv_obj_t *parent) :
    TuBase(),
	mMidText(parent)
	{
        mMidText.set_font_size(30);
        mMidText.align(LV_ALIGN_CENTER, 0, 0);
	}

    bool TuFinal::_initialize()
    {
        registerStepCB([&](){
            mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_congratulations));
            return true;
        });

        registerStepCB([&](){
            mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_completed));
            return true;
        });

        enableAutoStep();
        return true;
    }

	UITutorial::UITutorial(ObjPtr obj) :
	Base(std::move(obj))
	{
        uiList = {Tu_Intro,Tu_TouchBar,Tu_App,Tu_Water,Tu_WaterAssist,Tu_PlantDetect,Tu_Final};
	}

	bool UITutorial::_initialize()
	{
        return true;
	}

	void UITutorial::routine()
	{
        if (!mCurUI || mCurUI->finished())
        {
            mCurUI = build(uiList[mCurIndex]);
            mCurUI->initialize();
            mCurUI->start();
            mCurIndex ++;

            if (mCurIndex >= uiList.size())
                tutorial_over();
        }
	}

    bool UITutorial::_handleInput(InputEvtType &&input)
    {
        auto target = std::dynamic_pointer_cast<InputReceiver>(mCurUI);
        if (!target)
            return false;
        target->handleInput(input);

        return false;
    }

    std::shared_ptr<TuBase> UITutorial::build(TuIndex index)
    {
        switch (index)
        {
            case Tu_Intro:
                log_d("Tu_Intro");
                return std::make_shared<TuIntro>(m_scr);
            case Tu_TouchBar:
                log_d("Tu_TouchBar");
                return std::make_shared<TuTouchBar>(m_scr);
            case Tu_App:
                log_d("Tu_App");
                return std::make_shared<TuApp>(m_scr);
            case Tu_ProvTip:
                log_d("Tu_ProvTip");
                return std::make_shared<TuTouchBar>(m_scr);
            case Tu_Water:
                log_d("Tu_Water");
                return std::make_shared<TuWater>(m_scr);
            case Tu_WaterAssist:
                log_d("Tu_WaterAssist");
                return std::make_shared<TuWaterAssist>(m_scr);
            case Tu_PlantDetect:
                log_d("Tu_PlantDetect");
                return std::make_shared<TuPlantDetect>(m_scr);
            case Tu_Final:
                log_d("Tu_Final");
                return std::make_shared<TuFinal>(m_scr);
            default:
                return nullptr;
        }
    }

    void UITutorial::tutorial_over()
    {
        log_d("tutorial over");
        Sys::shutdown();
    }
}

void test()
{
    // 创建圆点基础对象
    lv_obj_t *dot = lv_obj_create(lv_scr_act());
    lv_obj_set_size(dot, 30, 30); // 设置宽高相等
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动

// 设置圆点样式
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_RADIUS_CIRCLE); // 设为圆形
    lv_style_set_bg_color(&style, lv_color_hex(0x2196F3)); // 背景色
    lv_style_set_bg_opa(&style, LV_OPA_COVER); // 不透明度
    lv_style_set_border_width(&style, 0); // 无边框
    lv_obj_add_style(dot, &style, 0);

// 添加数字标签
    lv_obj_t *label = lv_label_create(dot);
    lv_label_set_text(label, "5"); // 设置数字
    lv_obj_center(label); // 居中显示

// 调整位置（可选）
    lv_obj_align(dot, LV_ALIGN_CENTER, 0, 0);

}