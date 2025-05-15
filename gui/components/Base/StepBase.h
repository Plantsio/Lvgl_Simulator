//
// Created by Huwensong on 2024/11/27.
//

#ifndef FIRMWARE_COMMONBASE_H
#define FIRMWARE_COMMONBASE_H

#include <functional>

#include "lvgl.h"
#include "BasicText.h"
#include "Image.h"

#define COMMON_TIMER_DELAY      6000

namespace UI
{
    class StepBase
	{
    public:
        using HandlerCB = std::function<void()>;

        using ReadyCB = std::function<bool()>;
        using executeCB = std::function<bool()>;

        struct StepUnit
        {
            ReadyCB currentReady;
            ReadyCB nextReady;
            ReadyCB prevReady;
            executeCB execute;
        };

	public:
		 StepBase(HandlerCB start,HandlerCB end);
         StepBase();

		 ~StepBase();

    public:
        void stepHandler();

        void start();

        void stop();

        [[nodiscard]] bool finished() const;

    protected:
        bool enableAutoStep();

        void updateStepPeriod(uint32_t period);

        static void timer_cb(lv_timer_t *timer);

    protected:
        void registerStepCB(executeCB exe,
                            ReadyCB currentReady = [](){return true;},
                            ReadyCB nextReady = [](){return true;},
                            ReadyCB prevReady = [](){return true;}
                            );

        void registerStartCB(HandlerCB start);

        void registerStopCB(HandlerCB stop);

    private:
        HandlerCB mStartCB;
        HandlerCB mStopCB;

        bool mStarted = false;
        bool mFinished = false;

        int mCurrentStep = 0;

		lv_timer_t *mTimer = nullptr;

        std::vector<StepUnit> mStepList{};
	};
}




#endif //FIRMWARE_COMMONBASE_H
