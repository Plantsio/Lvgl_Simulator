//
// Created by Gordon on 2022/5/6.
//

#ifndef UI_SANDBOX_TUTORIALUI_H
#define UI_SANDBOX_TUTORIALUI_H

#include <stack>

#include "UIBase.h"
#include "TimerStepBase.h"
#include "Indicator.h"
#include "BasicText.h"
#include "Image.h"
#include "UIFluidAssist.h"
#include "Skin.h"

namespace UI
{
    class TuBase : public TimerStepBase
	{
	public:
		explicit TuBase(lv_obj_t *parent,Indicator &indicator);

		~TuBase() override;

	public:
		void handle_left_touch();

		void handle_right_touch();

	protected:
		Image m_image;
		Indicator m_indicator;

	protected:
		bool m_left_touched = false;
		bool m_right_touched = false;
	};

	class TuIntro : public TuBase
	{
	public:
		explicit TuIntro(lv_obj_t *parent,Indicator &indicator);

		void next() override;

	private:
		void update(const std::string &main, const std::string &sub);

		void update_main(const std::string &main);

		void update_sub(const std::string &sub);

		void realign() const;
	};

	class TuTouchBar : public TuBase
	{
	public:
		explicit TuTouchBar(lv_obj_t *parent,Indicator &indicator);

		void next() override;

	private:
		lv_obj_t *m_circles[2] = {};
	};

	class TuApp : public TuBase
	{
	public:
		explicit TuApp(lv_obj_t *parent,Indicator &indicator);

		void next() override;

	private:
		BasicText m_sub_text;
		BasicText m_mid_text;
	};

	class TuWater : public TuBase
	{
	public:
		explicit TuWater(lv_obj_t *parent,Indicator &indicator);

		void next() override;
	};

	class TuWaterAssist : public TuBase
	{
	public:
		explicit TuWaterAssist(lv_obj_t *parent,Indicator &indicator);

		void next() override;

	private:
		bool has_water = false;
		BasicText m_middle_text;
	};


	class TuPlantDetect : public TuBase
	{
	public:
		explicit TuPlantDetect(lv_obj_t *parent,Indicator &indicator);

		void next() override;
	};

	class TuFinal : public TuBase
	{
	public:
		explicit TuFinal(lv_obj_t *parent,Indicator &indicator);

		void next() override;

	private:
		BasicText m_text;
	};

	class UITutorial : public Base
	{
	public:
		explicit UITutorial(ObjPtr obj);

        using ActionCB = std::function<void()>;
        struct TutorialComponents
        {
            std::shared_ptr<TimerStepBase> ui;
            ActionCB action;
        };

	public:
		void routine() override;

	private:
        bool _initialize() override;

        bool _handleInput(InputEvtType &&input) override;

	private:
        Indicator mIndicator;

        TutorialComponents mCurrentComponent;

		std::stack<TutorialComponents> uiStack{};

		bool tutorial_over = false;
	};
}


#endif //UI_SANDBOX_TUTORIALUI_H
