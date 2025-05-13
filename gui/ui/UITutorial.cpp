//
// Created by Gordon on 2022/5/6.
//

#include "UITutorial.h"
#include "tools.h"
#ifdef Ivy

#include "memory"
#include "IvyNet.h"
#include "IOTManager.h"
#include "ThemeInterface.h"
#include "UIProvTip.h"
#include "IOTManager.h"

#include "Plant.h"

#endif

namespace UI
{
	TuBase::TuBase(lv_obj_t *parent,Indicator &indicator):
            TimerStepBase(parent, nullptr, nullptr),
            m_image(parent),
            m_indicator(indicator)
	{
		m_top_text.set_font_size(16);
		m_bottom_text.set_font_size(16);

		lv_obj_align(m_image.get_image_obj(), LV_ALIGN_CENTER, 0, 0);
	}

	TuBase::~TuBase()
	{
		m_indicator.disable_visible(Indicator::ALL);
	}

	void TuBase::handle_left_touch()
	{
		m_left_touched = true;
	}

	void TuBase::handle_right_touch()
	{
		m_right_touched = true;
	}

	//region intro
	TuIntro::TuIntro(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator)
	{
		m_top_text.set_font_size(30);
		m_bottom_text.set_font_size(16);
		lv_obj_align(m_top_text.get_origin_obj(), LV_ALIGN_CENTER, 0, -20);
		lv_obj_align_to(m_bottom_text.get_origin_obj(), m_top_text.get_origin_obj(), LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
	}

	void TuIntro::update(const std::string &main, const std::string &sub)
	{
		update_main(main);
		update_sub(sub);
	}

	void TuIntro::update_main(const std::string &main)
	{
		m_top_text.update(main);
	}

	void TuIntro::update_sub(const std::string &sub)
	{
		m_bottom_text.update(sub);
	}

	void TuIntro::realign() const
	{

	}

	void TuIntro::next()
	{
		switch (m_current_step ++)
		{
			case 0:
				update(THEME_TEXT_CONTENT(Lang::ui_tu_intro_hi),"");
				break;
			case 1:
				update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_ivy));
				break;
			case 2:
				update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_meet));
				break;
			case 3:
				update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_before_enter));
				break;
			case 4:
				update_main(THEME_TEXT_CONTENT(Lang::ui_tu_intro_ready));
				break;
			case 5:
				update_sub(THEME_TEXT_IN_COLOR(Lang::ui_tu_intro_shutdown,Theme::palette_notice));
				break;
			case 6:
				update(THEME_TEXT_CONTENT(Lang::ui_tu_skip),"");
				break;
			case 7:
				update("", "");
				lv_timer_set_period(m_timer, 2000);
				break;
			case 8:
				end();
				break;
		}
	}
	//endregion

	TuTouchBar::TuTouchBar(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator)
	{
		for (auto &circle: m_circles)
		{
			circle = lv_arc_create(m_parent);
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

	void TuTouchBar::next()
	{
		switch (m_current_step)
		{
			case 0:
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_interact));
				lv_timer_set_period(m_timer, 3000);
				break;
			case 1:
			{
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_two));
				m_image.update("tu/touch_bar");
				break;
			}
			case 2:
			{
				for (auto &circle: m_circles)
				{
					Animation::anim_create(circle, reinterpret_cast<lv_anim_exec_xcb_t>(lv_arc_set_bg_end_angle), 0, 360,
										 2000);
				}
				lv_timer_set_period(m_timer, COMMON_TIMER_DELAY);
				break;
			}
			case 3:
			{
				m_top_text.update(THEME_TEXT_IN_COLOR(Lang::ui_tu_touch_bar_yellow_dot,Theme::palette_notice));
				for (auto &circle: m_circles)
				{
					Animation::anim_fade_out(circle, 2000,0);
				}
				lv_timer_set_period(m_timer, 4000);
				break;
			}
			case 4:
			{
				m_indicator.enable_visible_flashing(Indicator::ALL,1000,0,1000,800,2);
				lv_timer_set_period(m_timer, 5000);
				break;
			}
			case 5:
			{
				m_left_touched = false;
				lv_timer_set_period(m_timer, 10);
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_left));
				m_indicator.enable_visible(Indicator::LEFT,true,1000,3000);
				break;
			}
			case 6:
			{
				if (!m_left_touched)
				{
					return;
				}
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_right), false);
				m_indicator.disable_visible(Indicator::LEFT, true, 600);
				m_indicator.enable_visible(Indicator::RIGHT, true, 600);
				m_right_touched = false;
				break;
			}
			case 7:
			{
				if (!m_right_touched)
				{
					return;
				}
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_touch_bar_great), false);
				m_indicator.disable_visible(Indicator::RIGHT, true, 600);
				lv_timer_set_period(m_timer, 2000);
				break;
			}
			case 8:
				end();
				break;
		}
		m_current_step ++;
	}

	TuApp::TuApp(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator),
	m_sub_text(m_parent, 300),
	m_mid_text(m_parent, 300, 800, 600)
	{
		m_sub_text.set_font_size(16);

		m_mid_text.set_font_size(30);
		lv_obj_align(m_mid_text.get_origin_obj(), LV_ALIGN_CENTER, 0, -20);
	}

	void TuApp::next()
	{
		switch (m_current_step)
		{
			case 0:
				m_mid_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_add_app));
				lv_timer_set_period(m_timer, 3000);
				break;
			case 2:
			{
				int vendor = Prop::get<int>(Prop::vendor);
				m_mid_text.update("");
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_prov_in_store));

				std::string download_app = THEME_TEXT_OPTION(Lang::ui_tu_download_app, vendor);
				m_sub_text.update(Theme::getPaletteText(download_app,Theme::palette_notice));
				lv_obj_align_to(m_sub_text.get_origin_obj(), m_top_text.get_origin_obj(), LV_ALIGN_OUT_BOTTOM_MID, 0,
								10);
				std::string asset_name = "tu/app_60";
				m_image.update(asset_name);
				break;
			}
			case 3:
			{
				m_indicator.enable_visible(Indicator::RIGHT, true,1000,1200);
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next));
				lv_timer_set_period(m_timer, 10);
				m_right_touched = false;
				break;
			}
			case 4:
			{
				if (!m_right_touched)
				{
					return;
				}
				m_indicator.disable_visible(Indicator::RIGHT, true,500);
				m_bottom_text.update("");
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_in_system_setting));
				m_sub_text.update(THEME_TEXT_IN_COLOR(Lang::ui_enable_ble,Theme::palette_notice));
				std::string str = THEME_TEXT_IN_COLOR(Lang::ui_enable_ble,Theme::palette_notice);
				m_image.update("tu/bluetooth");
				lv_timer_set_period(m_timer, 3000);
				break;
			}
			case 5:
			{
				m_indicator.enable_visible(Indicator::RIGHT, true,1000,1200);
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_touch_right_config));
				lv_timer_set_period(m_timer, 10);
				m_right_touched = false;
				break;
			}
			case 6:
			{
				if (!m_right_touched)
				{
					return;
				}
				end();
				break;
			}
		}
		m_current_step++;
	}

	TuWater::TuWater(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator)
	{
		lv_obj_align(m_top_text.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 15);
	}

	void TuWater::next()
	{
		switch (m_current_step)
		{
			case 0:
			{
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_prepare_plant));
				lv_timer_set_period(m_timer, 4500);
				break;
			}
			case 1:
			{
				m_top_text.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tu_water_remove_pot,
												  Theme::palette_notice));
                //m_top_text.update("Please add 20mL of Water")
				m_image.update("tu/add_water");
				break;
			}
			case 2:
			{
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next));
				m_indicator.enable_visible(Indicator::RIGHT,true,1000,1500);
				lv_timer_set_period(m_timer, 1);
				m_right_touched = false;
				break;
			}
			case 3:
			{
				if (!m_right_touched)
				{
					return;
				}
				//Sense::instance().set_routine_delay(400, 100000);
				//Prop::set(Prop::sense_debug_mode, false, true, false, false);
				//Prop::set(Prop::plant_auto_detect, false, true, false, false);
				break;
			}
			case 4:
				end();
				break;
		}
		m_current_step++;
	}

	TuWaterAssist::TuWaterAssist(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator),
	m_middle_text(m_parent)
	{
		m_middle_text.set_font_size(30);

		lv_obj_align(m_middle_text.get_origin_obj(), LV_ALIGN_CENTER, 0, -30);
	}

	void TuWaterAssist::next()
	{
		switch (m_current_step)
		{
			case 0:
			{
				std::string temp = THEME_FORMAT_TEXT<int>(Lang::ui_fluid_add_about, {Plant::get_target_water_volume()});
				m_top_text.update(temp);
				break;
			}
			case 1:
			{
				m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_right_next), true, 3000);
				m_indicator.enable_visible(Indicator::RIGHT,true,1000,4500);
				m_right_touched = false;
				m_middle_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_no_detected));
				lv_timer_set_period(m_timer, 100);
				break;
			}
			case 2:
			{
				if (!m_right_touched)
				{
					return;
				}
			}
			case 3:
			{
				if (has_water)
				{
					m_bottom_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_assist_good));
					lv_timer_set_period(m_timer, 2000);
				}
				break;
			}
			case 4:
			{
				end();
				break;
			}
		}
		m_current_step++;
	}

	TuPlantDetect::TuPlantDetect(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator)
	{
		lv_obj_align(m_top_text.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 15);
		//Sense::instance().set_routine_delay(100000, 1000000);
		//Prop::set(Prop::plant_auto_detect, true, true, false, false);
	}

	void TuPlantDetect::next()
	{
		switch (m_current_step)
		{
			case 0:
			{
				m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_plant_put_pot));
				m_image.update("tu/put_in");
				lv_obj_align(m_image.get_image_obj(), LV_ALIGN_CENTER, -5, 10);
				m_indicator.enable_visible(Indicator::RIGHT, true, 1000, 1500);
				lv_timer_set_period(m_timer, 100);
				break;
			}
			case 1:
			{
				if (!m_right_touched)
				{
					return;
				}
				end();
				break;
			}
		}
		m_current_step++;
	}

	TuFinal::TuFinal(lv_obj_t *parent,Indicator &indicator) :
	TuBase(parent,indicator),
	m_text(m_parent)
	{
		m_text.set_font_size(30);
		lv_obj_align(m_text.get_origin_obj(), LV_ALIGN_CENTER, 0, 0);
	}

	void TuFinal::next()
	{
		switch (m_current_step)
		{
			case 0:
			{
				lv_timer_set_period(m_timer, 4000);
				m_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_congratulations));
				break;
			}
			case 1:
			{
				m_text.update(THEME_TEXT_CONTENT(Lang::ui_tu_completed));
				break;
			}
			case 2:
			{
				m_text.update("");
				//Prop::set(Prop::tutorial_complete, true, true, false, true);
				end();
				break;
			}
		}
		m_current_step++;
	}



	UITutorial::UITutorial(ObjPtr obj) :
	Base(std::move(obj)),
    mIndicator(m_scr)
	{
        uiStack.push({std::make_shared<TuFinal>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<TuPlantDetect>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<TuWaterAssist>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<TuWater>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<ProvTip>(m_scr,m_obj->getData()), [](){

           IOTManager::instance().initialize(false);}});
        uiStack.push({std::make_shared<TuApp>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<TuTouchBar>(m_scr,mIndicator), nullptr});
        uiStack.push({std::make_shared<TuIntro>(m_scr,mIndicator), nullptr});


		enable_input(INPUT_SRC_MASK(BtnSrcLeft) | INPUT_SRC_MASK(BtnSrcRight));
	}

	bool UITutorial::_initialize()
	{
        mCurrentComponent = uiStack.top();
        uiStack.pop();
        mCurrentComponent.ui->start();
        if (mCurrentComponent.action)
            mCurrentComponent.action();


        return true;
	}

	void UITutorial::routine()
	{
		if(mCurrentComponent.ui->is_finished())
		{
            log_d("finish ...");
			if (!uiStack.empty())
			{
                mCurrentComponent = uiStack.top();
                uiStack.pop();
                mCurrentComponent.ui->start();
                if (mCurrentComponent.action)
                    mCurrentComponent.action();
                log_d("change ...");
			}
			else
			{
                log_d("tutorial_over");
				tutorial_over = true;
			}
		}

		if (tutorial_over)
		{
			log_d("tutorial over");
			//Boot::soft_reboot();
		}
	}

    bool UITutorial::_handleInput(InputEvtType &&input)
    {
        auto target = std::dynamic_pointer_cast<TuBase>(mCurrentComponent.ui);
        if (!target)
            return false;
        switch (input.index)
        {
            case BtnSrcLeft:
                target->handle_left_touch();
                break;
            case BtnSrcRight:
                target->handle_right_touch();
                break;
            default:
                break;
        }
        return false;
    }
}