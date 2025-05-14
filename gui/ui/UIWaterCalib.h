//
// Created by Huwensong on 2024/11/15.
//

#ifndef FIRMWARE_UIWATERCALIB_H
#define FIRMWARE_UIWATERCALIB_H

#include "UIBase.h"
#include "TimerStepBase.h"
#include "BasicText.h"
//#include "Sense.h"
#include "Indicator.h"

namespace UI
{
//	class UIWaterCalib : public Base
//	{
//	public:
//		explicit UIWaterCalib(ObjPtr obj);
//
//		~UIWaterCalib();
//
//	public:
//		void next() override;
//
//		void update_progress(int progress, int anim_t = 0);
//
//		void reset_progress();
//
//	private:
//		void wait_routine();
//
//		static void start_water_routine();
//
//		static bool water_routine_stopped();
//
//		void ask_add_water(int total);
//
//		bool calib_routine(int total_level);
//
//		void right_input_cb(InputData data) override;
//
//        bool _deInitialize() override;
//
//	private:
//		BasicText m_step_label;
//		BasicText m_value_label;
//		BasicText m_desc_label;
//		lv_obj_t *m_progress_bar;
//		Indicator m_indicator;
//		int confirm_t = 0;
//
////		Sense &sense = Sense::instance();
//		bool m_right_touched = false;
//	};
}




#endif //FIRMWARE_UIWATERCALIB_H
