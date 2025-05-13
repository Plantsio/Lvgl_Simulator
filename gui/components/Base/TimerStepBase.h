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
    using nextReadyCB = std::function<bool()>;
    using prevReadyCB = std::function<bool()>;

    struct StepConfig
    {
        std::string topDesc;
        std::string mainDesc;
        std::string assets;
        std::string subDesc;
        std::string buttonDesc;
        uint32_t durationSec;   //步骤持续时间, 0 表示默认时间。
    };

	class TimerStepBase
	{
	public:
        using HandlerCB = std::function<void()>;

		explicit TimerStepBase(lv_obj_t *parent,const std::string &_id,HandlerCB start,HandlerCB end);
		virtual ~TimerStepBase();
	public:
		void start();

		void end();

		[[nodiscard]] bool is_finished() const;

        void stepHandler();

	protected:
		static void timer_cb(lv_timer_t *timer);

        virtual bool prevReady();

        virtual bool nextReady();

        void updateInfo(StepConfig &info);


		virtual void next() = 0;

	protected:
        lv_obj_t *mParent;

        std::unique_ptr<BasicText> mTopText = nullptr;
        BasicText mMainText;
        BasicText mSubText;
		BasicText mBottomText;
        Image     mImage;

        std::vector<StepConfig> mStepInfoList{};

        std::string mID;

        HandlerCB mStartCB;
        HandlerCB mEndCB;

		int m_current_step = 0;
		lv_timer_t *m_timer = nullptr;

		bool started = false;
		bool finished = false;

        int current = 0;
	};
}




#endif //FIRMWARE_COMMONBASE_H
