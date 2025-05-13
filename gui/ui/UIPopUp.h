//
// Created by Huwensong on 2024/11/15.
//

#ifndef FIRMWARE_UIPOPUP_H
#define FIRMWARE_UIPOPUP_H

#include "UIBase.h"
#include "BasicText.h"

namespace UI
{
	class UIPopUp : public Base
	{
	public:
		explicit UIPopUp(ObjPtr obj);

	public:
		void update(const char *main, const char *sub, bool anim_on = true);

		void update_main(const char *main, bool anim_on = true);

		void update_sub(const char *sub, bool anim_on = true);

		void realign() const;

	private:
		void create_confirm_ui();

	private:
		BasicText m_text_main;
		BasicText m_text_sub;

		lv_obj_t *m_ok;
		lv_obj_t *m_arc;
	};
}




#endif //FIRMWARE_UIPOPUP_H
