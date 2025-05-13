//
// Created by Huwensong on 2024/11/15.
//

#include "UIWaterCalib.h"
#include "ThemeInterface.h"
#include "Lang.h"

namespace UI
{
	UIWaterCalib::UIWaterCalib(ObjPtr obj) :
            Base(std::move(obj)),
            TimerStepBase(m_scr, nullptr, nullptr),
            m_step_label(m_scr, 300, 400, 300),
            m_value_label(m_scr, 300, 400, 300),
            m_desc_label(m_scr, 300, 400, 300),
            m_progress_bar(lv_bar_create(m_scr)),
            m_indicator(m_scr)
	{
		m_step_label.set_font_size(16);
		m_value_label.set_font_size(16);
		m_desc_label.set_font_size(16);

		bar_set_default_style(m_progress_bar);
		lv_obj_set_size(m_progress_bar, 200, 20);

		lv_obj_align(m_step_label.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 30);
		lv_obj_align(m_value_label.get_origin_obj(), LV_ALIGN_CENTER, 0, -20);
		lv_obj_align(m_desc_label.get_origin_obj(), LV_ALIGN_CENTER, 0, 60);
		lv_obj_align(m_progress_bar, LV_ALIGN_CENTER, 0, 20);

		//Prop::set(Prop::sense_debug_mode, true, true, false);
	}

	UIWaterCalib::~UIWaterCalib() {
		sense.set_calib_zero(false);
		sense.set_level_calibration(false, false);
	}

	void UIWaterCalib::next() {
		switch (m_current_step) {
			case 0: {
				sense.reset_water_level_calib(false);
				m_step_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_calibrating));
				m_value_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_prepare_sensor));
				m_desc_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_wait_moment));
				lv_timer_set_period(m_timer, 500);
				break;
			}
			case 1: {
				start_water_routine();
				lv_timer_set_period(m_timer, 10);
				break;
			}
			case 2: {
				/* we need to start with no water present in the tank */
				if (!water_routine_stopped()) {
					return;
				}
				if (sense.get_current_water_level() > 0) {
					m_value_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_detected));
					m_desc_label.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tu_water_tank_empty,Theme::palette_notice));
				} else {
					confirm_t++;
				}
				if (confirm_t < 2) {
					start_water_routine();
					return;
				}
				sense.reset_water_level_calib_buf();
				sense.set_calib_zero(true);
				start_water_routine();
				break;
			}
			case 3: {
				if (calib_routine(0)) {
					break;
				}
				return;
			}
			case 4: {
				if (!m_right_touched) {
					return;
				}
				wait_routine();
				break;
			}
			case 5: {
				if (calib_routine(20)) {
					break;
				}
				return;
			}
			case 6: {
				if (!m_right_touched) {
					return;
				}
				wait_routine();
				break;
			}
			case 7: {
				if (calib_routine(40)) {
					break;
				}
				return;
			}
			case 8: {
				if (!m_right_touched) {
					return;
				}
				wait_routine();
				break;
			}
			case 9: {
				if (calib_routine(60)) {
					break;
				}
				return;
			}
			case 10: {
				if (!m_right_touched) {
					return;
				}
				wait_routine();
				break;
			}
			case 11: {
				if (calib_routine(80)) {
					break;
				}
				return;
			}
			case 12: {
				sense.calibrate_water_level();
				lv_timer_set_period(m_timer, 3000);
				update_progress(100);
				m_indicator.disable_visible(Indicator::RIGHT, true, 600, 0);
				m_value_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_calib_completed));
				m_desc_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_pour_out));
				break;
			}
			case 13: {
				sense.set_level_calibration(false, false);
//				LvglDriver::instance().set_locked(false);
//				LvglDriver::instance().route_to_default();
				break;
			}
		}
		m_current_step++;

	}

	void UIWaterCalib::ask_add_water(int total) {
		if (total == 20) {
			/* first time */
			m_value_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_add_20_water));
		} else {
			m_value_label.update(THEME_FORMAT_TEXT<int>(Lang::ui_tu_water_another_20_water, {total}));
		}
		m_desc_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_continue));
		m_indicator.enable_visible(Indicator::LEFT, true, 600, 0);
		m_right_touched = false;
		sense.reset_water_level_calib_buf();
	}

	void UIWaterCalib::wait_routine() {
		m_value_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_analyzing));
		m_desc_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_wait_moment));
		start_water_routine();
		m_indicator.disable_visible(Indicator::RIGHT, false, 200, 0);
	}

	void UIWaterCalib::start_water_routine() {
		//Prop::set(Prop::sense_debug_mode, SENSE_DEBUG_MODE_WATER_ROUTINE_START, true, false);

	}

	bool UIWaterCalib::water_routine_stopped() {
		//return Prop::get<int>(Prop::sense_debug_mode) == SENSE_DEBUG_MODE_WATER_ROUTINE_END;
		return true;
	}


	bool UIWaterCalib::calib_routine(int total_level) {
		if (!water_routine_stopped()) {
			return false;
		}
		if (total_level == 0) {
			if (sense.get_calib_zero()) {
				/* calib not over */
				start_water_routine();
				return false;
			}
			ask_add_water(20);
			return true;
		}

		if (sense.get_current_water_level() > 0) {
			if (sense.set_water_level_calib(total_level)) {
//                m_value_label.update(temp.c_str());
				update_progress(total_level);
				ask_add_water(total_level + 20);
				return true;
			} else {
				m_desc_label.update(THEME_TEXT_CONTENT(Lang::ui_tu_water_wait_moment));
			}
		} else {
//            m_value_label.update(temp.c_str());
			m_desc_label.update(THEME_TEXT_APPEND_COLOR(Lang::ui_tu_water_make_sure,Theme::palette_notice));
		}
		start_water_routine();
		return false;
	}

	void UIWaterCalib::reset_progress() {
		auto *bar = (lv_bar_t *) m_progress_bar;
		bar->cur_value = 0;
	}

	void UIWaterCalib::update_progress(int progress, int anim_t) {
		if (anim_t) {
			lv_obj_set_style_anim_time(m_progress_bar, anim_t, 0);
			lv_bar_set_value(m_progress_bar, progress, LV_ANIM_ON);
			auto *bar = (lv_bar_t *) m_progress_bar;
			bar->cur_value_anim.anim_start = 0;
		} else {
			lv_bar_set_value(m_progress_bar, progress, LV_ANIM_OFF);
		}
	}

	bool UIWaterCalib::_deInitialize() {
		sense.set_calib_zero(false);
		sense.set_level_calibration(false, false);
        return true;
	}

	void UIWaterCalib::right_input_cb(InputData data)
	{
		m_right_touched = true;
	}
}