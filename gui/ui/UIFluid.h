//
// Created by Gordon on 2022/5/17.
//

#ifndef PLANTSIO_IVY_UIFLUID_H
#define PLANTSIO_IVY_UIFLUID_H

#include "UIBase.h"
#include "cmath"
#include "tools.h"
#include "BasicText.h"

#define WATER_TANK_SIZE                 200
#define WATER_TANK_ENV_COLOR            lv_color_make(0,0,0)
#define WATER_TANK_VOLUME               90.0
#define WATER_TANK_MIN_AMP              1
#define WATER_TANK_MAX_AMP              10
#define WATER_TANK_STABLE_UNIT_CNT      2
#define WATER_TANK_FRONT_WAVE_COLOR     lv_color_make(51,187,211)
#define WATER_TANK_BACK_WAVE_COLOR      lv_color_make(24,157,208)

namespace UI {

    class WaveUnit {
    public:
        /**
         * generate sine wave unit
         * @param t_amp amplitude
         * @param t_wl wave length
         * @param t_y_offset vertical offset
         */
        WaveUnit(int t_amp, int t_wl, int t_y_offset)
                : amp(t_amp),
                  wl(t_wl) {
            data.reserve(t_wl);
            update_data();
        }

        void set_amp(int t_amp);

        void set_wl(int t_wl, bool update = true);

        std::vector<int> data;
        int amp;
        int wl;   /* wave length */

        void update_data();
    };

    class Wave {
    public:
        Wave(int t_amp, int t_wl, int t_y_offset, int t_length, int t_x_offset)
                : unit(t_amp, t_wl, t_y_offset),
                  length(t_length) {
            data.reserve(length);
            move_hor(t_x_offset);
            update_data();
        }

        void move_hor(int dis);

        void set_y_offset(int offset, bool update = true) {
            if (update) {
                update_data();
            }
        }

        void update_unit() {
            unit.update_data();
        }

        void set_amp(int amp) {
            unit.set_amp(amp);
        }

        WaveUnit unit;
        std::vector<int> data;
        int length;
        int x_offset = 0;

        void update_data();
    };

    class CMask {
    public:
        typedef struct {
            int start;
            int end;
        } bound_t;

        explicit CMask(int radius) : m_radius(radius) {
            update_data();
        }

        void update_data();

        bound_t get_bound(int i);

    private:
        int m_radius;
        std::vector<int> m_data;
    };

    class UIFluid : public Base {
    public:
        UIFluid(ObjPtr obj, int tank_size=WATER_TANK_SIZE);

        void routine() override;

        void render_top();

        virtual void render_wave();

        void render_bottom();

        virtual void render_text();

        virtual void stable_cb(bool stable);

        void set_target_y_from_percent(double percent) {
            m_target_y = (int) std::round((double) m_tank_size * (1 - percent));
        }

        void set_target_y(int y) {
            m_target_y = y;
        }

        int get_tank_size() const{
            return m_tank_size;
        }

    private:
        bool _initialize() override;

    protected:
        const int m_tank_size;
        void set_amp(int amp);
        virtual void update_status(bool force);

#ifndef Ivy

        static void mouse_cb(lv_event_t *event);

#endif

        lv_color_t m_front_wave_color = WATER_TANK_FRONT_WAVE_COLOR;
        lv_color_t m_back_wave_color = WATER_TANK_BACK_WAVE_COLOR;
        CMask m_mask;
        std::vector<lv_color_t> m_canvas_buf;
        lv_obj_t *m_canvas;
        lv_obj_t *m_bottom_label;
        BasicText m_top_text;
        Wave m_wave_front;
        Wave m_wave_back;

        int m_target_y ;
        int m_current_y;
        int m_current_amp = 10;
        int m_current_speed = 6;
        int m_wave_y_start = 0;
        int m_wave_y_end = 0;
        uint16_t m_stable_cnt = 0;
        int m_last_level = 0;

    private:

        bool m_analyzing = false;
        int m_watering_status = 0;
        int m_plant_type = -1;
        bool m_has_pot = false;
    };
}

#endif //PLANTSIO_IVY_UIFLUID_H
