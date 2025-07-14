//
// Created by Gordon on 2022/5/6.
//

#ifndef UI_SANDBOX_TUTORIALUI_H
#define UI_SANDBOX_TUTORIALUI_H


#include <stack>

#include "UIBase.h"
#include "StepBase.h"
#include "Indicator.h"
#include "BasicText.h"
#include "Image.h"
#include "UIFluidAssist.h"
#include "log.h"

namespace UI
{
    class InputReceiver
    {
    public:
        virtual void handleInput(InputEvtType &input) = 0;

    protected:
        bool pressed(InputEvent target){
            bool ret = mInputEvtType.index == target ? true : false;
            mInputEvtType = {};
            return ret;
        };

        InputEvtType mInputEvtType{};
    };

    class TuBase : public Initializable,public StepBase
    {
    public:
        TuBase():StepBase(){}
    private:
        bool _initialize() override {return true;}
    };

    class TuIntro : public TuBase
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

    class TuTouchBar : public TuBase, public InputReceiver
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
        //Image     mImage;

        lv_obj_t *m_circles[2] = {};
	};

	class TuApp : public TuBase, public InputReceiver
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

	class TuWater : public TuBase, public InputReceiver
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

	class TuWaterAssist : public TuBase, public InputReceiver
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


    class TuPlantDetect : public TuBase, public InputReceiver
	{
	public:
		explicit TuPlantDetect(lv_obj_t *parent);

    private:
        bool _initialize() final;

        void handleInput(InputEvtType &input) override;

    private:
        BasicText mTopText;
        Indicator mIndicator;
        BasicText mBottomText;
        Image     mImage;

        bool plantExist = false;
	};

    class TuFinal : public TuBase
	{
	public:
		explicit TuFinal(lv_obj_t *parent);

    private:
        bool _initialize() final;

	private:
		BasicText mMidText;
	};


    class DotWidget
    {
    public:
        explicit DotWidget(lv_obj_t *parent,lv_color_t base_color,const std::string &label_value = "");

        ~DotWidget();

    public:
        void updateContent(const std::string &value);

        void updateColor(lv_color_t value);

        void align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

        void align_to(const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

        void align_to(const DotWidget &base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

    private:
        lv_obj_t *group;
        lv_obj_t *dotMain = nullptr;
        lv_obj_t *dotSub[3] = {nullptr};
        lv_obj_t *dotContent = nullptr;
        lv_style_t groupStyle{};
        lv_style_t dotStyle{};
    };

    class UITutorial : public Base, public StepBase
	{
	public:
		explicit UITutorial(ObjPtr obj);
    public:
        enum TuIndex
        {
            Tu_Intro,
            Tu_TouchBar,
            Tu_App,
            Tu_ProvTip,
            Tu_Water,
            Tu_WaterAssist,
            Tu_PlantDetect,
            Tu_Final,
        };

        using ActionCB = std::function<void()>;
        struct TutorialUnit
        {
            TuIndex index;
            ActionCB startCB;
        };

	public:
		void routine() override;

	private:
        bool _initialize() override;

        bool _handleInput(InputEvtType &&input) override;

        std::shared_ptr<TuBase> build(TuIndex index);

        void tutorial_over();

	private:
        lv_obj_t *mProcessUnit;
        lv_obj_t *mUIUnit;
        lv_style_t mStyle{};

		std::vector<TuIndex> uiList{};
        std::shared_ptr<TuBase> mCurUI= nullptr;
        uint32_t mCurIndex = 0;

        std::vector<std::unique_ptr<DotWidget>> mDots{};
	};
}
void test();


#endif //UI_SANDBOX_TUTORIALUI_H
