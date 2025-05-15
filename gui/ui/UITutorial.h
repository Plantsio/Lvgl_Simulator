//
// Created by Gordon on 2022/5/6.
//

#ifndef UI_SANDBOX_TUTORIALUI_H
#define UI_SANDBOX_TUTORIALUI_H

#include "UIBase.h"
#include "StepBase.h"
#include "Indicator.h"
#include "BasicText.h"
#include "Image.h"
#include "UIFluidAssist.h"
//#include "Skin.h"

namespace UI
{
    class InputReceiver
    {
    public:
        virtual void handleInput(InputEvtType &input) = 0;

    protected:
        InputEvtType mInputEvtType{};
    };

    class TuComponentBase : public Initializable,public StepBase
    {
    public:
        TuComponentBase():StepBase(){}
    private:
        bool _initialize() override {return true;}
    };

    class TuIntro : public TuComponentBase
	{
	public:
		explicit TuIntro(lv_obj_t *parent);

	private:
        bool _initialize() final;

		void update(const std::string &main, const std::string &sub);

		void update_main(const std::string &main);

		void update_sub(const std::string &sub);

    private:
        BasicText mMainText;
        BasicText mSubText;
	};

    class TuTouchBar : public TuComponentBase, public InputReceiver
	{
	public:
		explicit TuTouchBar(lv_obj_t *parent);

        ~TuTouchBar() override;

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

	private:
        BasicText mTopText;
        BasicText mBottomText;
        Indicator mIndicator;
        Image     mImage;

        lv_obj_t *m_circles[2] = {};
	};

	class TuApp : public TuComponentBase, public InputReceiver
	{
	public:
		explicit TuApp(lv_obj_t *parent);

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

	private:
        BasicText mTopText;
        BasicText mSubText;
        BasicText mMidText;
        BasicText mBottomText;
        Indicator mIndicator;
        Image     mImage;
	};

	class TuWater : public TuComponentBase, public InputReceiver
	{
	public:
		explicit TuWater(lv_obj_t *parent);

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

    private:
        BasicText mTopText;
        Indicator mIndicator;
        Image     mImage;
	};

	class TuWaterAssist : public TuComponentBase, public InputReceiver
	{
	public:
		explicit TuWaterAssist(lv_obj_t *parent);

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

	private:
        BasicText mTopText;
        BasicText mMidText;
        BasicText mBottomText;
        Indicator mIndicator;

        int waterLevel = 0;
	};


    class TuPlantDetect : public TuComponentBase, public InputReceiver
	{
	public:
		explicit TuPlantDetect(lv_obj_t *parent);

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

    private:
        BasicText mTopText;
        Indicator mIndicator;
        Image     mImage;

        bool plantExist = false;
	};

    class TuFinal : public TuComponentBase
	{
	public:
		explicit TuFinal(lv_obj_t *parent);

    private:
        bool _initialize() final;

	private:
		BasicText mMidText;
	};

	class UITutorial : public Base
	{
	public:
		explicit UITutorial(ObjPtr obj);

        using ActionCB = std::function<void()>;
        struct TutorialComponents
        {
            std::shared_ptr<TuComponentBase> stepUI;
            ActionCB action;
        };

	public:
		void routine() override;

	private:
        bool _initialize() override;

        bool _handleInput(InputEvtType &&input) override;

	private:
        uint32_t mCurrent = 0;

		std::vector<TutorialComponents> uiList{};

		bool tutorial_over = false;
	};
}
void test();


#endif //UI_SANDBOX_TUTORIALUI_H
