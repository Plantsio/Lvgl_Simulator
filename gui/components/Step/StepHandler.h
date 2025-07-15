//
// Created by Huwensong on 2024/11/27.
//

#ifndef FIRMWARE_COMMONBASE_H
#define FIRMWARE_COMMONBASE_H

#include <functional>

#include "lvgl.h"
#include "BasicText.h"
#include "Image.h"

#define STEP_FAIL_NO_JUMP         -1

namespace UI
{
    class StepHandler
	{
    public:
        using Condition = std::function<bool(uint32_t &timeout)>;
        using SuccessHandler = std::function<void()>;
        using FailHandler = std::function<void(int &jump_step)>;

        struct StepUnit
        {
            Condition stepCondition;
            SuccessHandler stepSuccess;
            FailHandler stepFail;
        };

	public:
         StepHandler() = default;

		 ~StepHandler();

    public:
        void start();

        void stop();

        [[nodiscard]] bool finished() const;

        bool enableAutoStep();

        int getStepPercent();

        static void timer_cb(lv_timer_t *timer);

    public:
        void registerStepUnit(Condition condition, SuccessHandler success, FailHandler fail);

    private:
        void stepRoutine();

    private:
        bool mStarted = false;
        bool mFinished = false;

        int mCurrentStep = 0;

        long long mCurrentStartTime = 0;

		lv_timer_t *mTimer = nullptr;

        std::vector<StepUnit> mStepList{};
	};
}




#endif //FIRMWARE_COMMONBASE_H
