#ifndef THREE_DOTS_LOADING_H
#define THREE_DOTS_LOADING_H

#include "lvgl.h"
#include <vector>

class ThreeDotsLoading {
public:
    enum Orientation {
        HORIZONTAL,
        VERTICAL
    };

    explicit ThreeDotsLoading(lv_obj_t* parent = nullptr, Orientation orientation = HORIZONTAL);
    ~ThreeDotsLoading();

    void start();
    void stop();
    void setVisible(bool visible);
    void setOrientation(Orientation orientation);
    void setPosition(int16_t x, int16_t y);
    void setDotSize(uint8_t size);
    void setDotColor(lv_color_t color);
    void setAnimationDuration(uint16_t duration_ms);
    void setSpacing(uint8_t spacing);

private:
    void createDots();
    void setupAnimations();
    void deleteDots();
    void deleteAnimations();
    void updateLayout();
    void setupAnimationForDot(lv_anim_t* anim, lv_obj_t* dot, uint32_t delay);

    lv_obj_t* parent_;
    std::vector<lv_obj_t*> dots_;
    std::vector<lv_anim_t*> animations_;

    Orientation orientation_;
    uint8_t dot_size_;
    uint8_t spacing_;
    uint16_t animation_duration_;
    lv_color_t dot_color_;
    lv_coord_t pos_x_;
    lv_coord_t pos_y_;
    bool is_running_;
};

#endif // THREE_DOTS_LOADING_H
