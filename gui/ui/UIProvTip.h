//
// Created by Huwensong on 2023/2/6.
//

#ifndef TUYA_PLANTS_IVY_UIPROVTIP_H
#define TUYA_PLANTS_IVY_UIPROVTIP_H

#include "UIBase.h"
#include "TimerStepBase.h"
#include "Image.h"
#include "BasicText.h"
#include "IOTManager.h"
#include "gui_data_defines.h"

namespace UI {
    class ProvTip : public TimerStepBase
	{
    public:
		explicit ProvTip(lv_obj_t *_parent,const Gui::ObjData &data);

	private:
		bool checkError();

		bool reach_status(IOTManager::NetStatus target, const std::string &processing_text);

		void next() override;

        std::optional<std::reference_wrapper<const Gui::ProvInfo>> getInfo();

	private:
		BasicText m_sub_text;
		BasicText m_title_text;
		Image m_image;

		std::string mCurrentProcessingText;
        uint32_t mCurrentError = 0;
		int m_current_step = 0;
        const Gui::ObjData &mData;
	};

	class UIProvTip : public Base,public ProvTip
	{
	public:
		explicit UIProvTip(ObjPtr obj) : Base(std::move(obj)), ProvTip(m_scr,m_obj->getData()) {}

    private:
        bool _initialize() override;

	};
}

#endif //TUYA_PLANTS_IVY_UIPROVTIP_H
