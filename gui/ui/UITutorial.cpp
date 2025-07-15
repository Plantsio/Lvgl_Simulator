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
    //region TuBase
    void TuBase::start()
    {
        mStepHandler.start();
    }

    bool TuBase::finished()
    {
        return mStepHandler.finished();
    }
    //endregion

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
        registerStep(THEME_TEXT_CONTENT(Lang::ui_tu_intro_hi),"");

        registerStep(THEME_TEXT_CONTENT(Lang::ui_tu_intro_ivy),"");

        registerStep(THEME_TEXT_CONTENT(Lang::ui_tu_intro_meet),"");

        registerStep(THEME_TEXT_CONTENT(Lang::ui_tu_intro_before_enter),"");

        auto mainDsc = THEME_TEXT_CONTENT(Lang::ui_tu_intro_ready);
        registerStep(mainDsc,"");
        registerStep(mainDsc,THEME_TEXT_IN_COLOR(Lang::ui_tu_intro_shutdown,Theme::palette_notice));

        registerStep(THEME_TEXT_CONTENT(Lang::ui_tu_skip),"");

        registerStep("","");

        return true;
    }

    void TuIntro::registerStep(const std::string &main,const std::string &sub)
    {
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mMainText.update(main);
                                          mSubText.update(sub);
                                      }, nullptr);
    }
	//endregion

	TuTouchBar::TuTouchBar(lv_obj_t *parent) :
    TuBase(),
    mTopText(parent),
    mBottomText(parent),
    mIndicator(parent)
    //mImage(parent)
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
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_interact));
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_two));
                                          //mImage.update("touch_bar");
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          for (auto &circle: m_circles)
                                          {
                                              Animation::anim_create(circle, reinterpret_cast<lv_anim_exec_xcb_t>(lv_arc_set_bg_end_angle), 0, 360,
                                                                     2000);
                                          }
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_touch_bar_yellow_dot,Theme::palette_notice));
                                          for (auto &circle: m_circles)
                                          {
                                              Animation::anim_fade_out(circle, 2000,0);
                                          }
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mIndicator.enable_visible_flashing(Indicator::ALL,1000,0,1000,500,2);

                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_left),true,6000);
                                          mIndicator.enable_visible(Indicator::LEFT,true,1000,3000);
                                      }, nullptr);


        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcLeft);},
                                      [&]()
                                      {
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_right));
                                          mIndicator.disable_visible(Indicator::LEFT, true, 600);
                                          mIndicator.enable_visible(Indicator::RIGHT, true, 600);
                                      }, nullptr);

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_great));
                                          mIndicator.disable_visible(Indicator::RIGHT, true, 600);
                                      }, nullptr);

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
    mIndicator(parent)
    //mImage(parent)
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
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_add_app));
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          int vendor = 0;                               //todo: vendor 是否还有需求
                                          mMidText.update("");

                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_prov_in_store));
                                          std::string download_app = THEME_TEXT_OPTION(Lang::ui_tu_download_app, vendor);
                                          mSubText.update(Theme::getPaletteText(download_app,Theme::palette_notice));

                                          //mImage.update("app_60");

                                          mIndicator.enable_visible(Indicator::RIGHT, true,1000,1200);
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next),true,6000);
                                      }, nullptr);

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mBottomText.update("");
                                          mIndicator.disable_visible(Indicator::RIGHT, true,500);

                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_in_system_setting));
                                          mSubText.update(THEME_TEXT_IN_COLOR(Lang::ui_enable_ble,Theme::palette_notice));
                                      }, nullptr);

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mIndicator.enable_visible(Indicator::RIGHT, true,1000,1200);
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_touch_right_config));
                                      }, nullptr);

        return true;
    }

    void TuApp::handleInput(InputEvtType &input)
    {
        mInputEvtType = input;
    }

	TuWater::TuWater(lv_obj_t *parent):
    TuBase(),
    mTopText(parent),
    mIndicator(parent)
    //mImage(parent)
	{
        mTopText.set_font_size(16);
        mTopText.align( LV_ALIGN_TOP_MID, 0, 15);
	}

    bool TuWater::_initialize()
    {
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_prepare_plant));
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tu_water_remove_pot,Theme::palette_notice));
                                          //mImage.update("add_water");

                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next));
                                          mIndicator.enable_visible(Indicator::RIGHT,true,1000,1200);
                                      }, nullptr);

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mTopText.update("");
                                          mIndicator.disable_visible(Indicator::RIGHT, true,500);
                                      }, nullptr);

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
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_FORMAT_TEXT<int>(Lang::ui_fluid_add_about, {50}));
                                      }, nullptr);

        mStepHandler.registerStepUnit([](uint32_t &timeout)
                                      {
                                          timeout = 4000;
                                          return SystemStore::get<int>(DeviceDp::waterlevel) > 0;
                                      },
                                      [&]()
                                      {
                                          mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_detected));
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
                                          mIndicator.enable_visible(Indicator::RIGHT,true,1000,4200);
                                      },
                                      [&](int &jumpStep)
                                      {
                                          jumpStep = STEP_FAIL_NO_JUMP;
                                          mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_no_detected));
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
                                          mIndicator.enable_visible(Indicator::RIGHT,true,1000,4200);
                                      });

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_assist_good));
                                          mIndicator.disable_visible(Indicator::RIGHT, true,500);
                                      }, nullptr);

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
    mBottomText(parent)
    //mImage(parent)
	{
        mTopText.set_font_size(16);
        mBottomText.set_font_size(16);

        mTopText.align( LV_ALIGN_TOP_MID, 0, 15);
        mBottomText.align(LV_ALIGN_BOTTOM_MID, 0, -20);
	}

    bool TuPlantDetect::_initialize()
    {
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mTopText.update(THEME_TEXT_CONTENT(Lang::ui_tu_plant_put_pot));
                                          //mImage.update("put_in");
                                      }, nullptr);

        mStepHandler.registerStepUnit([](uint32_t &timeout)
                                      {
                                          timeout = 4000;
                                          return SystemStore::get<bool>(DeviceDp::plantExist);
                                      },
                                      [&]()
                                      {
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
                                          mIndicator.enable_visible(Indicator::RIGHT, true, 1000, 4200);
                                      },
                                      [&](int &jumpStep)
                                      {
                                          jumpStep = STEP_FAIL_NO_JUMP;
                                          mBottomText.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
                                          mIndicator.enable_visible(Indicator::RIGHT, true, 1000, 4200);
                                      });

        mStepHandler.registerStepUnit([&](uint32_t &timeout){timeout = portMAX_DELAY;return pressed(BtnSrcRight);},
                                      [&]()
                                      {
                                          mIndicator.disable_visible(Indicator::RIGHT, true,500);
                                      }, nullptr);

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
        auto commonTrue = [](uint32_t &timeout){timeout = 0;return true;};
        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_congratulations));
                                      }, nullptr);

        mStepHandler.registerStepUnit(commonTrue,
                                      [&]()
                                      {
                                          mMidText.update(THEME_TEXT_CONTENT(Lang::ui_tu_completed));
                                      }, nullptr);
        return true;
    }

    DotWidget::DotWidget(lv_obj_t *parent,lv_color_t base_color,const std::string &label_value):
    group(lv_obj_create(parent))
    {
        lv_style_init(&dotStyle);
        lv_style_set_radius(&dotStyle, LV_RADIUS_CIRCLE); // 设为圆形
        lv_style_set_bg_color(&dotStyle, base_color); // 背景色
        lv_style_set_bg_opa(&dotStyle, LV_OPA_COVER); // 不透明度
        lv_style_set_border_width(&dotStyle, 0); // 无边框

        lv_style_init(&groupStyle);
        lv_style_set_border_width(&groupStyle,0);
        lv_style_set_radius(&groupStyle,0);

        lv_obj_set_size(group,40,18);
        lv_obj_add_style(group,&groupStyle,0);
        lv_obj_clear_flag(group, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_layout(group, LV_FLEX_FLOW_ROW);

        dotMain = lv_obj_create(group);
        lv_obj_set_size(dotMain, 13, 13); // 设置宽高相等
        lv_obj_clear_flag(dotMain, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动

        dotContent = lv_label_create(dotMain);
        lv_label_set_text(dotContent, label_value.c_str()); // 设置数字
        lv_obj_set_style_text_color(dotContent,lv_color_black(),0);
        lv_obj_set_style_text_font(dotContent,&lv_font_montserrat_8,0);
        lv_obj_set_style_text_align(dotContent,LV_TEXT_ALIGN_CENTER,0);
        lv_obj_align(dotContent,LV_ALIGN_CENTER,-1,-1); // 居中显示

        for (auto &dot : dotSub)
        {
            dot = lv_obj_create(group);
            lv_obj_set_size(dot, 3, 3); // 设置宽高相等
            lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动
        }

        lv_obj_add_style(dotMain, &dotStyle, 0);
        for (auto &dot : dotSub)
        {
            lv_obj_add_style(dot, &dotStyle, 0);
        }

        lv_obj_align(dotMain, LV_ALIGN_LEFT_MID, -12, 0);
        lv_obj_align_to(dotSub[0],dotMain,LV_ALIGN_OUT_RIGHT_MID,5,0);
        lv_obj_align_to(dotSub[1],dotSub[0],LV_ALIGN_OUT_RIGHT_MID,5,0);
        lv_obj_align_to(dotSub[2],dotSub[1],LV_ALIGN_OUT_RIGHT_MID,5,0);
    }

    DotWidget::~DotWidget()
    {
        lv_obj_del(dotMain);
        for (auto &dot : dotSub)
        {
            lv_obj_del(dot);
        }
    }

    void DotWidget::updateContent(const std::string &value)
    {
        lv_label_set_text(dotContent, value.c_str()); // 设置数字
    }

    void DotWidget::updateColor(lv_color_t value)
    {
        lv_obj_set_style_bg_color(dotMain,value,0);
        for (auto &dot : dotSub)
        {
            lv_obj_set_style_bg_color(dot,value,0);
        }
    }

    void DotWidget::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        lv_obj_align(group,align,x_ofs,y_ofs);
    }

    void DotWidget::align_to(const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        lv_obj_align_to(group,base,align,x_ofs,y_ofs);
    }

    void DotWidget::align_to(const DotWidget &base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
    {
        lv_obj_align_to(group,base.group,align,x_ofs,y_ofs);
    }

	UITutorial::UITutorial(ObjPtr obj) :
	Base(std::move(obj)),
    mProcessUnit(lv_obj_create(m_scr)),
    mUIUnit(lv_obj_create(m_scr))
	{
        lv_style_init(&mStyle);
        lv_style_set_border_width(&mStyle,0);
        lv_style_set_radius(&mStyle,0);

        lv_obj_set_size(mProcessUnit,320,40);
        lv_obj_add_style(mProcessUnit,&mStyle,0);
        lv_obj_clear_flag(mProcessUnit, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_set_size(mUIUnit,320,200);
        lv_obj_align_to(mUIUnit,mProcessUnit,LV_ALIGN_OUT_BOTTOM_LEFT,0,0);
        lv_obj_add_style(mUIUnit,&mStyle,0);
        lv_obj_clear_flag(mUIUnit, LV_OBJ_FLAG_SCROLLABLE);

        uiList = {Tu_Intro,Tu_TouchBar,Tu_App,Tu_Water,Tu_WaterAssist,Tu_PlantDetect,Tu_Final};
        //uiList = {Tu_WaterAssist,Tu_PlantDetect,Tu_Final};

        mIndev.attachIndev(mUIUnit,[&](InputEvtType &input){return _handleInput(std::forward<InputEvtType>(input));});

	}

	bool UITutorial::_initialize()
	{
        for (int i = 0;i < uiList.size();i ++)
        {
            mDots.push_back(std::make_unique<DotWidget>(mProcessUnit, lv_color_hex(0x969696),std::to_string(i + 1)));
        }

        mDots[0]->align(LV_ALIGN_LEFT_MID,-7,0);

        for (int i = 1; i < mDots.size(); ++i)
        {
            mDots[i]->align_to(*mDots[i - 1],LV_ALIGN_OUT_RIGHT_MID,3,0);
        }

        return true;
	}

	void UITutorial::routine()
	{
        if (!mCurUI || mCurUI->finished())
        {
            if (mCurIndex < uiList.size())
            {
                mCurUI = build(uiList[mCurIndex]);
                mCurUI->initialize();
                mCurUI->start();
                mDots[mCurIndex]->updateColor(lv_palette_main(LV_PALETTE_YELLOW));
                mCurIndex ++;
            }
            else
            {
                tutorial_over();
            }
        }
	}

    bool UITutorial::_handleInput(InputEvtType &&input)
    {
        log_d("input src == %d",input.index);
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
                return std::make_shared<TuIntro>(mUIUnit);
            case Tu_TouchBar:
                log_d("Tu_TouchBar");
                return std::make_shared<TuTouchBar>(mUIUnit);
            case Tu_App:
                log_d("Tu_App");
                return std::make_shared<TuApp>(mUIUnit);
            case Tu_ProvTip:
                log_d("Tu_ProvTip");
                return std::make_shared<TuTouchBar>(mUIUnit);
            case Tu_Water:
                log_d("Tu_Water");
                return std::make_shared<TuWater>(mUIUnit);
            case Tu_WaterAssist:
                log_d("Tu_WaterAssist");
                return std::make_shared<TuWaterAssist>(mUIUnit);
            case Tu_PlantDetect:
                log_d("Tu_PlantDetect");
                return std::make_shared<TuPlantDetect>(mUIUnit);
            case Tu_Final:
                log_d("Tu_Final");
                return std::make_shared<TuFinal>(mUIUnit);
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
    lv_obj_set_size(dot, 10, 10); // 设置宽高相等
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