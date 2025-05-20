//
// Created by Huwensong on 2024/11/27.
//

#ifndef FIRMWARE_COMMONBASE_H
#define FIRMWARE_COMMONBASE_H

#include <functional>

#include "lvgl.h"
#include "BasicText.h"
#include "Image.h"

#define STEP_ONE_TIME             1
#define STEP_INFINITY            -1

#define COMMON_TIMER_DELAY      6000

namespace UI
{
    class StepBase
	{
    public:

        using ReadyCB = std::function<bool()>;
        using executeCB = std::function<bool()>;

        struct StepUnit
        {
            int32_t repeat_count;    /**< 1: One time;  -1 : infinity;  n>0: residual times*/
            ReadyCB nextReady;
            ReadyCB prevReady;
            executeCB execute;
        };

	public:
         StepBase() = default;

		 ~StepBase();

    public:
        void start();

        void stop();

        [[nodiscard]] bool finished() const;

    protected:
        void stepHandler();

        bool enableAutoStep();

        void updateStepPeriod(uint32_t period);

        static void timer_cb(lv_timer_t *timer);

    protected:
        void registerStepCB(executeCB exe,
                            int32_t repeat_count = STEP_ONE_TIME,
                            ReadyCB nextReady = [](){return true;},
                            ReadyCB prevReady = nullptr
                            );

    private:
        bool mStarted = false;
        bool mFinished = false;

        int mCurrentStep = 0;

		lv_timer_t *mTimer = nullptr;

        std::vector<StepUnit> mStepList{};
	};
}




#endif //FIRMWARE_COMMONBASE_H
