//
// Created by sungaoran on 2021/11/16.
//

#ifndef PLANTSIO_IVY_UIPROGRESS_H
#define PLANTSIO_IVY_UIPROGRESS_H

#include <string>
#include "lvgl.h"

namespace UI
{
	class Progress
	{
	public:
		explicit Progress(lv_obj_t *_parent);

		void update(const std::string &title, const std::string &status, uint8_t progress);

		void update(const std::string &status, uint8_t progress);

		void update_title(const std::string &value);

		void update_status(const std::string &value);

		void update_progress(uint8_t progress);

		void update_desc(const std::string &desc);

	private:
		lv_obj_t *m_bar;
		lv_obj_t *m_title_label;
		lv_obj_t *m_status_label;
		lv_obj_t *m_desc_label;
	};
}

#endif //PLANTSIO_IVY_UIPROGRESS_H
