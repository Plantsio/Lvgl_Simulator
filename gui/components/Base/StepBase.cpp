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
        auto curReady = mStepList[mCurrentStep].currentReady;
        auto nextReady = mStepList[mCurrentStep].nextReady;
        auto prevReady= mStepList[mCurrentStep].prevReady;
        auto exe = mStepList[mCurrentStep].execute;

        if (curReady && curReady())
        {
            if (exe)
                exe();
        }

        if (prevReady && prevReady())
        {
            if (mCurrentStep >= 0)
                mCurrentStep --;
            else
                stop();
        }

        if (nextReady || nextReady())
        {
            if (mCurrentStep <= mStepList.size())
                mCurrentStep ++;
            else
                stop();
        }
    }

	void StepBase::start()
	{
        log_d("start ......");
		if (mStarted)
			return;

        if (mStartCB)
            mStartCB();

        if (mTimer)
            lv_timer_resume(mTimer);

        mStarted = true;
        mFinished = false;
	}

	void StepBase::stop()
	{
        log_d("ui end ...");

        mStarted = false;
        mFinished = true;

        if (mTimer)
            lv_timer_pause(mTimer);

        if (mStopCB)
            mStopCB();

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

    void StepBase::registerStepCB(executeCB exe,ReadyCB currentReady,ReadyCB nextReady,ReadyCB prevReady)
    {
        mStepList.push_back({std::move(currentReady),std::move(nextReady),std::move(prevReady),std::move(exe)});
    }

    void StepBase::registerStartCB(HandlerCB start)
    {
        mStartCB = std::move(start);
    }

    void StepBase::registerStopCB(HandlerCB stop)
    {
        mStopCB = std::move(stop);
    }
}