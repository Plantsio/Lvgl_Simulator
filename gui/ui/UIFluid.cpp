//
// Created by Gordon on 2022/5/17.
//

#include "UIFluid.h"
#include "ThemeInterface.h"
#include "Lang.h"
#include "interface.h"

namespace UI {
#define PI 3.1415926535897932384626433832795
    void WaveUnit::update_data() {
        data.resize(wl);
        double k = 2 * PI / (double) wl;
        for (int x = 0; x < wl; x++) {
            data[x] = (int) round((double) amp * sin(k * (double) x)) + amp;
        }
    }

    void WaveUnit::set_amp(int t_amp) {
        amp = t_amp;
        update_data();
    }

    void WaveUnit::set_wl(int t_wl, bool update) {
        wl = t_wl;
        if (update) {
            update_data();
        }
    }

    void Wave::update_data() {
        /* copy the last _t of a unit */
        data.clear();
        data.reserve(length);
        std::copy(unit.data.begin() + unit.wl - x_offset, unit.data.end(), std::back_inserter(data));
        int n = x_offset;
        while (length - n > unit.wl) {
            std::copy(unit.data.begin(), unit.data.end(), std::back_inserter(data));
            n += unit.wl;
        }
        std::copy(unit.data.begin(), unit.data.begin() + length - n, std::back_inserter(data));
    }

    void Wave::move_hor(int dis) {
        dis = dis % unit.wl;
        x_offset += dis;
        x_offset = x_offset % unit.wl;
        if (x_offset < 0) {
            x_offset = unit.wl + x_offset;
        }
    }

    void CMask::update_data() {
        m_data.resize(m_radius + 1);
        for (int i = 0; i <= m_radius; i++) {
            double k = sqrt(2 * i * m_radius - pow(i, 2));
            m_data[i] = (int) std::round(k);
        }
    }

    CMask::bound_t CMask::get_bound(int i) {
        if (i > m_radius) {
            i = 2 * m_radius - i;
        }
        return {m_radius - m_data[i], m_radius + m_data[i]};
    }

#ifndef Ivy

    void UIFluid::mouse_cb(lv_event_t *event) {
        lv_indev_t *indev = lv_indev_get_act();
        lv_indev_type_t indev_type = lv_indev_get_type(indev);
        if (indev_type == LV_INDEV_TYPE_POINTER) {
            lv_point_t point;
            lv_indev_get_point(indev, &point);
            auto *ui = (UIFluid *) event->user_data;
            int start = (LV_VER_RES - ui->get_tank_size()) / 2;
            int end = start + ui->get_tank_size();
            int y = std::min((int) point.y, end) - start;
            y = std::max(1, y);
            ui->set_target_y(y);
        }
    }

#endif

	UIFluid::UIFluid(ObjPtr obj, int tank_size)
			: Base(std::move(obj)),
			  m_tank_size(tank_size),
			  m_mask(m_tank_size / 2),
			  m_canvas(lv_canvas_create(m_scr)),
			  m_bottom_label(lv_label_create(m_scr)),
			  m_top_text(m_scr, 180, 1000, 800),
			  m_wave_front(10, 150, 10, m_tank_size, 0),
			  m_wave_back(6, 120, 10, m_tank_size, 3),
			  m_target_y(m_tank_size / 2),
			  m_current_y(m_tank_size)
	{
		m_top_text.set_font_size(16);
		m_canvas_buf.resize(m_tank_size * m_tank_size * sizeof(uint16_t));
		lv_canvas_set_buffer(m_canvas, m_canvas_buf.data(), m_tank_size, m_tank_size, LV_IMG_CF_TRUE_COLOR);
		lv_obj_align(m_canvas, LV_ALIGN_CENTER, 0, 0);
		lv_canvas_fill_bg(m_canvas, WATER_TANK_ENV_COLOR, LV_OPA_COVER);
        label_set_style(m_bottom_label, &ba_30);
		lv_obj_align(m_bottom_label, LV_ALIGN_CENTER, 0, 50);
		//lv_obj_align(m_top_text.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 20);
		lv_label_set_text(m_bottom_label, "");
#ifndef Ivy
		lv_obj_add_event_cb(m_scr, mouse_cb, LV_EVENT_CLICKED, this);
#endif
	}

    void UIFluid::set_amp(int amp) {
        int front_amp;
        int back_amp;
        int max_amp = std::min(m_current_y, m_tank_size - m_current_y);
        if (amp > max_amp) amp = max_amp;
        if (amp < WATER_TANK_MIN_AMP) amp = WATER_TANK_MIN_AMP;
        if (amp > WATER_TANK_MAX_AMP) amp = WATER_TANK_MAX_AMP;
        m_current_amp = amp;
        front_amp = amp;
        if (front_amp > 4) {
            back_amp = front_amp - 2;
        } else {
            back_amp = front_amp;
        }
        m_wave_front.set_amp(front_amp);
        m_wave_back.set_amp(back_amp);
    }

    bool UIFluid::_initialize() {
        update_status(true);
        return true;
    }

