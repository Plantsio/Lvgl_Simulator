//
// Created by Huwensong on 2024/11/15.
//

#ifndef FIRMWARE_UIPLANTSELECT_H
#define FIRMWARE_UIPLANTSELECT_H

#include "UIBase.h"
#include "BasicText.h"
#include "Indicator.h"

namespace UI
{
	class UIPlantSelect : public Base
	{
	public:
		explicit UIPlantSelect(ObjPtr obj);

	public:
		void routine() override;

    private:
        bool _initialize() override;

        bool _handleInput(InputEvtType &&input) override;

	private:
		BasicText m_top_text;
		BasicText m_middle_text;
		BasicText m_bottom_text;
		Indicator m_indicator;

		bool m_right_touched = false;
	};
}

#endif //FIRMWARE_UIPLANTSELECT_H
