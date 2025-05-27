//
// Created by Huwensong on 2024/11/27.
//

#include "StepBase.h"
#include "log.h"

namespace UI
{
	StepBase::~StepBase()
	{
		log_d("deconstruct");
		if (mTimer)
		{
			lv_timer_del(mTimer);
		}
	}

    void StepBase::stepHandler()
    {
        if (!mTimer && !mStarted)
            return;

        auto nextReady = mStepList[mCurrentStep].nextReady;
        auto prevReady= mStepList[mCurrentStep].prevReady;
        auto execute = mStepList[mCurrentStep].execute;

        int32_t original_repeat_count = mStepList[mCurrentStep].repeat_count;
        if(mStepList[mCurrentStep].repeat_count > 0) mStepList[mCurrentStep].repeat_count--;

        if (execute && original_repeat_count != 0)
        {
            log_d("current");
            execute();
        }



        if (prevReady && prevReady())
        {
            log_d("prev");
            mCurrentStep --;
            if (mCurrentStep < 0)
                stop();
        }

        if (nextReady && nextReady())
        {
            log_d("next");
            mCurrentStep ++;
            if (mCurrentStep > mStepList.size() - 1)
                stop();
        }
    }

	void StepBase::start()
	{
		if (mStarted)
			return;

        log_d("step start");

        if (mTimer)
            lv_timer_resume(mTimer);

        mStarted = true;
        mFinished = false;
	}

	void StepBase::stop()
	{
        if (!mStarted)
            return;
        log_d("step stop");

        mStarted = false;
        mFinished = true;

        if (mTimer)
            lv_timer_pause(mTimer);

	}

    bool StepBase::finished() const
    {
        return mFinished;
    }

    bool StepBase::enableAutoStep()
    {
        mTimer = lv_timer_create(timer_cb, COMMON_TIMER_DELAY, this);
        if (!mTimer)
        {
            log_e("StepBase: Failed to create timer");
            return false;
        }
        lv_timer_pause(mTimer);
        return true;
    }

    void StepBase::updateStepPeriod(uint32_t period)
    {
        lv_timer_set_period(mTimer,period);
    }

	void StepBase::timer_cb(lv_timer_t *timer)
	{
		auto TimerStep = static_cast<StepBase *>(timer->user_data);

        TimerStep->stepHandler();
	}

    void StepBase::registerStepCB(executeCB exe,int32_t repeat_count,ReadyCB nextReady,ReadyCB prevReady)
    {
        mStepList.push_back({repeat_count,std::move(nextReady),std::move(prevReady),std::move(exe)});
    }
}