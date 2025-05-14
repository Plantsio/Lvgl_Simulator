//
// Created by Huwensong on 2024/11/27.
//

#include "TimerStepBase.h"
#include "log.h"

namespace UI
{
	TimerStepBase::TimerStepBase(lv_obj_t *parent,const std::string &_id,HandlerCB start,HandlerCB end):
    mParent(parent),
	mTopText(parent, 300),
    mMainText(parent,300),
    mSubText(parent,300),
	mBottomText(parent, 250),
    mImage(parent),
    mID(_id),
	mStartCB(std::move(start)),
    mEndCB(std::move(end))
	{
		m_timer = lv_timer_create(timer_cb, COMMON_TIMER_DELAY, this);
		lv_timer_pause(m_timer);

		lv_obj_align(mTopText.get_origin_obj(), LV_ALIGN_TOP_MID, 0, 10);
		lv_obj_align(mBottomText.get_origin_obj(), LV_ALIGN_BOTTOM_MID, 0, -20);
	}

	TimerStepBase::~TimerStepBase()
	{
		log_d("deconstruct");
		if (m_timer)
		{
			lv_timer_del(m_timer);
		}
	}

	void TimerStepBase::start()
	{
		if (started)
			return;

        if (mStartCB)
            mStartCB();

		lv_obj_clear_flag(mParent,LV_OBJ_FLAG_HIDDEN);
		lv_timer_resume(m_timer);
		log_d("start ......");
		started = true;
		finished = false;
	}

	void TimerStepBase::end()
	{
		//lv_obj_add_flag(m_parent,LV_OBJ_FLAG_HIDDEN);
		lv_timer_pause(m_timer);
		started = false;
		finished = true;

        if (mEndCB)
            mEndCB();
		log_d("ui end ...");
	}

	bool TimerStepBase::is_finished() const
	{
		return finished;
	}

	void TimerStepBase::timer_cb(lv_timer_t *timer)
	{
		auto TimerStep = static_cast<TimerStepBase *>(timer->user_data);

        TimerStep->stepHandler();
	}

    void TimerStepBase::stepHandler()
    {
        if (nextReady())
            updateInfo(mStepInfoList[current ++]);
        if (prevReady())
            updateInfo(mStepInfoList[current --]);
    }

    bool TimerStepBase::prevReady()
    {
        return false;
    }

    bool TimerStepBase::nextReady()
    {
        return true;
    }

    void nextUpdateTimerPeriod()
    {}

    void TimerStepBase:: updateInfo(StepConfig &info)
    {
        mTopText.update(info.mainDesc);
        mMainText.update(info.mainDesc);
        mSubText.update(info.subDesc);
        mBottomText.update(info.buttonDesc);
        mImage.update(info.assets);
    }
}