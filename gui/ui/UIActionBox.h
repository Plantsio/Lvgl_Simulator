//
// Created by sungaoran on 2022/2/27.
//

#ifndef LVGL_UIACTIONBOX_H
#define LVGL_UIACTIONBOX_H

#include "UIBase.h"
#include "gui_data_defines.h"

namespace UI {
    class UIActionBox : public Base
    {
    public:
        using actionCB = std::function<void()>;

        enum ActionStatus
        {
            ACTION_WAITING,
            ACTION_APPLY,
            ACTION_CANCEL,
            ACTION_OVER,
        };

    public:
        explicit UIActionBox(ObjPtr obj);

        bool _initialize() override;

        void update(const Gui::DataActionBox &desc);

        void routine() override;

        void left_input_cb(InputData data) override;

        void right_input_cb(InputData data) override;

    private:
        void updateTitle(const std::string &title);

        void updateDesc(const std::string &desc);

        void updateApplyDesc(const std::string &apply);

        void updateCancelDesc(const std::string &cancel);

        void setCountingMax(int max);

        void setApplyCB(actionCB apply);

        void setCancelCB(actionCB cancel);

        void setFinalCB(actionCB final);

        void check();

        void apply();

        void cancel();

        void exit();

    private:
        lv_obj_t *mTitleLabel;
        lv_obj_t *mDescLabel;
        lv_obj_t *mLeftButton;
        lv_obj_t *mRightButton;
        lv_obj_t *mTouchLeftLabel;
        lv_obj_t *mTouchRightLabel;
        lv_obj_t *mCountingArc;
        lv_obj_t *mCountingLabel;

        actionCB mApplyCB;
        actionCB mCancelCB;
        actionCB mFinalCB;

        int mCountingMax = 0;
        int mCountingTimeout = 0;

        ActionStatus mStatus = ACTION_WAITING;
    };
}

#endif //LVGL_UIACTIONBOX_H