    void UIFluid::update_status(bool force) {
//        Plant &plant = Plant::instance();
//        bool t_analyzing = plant.analyzing();
//        int t_status = plant.get_status().water_status;
//        int plant_type = Prop::get<int>(Prop::plant_type);
//        bool t_has_pot = Prop::get<bool>(Prop::has_pot);
//        if (m_plant_type != plant_type) {
//            m_plant_type = plant_type;
//            if (!plant_type) {
//                m_top_text.update(THEME_TEXT_APPEND_COLOR(Lang::ui_fluid_select_plant,Theme::palette_notice));
//                return;
//            }
//        }
//        if (force || m_analyzing != t_analyzing || m_watering_status != t_status || m_has_pot != t_has_pot) {
//            if (t_analyzing) {
//                if (t_has_pot) {
//                    m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_fluid_analy_ten_mins));
//                } else {
//                    m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_fluid_analy_put_plant));
//                }
//                m_analyzing = t_analyzing;
//            } else {
//                if (plant.get_status().water_status == Plant::just_add_water) {
//                    m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_fluid_healthy));
//                } else if (plant.get_status().water_status == Plant::tank_dry_no_need) {
//                    m_top_text.update(THEME_TEXT_CONTENT(Lang::ui_fluid_absorbing));
//                }
//                m_watering_status = t_status;
//            }
//        }
//        m_has_pot = t_has_pot;
    }

    void UIFluid::routine() {
//        update_status(false);
//        set_target_y_from_percent(Sense::instance().get_current_water_level() / WATER_TANK_VOLUME);
//        if (m_current_y < m_target_y) {
//            /* dropping */
//            m_current_y += (m_target_y - m_current_y) / 5 + 1;
//            m_current_y = min(m_current_y, m_tank_size);
//        } else if (m_current_y > m_target_y) {
//            /* rising */
//            m_current_y -= (m_current_y - m_target_y) / 5 + 1;
//            m_current_y = max(m_current_y, 1);
//        }
//        /* add wave effect during dramatic change */
//        if (m_current_y != m_last_level) {
//            set_amp(m_current_amp + 1);
//            m_stable_cnt = 0;
//            m_last_level = m_current_y;
//            /* stabilize water level when no change anymore */
//        } else if (++m_stable_cnt > WATER_TANK_STABLE_UNIT_CNT) {
//            set_amp(m_current_amp - 1);
//            m_stable_cnt = 0;
//        }
//        /* add wave moving effect */
//        m_wave_front.move_hor(m_current_speed);
//        m_wave_back.move_hor(-1 * m_current_speed / 2);
//
//        m_wave_front.update_data();
//        m_wave_back.update_data();
//
//        /* todo
//         * 1. horizontal chunk set
//         * 2. only render wave part
//         * 3. only render changed parts
//         * */
//        m_wave_y_start = m_current_y - m_current_amp;
//        m_wave_y_end = m_current_y + m_current_amp;
//        render_top();
//        render_wave();
//        render_bottom();
//
//        if (m_current_amp > WATER_TANK_MIN_AMP + 1) {
//            stable_cb(false);
//        } else {
//            stable_cb(true);
//        }
//
//        lv_obj_invalidate(m_canvas);
//
//        render_text();
////        std::string value_string = std::to_string((int) std::round(current_level)) + "mL";
////        update_bottom_label(value_string.c_str(), lv_color_white());
//
//        Base::routine();
    }

    void UIFluid::render_top() {
        int index_start = m_wave_y_start * m_tank_size;
        //lv_color_fill(m_canvas_buf.data(), WATER_TANK_ENV_COLOR, index_start);
    }

    void UIFluid::render_wave() {
        int index_start = m_wave_y_start * m_tank_size;
        for (int x = 0; x < m_tank_size; x++) {
            CMask::bound_t bound = m_mask.get_bound(x);
            if (bound.start > m_wave_y_end || m_wave_y_start > bound.end) {
                continue;
            }
            int start = std::max(bound.start, m_wave_y_start);
            int end = std::min(bound.end, m_wave_y_end);
            int back_y = m_wave_back.data[x];
            int front_y = m_wave_front.data[x];
            for (int y = start - m_wave_y_start; y < end - m_wave_y_start; y++) {
                lv_color_t *pix = &m_canvas_buf[index_start + y * m_tank_size + x];

                /* render wave */
                if (back_y > front_y) {
                    /* back wave is blocked */
                    if (y < front_y) {
                        *pix = WATER_TANK_ENV_COLOR;
                    } else {
                        *pix = m_front_wave_color;
                    }
                } else {
                    /* back wave is visible */
                    if (y < back_y) {
                        *pix = WATER_TANK_ENV_COLOR;
                    } else {
                        if (y < front_y) {
                            *pix = m_back_wave_color;
                        } else {
                            *pix = m_front_wave_color;
                        }
                    }
                }
            }
        }
    }

    void UIFluid::render_bottom() {
        for (int y = m_wave_y_end; y < m_tank_size; y++) {
            CMask::bound_t bound = m_mask.get_bound(y);
            //lv_color_fill(m_canvas_buf.data() + m_tank_size * y + bound.start, m_front_wave_color,
            //              bound.end - bound.start);
            if (y < 0 || y > m_tank_size ||
                m_tank_size * y + bound.start + bound.end - bound.start > m_tank_size * m_tank_size) {
                printf("???\n");
            }
        }
    }

    void UIFluid::render_text() {
        //lv_label_set_text(m_bottom_label, (std::to_string(Prop::get<int>(Prop::water_level)) + "mL").c_str());
    }

    void UIFluid::stable_cb(bool stable) {
    }
}
