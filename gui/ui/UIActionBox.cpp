//
// Created by sungaoran on 2022/2/27.
//

#include "UIActionBox.h"

#include "Lang.h"
#include "ThemeInterface.h"
#include "Animation.h"
#include "SysTime.h"

namespace UI {
    UIActionBox::UIActionBox(ObjPtr obj) : Base(std::move(obj)){
        mTitleLabel = lv_label_create(m_scr);
        lv_obj_set_width(mTitleLabel, 240);
        THEME_SET_FONT_SIZE(mTitleLabel,30);

        mDescLabel = lv_label_create(m_scr);
        lv_obj_set_width(mDescLabel, 240);
        lv_label_set_long_mode(mDescLabel,LV_LABEL_LONG_SCROLL_CIRCULAR);
        THEME_SET_FONT_SIZE(mDescLabel,16);

        mLeftButton = lv_label_create(m_scr);
        lv_obj_set_width(mLeftButton, 60);
        lv_label_set_long_mode(mLeftButton,LV_LABEL_LONG_SCROLL_CIRCULAR);
        THEME_SET_FONT_SIZE(mLeftButton,30);

        mRightButton = lv_label_create(m_scr);
        lv_obj_set_width(mRightButton, 60);
        lv_label_set_long_mode(mRightButton,LV_LABEL_LONG_SCROLL_CIRCULAR);
        THEME_SET_FONT_SIZE(mRightButton,30);

        mTouchLeftLabel = lv_label_create(m_scr);
        THEME_SET_FONT_SIZE(mTouchLeftLabel,16);
        lv_label_set_text(mTouchLeftLabel, THEME_TEXT_CONTENT(Lang::ui_actionbox_touch_left).c_str());



        mTouchRightLabel = lv_label_create(m_scr);
        THEME_SET_FONT_SIZE(mTouchRightLabel,16);
        lv_label_set_text(mTouchRightLabel,THEME_TEXT_CONTENT(Lang::ui_actionbox_touch_right).c_str());

        mCountingLabel = lv_label_create(m_scr);
        THEME_SET_FONT_SIZE(mCountingLabel,16);
        lv_obj_add_flag(mCountingLabel, LV_OBJ_FLAG_HIDDEN);

        mCountingArc = lv_arc_create(m_scr);
        lv_obj_set_size(mCountingArc, 50, 50);
        lv_obj_set_style_arc_width(mCountingArc, 2, 0);
        lv_obj_set_style_arc_width(mCountingArc, 2, LV_PART_INDICATOR);
        lv_arc_set_rotation(mCountingArc, 0);
        lv_arc_set_value(mCountingArc, 100);
        lv_arc_set_bg_angles(mCountingArc, 0, 360);
        lv_obj_set_style_arc_color(mCountingArc, lv_color_black(), 0);
        lv_obj_set_style_arc_color(mCountingArc, lv_color_make(200, 200, 200), LV_PART_INDICATOR);
        lv_obj_remove_style(mCountingArc, NULL, LV_PART_KNOB);
        lv_obj_clear_flag(mCountingArc, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(mCountingArc, LV_OBJ_FLAG_HIDDEN);
    }

    bool UIActionBox::_initialize() {
        enable_input(INPUT_SRC_MASK(BtnSrcLeft) | INPUT_SRC_MASK(BtnSrcRight));
        Animation::anim_create(mTouchLeftLabel, Animation::anim_fade_cb, LV_OPA_TRANSP, LV_OPA_COVER, 2000, 0, 2500,
                               1200,LV_ANIM_REPEAT_INFINITE);

        Animation::anim_create(mTouchRightLabel, Animation::anim_fade_cb, LV_OPA_TRANSP, LV_OPA_COVER, 2000, 0, 2500,
                               1200,LV_ANIM_REPEAT_INFINITE);
        return true;
    }


    void UIActionBox::update(const Gui::DataActionBox &desc)
    {
        updateTitle(desc.title);
        updateDesc(desc.description);
        updateApplyDesc(desc.applyDesc);
        updateCancelDesc(desc.cancelDesc);

        setApplyCB(desc.applyCB);
        setCancelCB(desc.cancelCB);
        setFinalCB(desc.finalCB);

        setCountingMax(desc.countdownTimeout);
    }

    void UIActionBox::routine() {
//        if (mData.has_value())
//        {
//            auto data = std::any_cast<Gui::DataActionBox>(mData);
//            update(data);
//            mData.reset();
//        }
        check();
    }

	void UIActionBox::left_input_cb(InputData data)
	{
		mStatus = ACTION_CANCEL;
	}

	void UIActionBox::right_input_cb(InputData data)
	{
        mStatus = ACTION_APPLY;
	}

    void UIActionBox::updateTitle(const std::string &title)
    {
        lv_label_set_text(mTitleLabel, title.c_str());
        lv_obj_set_style_text_align(mTitleLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align(mTitleLabel, LV_ALIGN_CENTER, 0, -60);
    }

    void UIActionBox::updateDesc(const std::string &desc)
    {
        lv_label_set_text(mDescLabel, desc.c_str());
        lv_obj_set_style_text_align(mDescLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align_to(mDescLabel, mTitleLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    }

    void UIActionBox::updateApplyDesc(const std::string &apply)
    {
        lv_label_set_text(mRightButton, apply.c_str());
        lv_obj_set_style_text_align(mRightButton, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align_to(mRightButton, mDescLabel, LV_ALIGN_OUT_BOTTOM_MID, 80, 20);

        lv_obj_set_style_text_align(mTouchRightLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align_to(mTouchRightLabel, mRightButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }

    void UIActionBox::updateCancelDesc(const std::string &cancel)
    {
        lv_label_set_text(mLeftButton, cancel.c_str());
        lv_obj_set_style_text_align(mLeftButton, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align_to(mLeftButton, mDescLabel, LV_ALIGN_OUT_BOTTOM_MID, -80, 20);

        lv_obj_set_style_text_align(mTouchLeftLabel, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_align_to(mTouchLeftLabel, mLeftButton, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }

    void UIActionBox::setCountingMax(int max)
    {
        mCountingMax = max;
        mCountingTimeout = SysTime::millis() + max * 1000;
        lv_obj_align_to(mCountingArc, mDescLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    }

    void UIActionBox::setApplyCB(actionCB apply)
    {
        mApplyCB = std::move(apply);
    }

    void UIActionBox::setCancelCB(actionCB cancel)
    {
        mCancelCB = std::move(cancel);
    }

    void UIActionBox::setFinalCB(actionCB final)
    {
        mFinalCB = std::move(final);
    }

    void UIActionBox::check()
    {
        switch (mStatus)
        {
            case ACTION_WAITING:
                if (mCountingTimeout > 0)
                {
                    int time_left = mCountingTimeout - (int)SysTime::millis();
                    if (time_left >= 0)
                    {
                        uint32_t time_left_seconds = time_left / 1000;
                        std::string time_left_str = std::to_string(time_left_seconds) + "s";
                        if (time_left_str != lv_label_get_text(mCountingLabel))
                        {
                            lv_label_set_text(mCountingLabel, time_left_str.c_str());
                            lv_obj_align_to(mCountingLabel, mCountingArc, LV_ALIGN_CENTER, 0, 0);
                            Animation::anim_create(mCountingArc, (lv_anim_exec_xcb_t) lv_arc_set_value,
                                                   lv_arc_get_value(mCountingArc),
                                                   time_left_seconds * 100 / mCountingMax,
                                                   300);
                        }
                    }
                    else
                    {
                        mCountingTimeout = 0;
                        cancel();
                        exit();
                    }
                    lv_obj_clear_flag(mCountingArc, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(mCountingLabel, LV_OBJ_FLAG_HIDDEN);
                }
                break;
            case ACTION_APPLY:
                mStatus = ACTION_OVER;
                apply();
                exit();
                break;
            case ACTION_CANCEL:
                mStatus = ACTION_OVER;
                cancel();
                exit();
                break;
            default:
                break;
        }
    }

    void UIActionBox::apply()
    {
        if (mApplyCB)
            mApplyCB();
    }

    void UIActionBox::cancel()
    {
        if (mCancelCB)
            mCancelCB();
    }

    void UIActionBox::exit()
    {
        if (mFinalCB)
            mFinalCB();

        externalClose();
    }
}