//
// Created by Gordon on 2022/4/15.
//

#ifndef LVGL_UIPLANTSTATUS_H
#define LVGL_UIPLANTSTATUS_H

#include "UIBase.h"
#include "vector"
#include "string"
#include "Plant.h"
#include "Indicator.h"

namespace UI {
#define BUBBLE_HEIGHT               45
#define BUBBLE_POINTER_W            20
#define BUBBLE_POINTER_H            10
#define BUBBLE_AREA_H               (BUBBLE_HEIGHT + 2 * BUBBLE_POINTER_H)
#define DEFAULT_PADDING_W           6
#define DEFAULT_PADDING_H           5
#define DEFAULT_AREA_W              (DEFAULT_CARD_W * 3 + DEFAULT_PADDING_W * 2)
#define DEFAULT_CARD_H              ((240-3*DEFAULT_PADDING_H)/2)
#define DEFAULT_CARD_W              80

    class StatusCard {
    public:
        StatusCard(lv_obj_t *parent, const std::string &name, const std::string &unit, lv_color_t name_color);

        lv_obj_t *m_card;

        void update_value_label(const std::string &value_str);

        void update_selected_style(bool selected) const;

        void update_color(lv_color_t color) const;

    private:
        lv_obj_t *m_value_label;
        lv_obj_t *m_name_label;
        lv_obj_t *m_unit_label;
    };

    class BubblePointer {
    public:
        BubblePointer(lv_obj_t *parent, lv_coord_t pointer_w, lv_coord_t pointer_h, lv_coord_t line_w = 2);

        void update_color(lv_color_t color = lv_color_white());

        void set_direction(bool up);

        lv_obj_t *body;

    private:
        lv_coord_t m_pointer_w;
        lv_coord_t m_pointer_h;
        lv_coord_t m_line_w;
        lv_obj_t *line_top;
        lv_obj_t *line_bottom;
        std::vector<lv_point_t> points_top;
        std::vector<lv_point_t> points_bottom;
    };

    class ChatBubble {
    public:
        ChatBubble(lv_obj_t *parent, lv_coord_t bubble_w, lv_coord_t bubble_h, lv_coord_t pointer_w,
                   lv_coord_t pointer_h);

        void update_pointer_location(int x, bool up);

        void update_color(lv_color_t color);

        void update_content(const char *content);

        void set_visible(bool show);

        lv_obj_t *m_space;
    private:
        BubblePointer m_pointer;
        lv_obj_t *m_card;
        lv_obj_t *m_label;
    };

    class CardArea {
    public:
        CardArea(lv_obj_t *parent, lv_coord_t pos_init, lv_coord_t pos_offset);

        void move_away(lv_anim_ready_cb_t cb, void *user_data) const;

        void move_away() const;

        void move_back() const;

        lv_obj_t *m_area;
    private:
        lv_coord_t m_pos_init;
        lv_coord_t m_pos_offset;
    };

    class UIPlantStatus : public Base {
    public:
        explicit UIPlantStatus(ObjPtr obj);

        typedef enum {
            light = 0,
            temp,
            humidity,
            soil,
            water,
            battery,
        } sensor_index_t;

        typedef struct {
            std::string content;
            lv_color_t color;
        } bubble_info_t;

        StatusCard &get_card_by_index(sensor_index_t index) {
            return m_cards[index];
        }

        void routine() override;

        void update();

        void select_index(int index);

        bubble_info_t get_bubble_info_by_index(int index);

		void left_input_cb(InputData data) override;

		void right_input_cb(InputData data) override;

        void hide_bubble_cb();

        void show_bubble_cb();

    private:
        bool _initialize() override;

        bool _deInitialize() override;

        void update_selection_cb(int index);

        static lv_coord_t get_pointer_x_by_index(int index);

        void update_bubble_status(const char *content, lv_color_t color, lv_coord_t x, bool check_selection = true);

        void move_bubble() const;

        void select_next();

        void select_last();

        Indicator mIndicator;
        CardArea top_area;
        ChatBubble bubble;
        CardArea bottom_area;
        std::vector<StatusCard> m_cards;

        typedef enum {
            top_selected,
            no_selection,
            bottom_selected,
        } area_select_t;

        void set_focus(area_select_t t_focus);

        int m_current_index = -1;
        area_select_t last_area_select = no_selection;
        area_select_t current_area_select = no_selection;

        lv_timer_t *m_value_update_timer = nullptr;
        bool m_cached_charging = false;
        Plant::full_status_t m_cached_status;
    };
}

#endif //LVGL_UIPLANTSTATUS_H
