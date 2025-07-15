//
// Created by Huwensong on 2024/11/27.
//

#include "StepHandler.h"
#include "log.h"
#include "interface.h"

namespace UI
{
	StepHandler::~StepHandler()
	{
		log_d("deconstruct");
		if (mTimer)
		{
			lv_timer_del(mTimer);
		}
	}

    void StepHandler::stepRoutine()
    {
        int32_t nextStep = mCurrentStep;

        if (mCurrentStep >= mStepList.size())
        {
            mFinished = true;
            return;
        }

        auto curStepUnit = mStepList.at(mCurrentStep);
        auto checkCondition = curStepUnit.stepCondition;

        if (checkCondition)
        {
            uint32_t timeout = 0;
            bool success = checkCondition(timeout);
            if (success)
            {
                if (curStepUnit.stepSuccess)
                    curStepUnit.stepSuccess();

                nextStep ++;
            }
            else
            {
                if (Millis() - mCurrentStartTime > timeout)
                {

                    if (curStepUnit.stepFail)
                        curStepUnit.stepFail(nextStep);

                    if (nextStep == STEP_FAIL_NO_JUMP)
                        nextStep ++;
                }
            }
        }
        else
        {
            nextStep ++;
        }

        if (mCurrentStep != nextStep)
            mCurrentStartTime = Millis();

        mCurrentStep = nextStep;


    }

	void StepHandler::start()
	{
		if (mStarted)
			return;

        log_d("step start");

        if (mTimer)
            lv_timer_resume(mTimer);

        mStarted = true;
        mFinished = false;
	}

	void StepHandler::stop()
	{
        if (!mStarted)
            return;
        log_d("step stop");

        mStarted = false;
        mFinished = true;

        if (mTimer)
            lv_timer_pause(mTimer);

	}

    bool StepHandler::finished() const
    {
        return mFinished;
    }

    bool StepHandler::enableAutoStep()
    {
        mTimer = lv_timer_create(timer_cb, 5, this);
        if (!mTimer)
        {
            log_e("StepBase: Failed to create timer");
            return false;
        }
        lv_timer_pause(mTimer);
        return true;
    }

    int StepHandler::getStepPercent()
    {
        return mCurrentStep * 100 / mStepList.size();
    }

	void StepHandler::timer_cb(lv_timer_t *timer)
	{
		auto TimerStep = static_cast<StepHandler *>(lv_timer_get_user_data(timer));

        TimerStep->stepRoutine();
	}

    void StepHandler::registerStepUnit(Condition condition, SuccessHandler success, FailHandler fail)
    {
        mStepList.push_back({std::move(condition),std::move(success),std::move(fail)});
    }
}