#include "ThreeDotsLoading.h"
#include <stdlib.h>

ThreeDotsLoading::ThreeDotsLoading(lv_obj_t* parent, Orientation orientation)
    : parent_(parent ? parent : lv_scr_act()),
      orientation_(orientation),
      dot_size_(10),
      spacing_(15),
      animation_duration_(500),
      dot_color_(lv_color_hex(0x000000)),
      pos_x_(LV_ALIGN_CENTER),
      pos_y_(LV_ALIGN_CENTER),
      is_running_(false)
{
    createDots();
}

ThreeDotsLoading::~ThreeDotsLoading() {
    stop();
    deleteDots();
}

void ThreeDotsLoading::createDots() {
    for (int i = 0; i < 3; i++) {
        lv_obj_t* dot = lv_obj_create(parent_);
        lv_obj_set_size(dot, dot_size_, dot_size_);
        lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(dot, dot_color_, 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        lv_obj_remove_style(dot, NULL, LV_PART_SCROLLBAR); // Remove any default styles
        lv_obj_clear_flag(dot, LV_OBJ_FLAG_SCROLLABLE);
        dots_.push_back(dot);
    }
    updateLayout();
}

void ThreeDotsLoading::setupAnimations() {
    deleteAnimations();

    for (size_t i = 0; i < dots_.size(); i++) {
        lv_anim_t* anim = static_cast<lv_anim_t*>(malloc(sizeof(lv_anim_t)));
        animations_.push_back(anim);
        setupAnimationForDot(anim, dots_[i], i * (animation_duration_ / 3));
    }

    is_running_ = true;
}

void ThreeDotsLoading::setupAnimationForDot(lv_anim_t* anim, lv_obj_t* dot, uint32_t delay) {
    lv_anim_init(anim);
    lv_anim_set_var(anim, dot);
    lv_anim_set_time(anim, animation_duration_);
    lv_anim_set_playback_time(anim, animation_duration_);
    lv_anim_set_repeat_count(anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(anim, lv_anim_path_ease_in_out);
    lv_anim_set_delay(anim, delay);

    // Get current position
    lv_coord_t x = lv_obj_get_x(dot);
    lv_coord_t y = lv_obj_get_y(dot);

    if (orientation_ == HORIZONTAL) {
        // Dots in horizontal line - animate vertically
        lv_anim_set_values(anim, y - dot_size_, y + dot_size_);
        lv_anim_set_exec_cb(anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
    } else {
        // Dots in vertical line - animate horizontally
        lv_anim_set_values(anim, x - dot_size_, x + dot_size_);
        lv_anim_set_exec_cb(anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
    }

    lv_anim_start(anim);
}

void ThreeDotsLoading::start() {
    if (!is_running_) {
        setupAnimations();
    }
}

void ThreeDotsLoading::stop() {
    for (auto anim : animations_) {
        lv_anim_del(anim->var, anim->exec_cb);
        free(anim);
    }
    animations_.clear();
    is_running_ = false;
}

void ThreeDotsLoading::setVisible(bool visible) {
    for (auto dot : dots_) {
        if (visible) {
            lv_obj_clear_flag(dot, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(dot, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void ThreeDotsLoading::setOrientation(Orientation orientation) {
    if (orientation_ != orientation) {
        orientation_ = orientation;
        updateLayout();
        if (is_running_) {
            stop();
            start();
        }
    }
}

void ThreeDotsLoading::setPosition(int16_t x, int16_t y) {
    pos_x_ = x;
    pos_y_ = y;
    updateLayout();
}

void ThreeDotsLoading::setDotSize(uint8_t size) {
    if (dot_size_ != size) {
        dot_size_ = size;
        for (auto dot : dots_) {
            lv_obj_set_size(dot, dot_size_, dot_size_);
        }
        updateLayout();
    }
}

void ThreeDotsLoading::setDotColor(lv_color_t color) {
    dot_color_ = color;
    for (auto dot : dots_) {
        lv_obj_set_style_bg_color(dot, dot_color_, 0);
    }
}

void ThreeDotsLoading::setAnimationDuration(uint16_t duration_ms) {
    if (animation_duration_ != duration_ms) {
        animation_duration_ = duration_ms;
        if (is_running_) {
            stop();
            start();
        }
    }
}

void ThreeDotsLoading::setSpacing(uint8_t spacing) {
    if (spacing_ != spacing) {
        spacing_ = spacing;
        updateLayout();
    }
}

void ThreeDotsLoading::deleteDots() {
    for (auto dot : dots_) {
        lv_obj_del(dot);
    }
    dots_.clear();
}

void ThreeDotsLoading::deleteAnimations() {
    stop();
}

void ThreeDotsLoading::updateLayout() {
    for (size_t i = 0; i < dots_.size(); i++) {
        if (orientation_ == HORIZONTAL) {
            // Horizontal layout: dots are side by side
            lv_obj_align(dots_[i], LV_ALIGN_CENTER,
                        (i - 1) * (dot_size_ + spacing_), 0);
        } else {
            // Vertical layout: dots are stacked
            lv_obj_align(dots_[i], LV_ALIGN_CENTER,
                        0, (i - 1) * (dot_size_ + spacing_));
        }

        // Apply position offset
        lv_coord_t x = lv_obj_get_x(dots_[i]);
        lv_coord_t y = lv_obj_get_y(dots_[i]);
        lv_obj_set_pos(dots_[i], x + pos_x_, y + pos_y_);
    }
}
