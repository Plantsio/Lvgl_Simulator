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

        using nextReadyCB = std::function<bool()>;
        using prevReadyCB = std::function<bool()>;
        using executeCB = std::function<bool()>;

	public:
		 StepBase(HandlerCB start,HandlerCB end);
         StepBase();

		 ~StepBase();

    public:
        void stepHandler();

	protected:
        void start();

        void stop();

        bool enableAutoStep();

        [[nodiscard]] bool is_finished() const;

        void registerStepCB(executeCB cb);

        virtual bool prevReady();

        virtual bool nextReady();

        static void timer_cb(lv_timer_t *timer);

	protected:
        HandlerCB mStartCB;
        HandlerCB mStopCB;

    private:
        bool started = false;
        bool finished = false;

        int mCurrentStep = 0;

		lv_timer_t *mTimer = nullptr;

        std::vector<executeCB> mStepList{};
	};
}




#endif //FIRMWARE_COMMONBASE_H
