//
// Created by Huwensong on 2024/11/27.
//

#include "StepBase.h"
#include "log.h"

namespace UI
{
	StepBase::StepBase(HandlerCB start,HandlerCB end):
	mStartCB(std::move(start)),
    mStopCB(std::move(end))
	{
	}

    StepBase::StepBase():mStartCB(nullptr),mStopCB(nullptr){}

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
        if (nextReady())
        {
            if (mCurrentStep <= mStepList.size())
            {
                auto exe = mStepList[mCurrentStep];
                if (exe)
                    exe();
                mCurrentStep ++;
            }
            else
                stop();
        }

        if (prevReady())
        {
            if (mCurrentStep >= 0)
            {
                auto exe = mStepList[mCurrentStep];
                if (exe)
                    exe();
                mCurrentStep --;
            }
            else
                stop();
        }
    }

	void StepBase::start()
	{
        log_d("start ......");
		if (started)
			return;

        if (mStartCB)
            mStartCB();

        if (mTimer)
            lv_timer_resume(mTimer);

		started = true;
		finished = false;
	}

	void StepBase::stop()
	{
        log_d("ui end ...");

		started = false;
		finished = true;

        if (mTimer)
        lv_timer_pause(mTimer);

        if (mStopCB)
            mStopCB();

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

	bool StepBase::is_finished() const
	{
		return finished;
	}



	void StepBase::timer_cb(lv_timer_t *timer)
	{
		auto TimerStep = static_cast<StepBase *>(timer->user_data);

        TimerStep->stepHandler();
	}

    void StepBase::registerStepCB(executeCB cb)
    {
        mStepList.push_back(std::move(cb));
    }

    bool StepBase::prevReady()
    {
        return false;
    }

    bool StepBase::nextReady()
    {
        return true;
    }

    void nextUpdateTimerPeriod()
    {}
}