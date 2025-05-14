//
// Created by sungaoran on 2022/1/18.
//

#include "tools.h"
#include "filepath.h"

namespace UI
{
	static void decode_to_canvas_cb(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *data)
	{
		/* assume x===0 */
		int ptr = w * y;
		//memcpy(UIManager::instance().get_canvas_buf() + ptr, data, w * h * sizeof(uint16_t));
	}

	std::string current_asset;

	void anim_canvas_reset_asset()
	{
		current_asset = "";
	}

	void anim_canvas_bind_asset(lv_obj_t *canvas, std::string asset_name, custom_assts_path_t path, bool reset)
	{
		if (reset || current_asset != asset_name)
		{
			current_asset = asset_name;
			anim_canvas_bind(canvas, get_asset_path(asset_name, path).c_str());
		}
	}

	void anim_canvas_bind(lv_obj_t *canvas, const char *filepath)
	{
		/* fixme decode_video should be called with consideration of other animations that maybe playing at the moment */
//        QoiCodec &qoi = QoiCodec::instance();
//        auto handler = qoi.decode_video(filepath, &decode_to_canvas_cb);
//        if (handler->success) {
//            uint16_t w = qoi.get_current_desc().width;
//            uint16_t h = qoi.get_current_desc().height;
//            //lv_canvas_set_buffer(canvas, UIManager::instance().get_canvas_buf(), w, h, LV_COLOR_FORMAT_NATIVE);
//        } else {
//            lv_draw_label_dsc_t label_dsc;
//            lv_draw_label_dsc_init(&label_dsc);
//            label_dsc.color = lv_color_white();
//            //lv_canvas_draw_text(canvas, 0, 0, 50, &label_dsc, "not found");
//        }
	}

	void anim_canvas_update(lv_obj_t *canvas)
	{
//        if (QoiCodec::instance().decode_video_routine()) {
//            lv_obj_invalidate(canvas);
//        }
	}

	static void _anim_canvas_change_bind_cb(struct _lv_anim_t *anim)
	{
		anim_canvas_bind_asset((lv_obj_t *) anim->var, current_asset, cus_tu, true);
		anim_canvas_update((lv_obj_t *) anim->var);
		auto a = anim_create((lv_obj_t *) anim->var, anim_fade, LV_OPA_TRANSP, LV_OPA_COVER, anim->time);
		lv_anim_start(&a);
	}

	void anim_canvas_change_bind(lv_obj_t *canvas, std::string asset_name, uint32_t time, uint32_t delay)
	{
		current_asset = std::move(asset_name);
		auto a = anim_create(canvas, anim_fade, LV_OPA_COVER, LV_OPA_TRANSP, time, delay, 0, 0,
							 _anim_canvas_change_bind_cb);
		lv_anim_start(&a);
	}

	std::string get_asset_path(std::string &asset_name, custom_assts_path_t path)
	{

		std::string asset_full_name;
		switch (path)
		{
			case cus_tu:
				asset_full_name = std::string(PATH_ASSETS) + "/" + asset_name + ".mqoi";
				break;
			case cus_factory:
				asset_full_name = std::string(PATH_FACTORY_ASSETS) + "/" + asset_name + ".mqoi";
				break;
			case cus_custom:
				asset_full_name = std::string(PATH_CUSTOM_ANIM) + "/" + asset_name + ".mqoi";
				break;
		}
		return asset_full_name;
	}

	lv_anim_t
	anim_create(lv_obj_t *obj, lv_anim_exec_xcb_t exec_cb, int value_start, int value_end, uint32_t time,
				uint32_t delay,
				uint32_t playback_time, uint32_t playback_delay,
				lv_anim_ready_cb_t ready_cb, void *user_data, int repeat_cnt, lv_anim_path_cb_t path)
	{
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_var(&a, obj);
		lv_anim_set_exec_cb(&a, exec_cb);
		lv_anim_set_values(&a, value_start, value_end);
		lv_anim_set_time(&a, time);
		if (delay)
		{
			lv_anim_set_delay(&a, delay);
		}
		if (playback_time)
		{
			lv_anim_set_playback_time(&a, playback_time);
			lv_anim_set_playback_delay(&a, playback_delay);
		}
		lv_anim_set_path_cb(&a, path);
		lv_anim_set_ready_cb(&a, ready_cb);
		lv_anim_set_user_data(&a, user_data);
		lv_anim_set_repeat_count(&a, repeat_cnt);
		return a;
	}

	void anim_fade(void *var, int32_t value)
	{
		lv_obj_set_style_opa((lv_obj_t *) var, value, 0);
	}

	void anim_move_ver(void *var, int32_t value)
	{
		lv_obj_set_y((lv_obj_t *) var, value);
	}

	void anim_move_hor(void *var, int32_t value)
	{
		lv_obj_set_x((lv_obj_t *) var, value);
	}

	void anim_set_align_top(void *var, int32_t value)
	{
		lv_obj_align((lv_obj_t *) var, LV_ALIGN_TOP_MID, 0, value);
	}

	void bar_set_default_style(lv_obj_t *bar) {
		lv_obj_remove_style_all(bar);  /*To have a clean start*/
	}

	void line_set_style(lv_obj_t *line, lv_coord_t width, lv_color_t color, bool is_rounded) {
		lv_obj_set_style_line_width(line, width, 0);
		lv_obj_set_style_line_color(line, color, 0);
		lv_obj_set_style_line_rounded(line, is_rounded, 0);
	}

	void ui_set_action(bool apply) {
		//LvglDriver &lvgl = LvglDriver::instance();
		//std::lock_guard<std::recursive_timed_mutex> _lock(IvyAnim::instance().get_mutex());
//    std::lock_guard<std::recursive_mutex> lock(lvgl.get_mutex());
//		if (lvgl.is_ui(UI_ACTION)) {
//			auto ui = lvgl.get_current_ui<UI::UIActionBox>();
//			if (apply) {
//				ui->set_action(ACTION_APPLY);
//			} else {
//				ui->set_action(ACTION_CANCEL);
//			}
//		}
	}

	void label_set_style(lv_obj_t *label, const lv_font_t *font, lv_color_t color, lv_text_align_t align) {
		lv_obj_set_style_text_font(label, font, 0);
		lv_obj_set_style_text_color(label, color, 0);
		lv_obj_set_style_text_align(label, align, 0);
	}

	void show_temp_text(const char *text, uint32_t period) {
//		auto ui = build_ui<TempText>(Gui::GUI_TEMP_TEXT);
//		LvglDriver::instance().load(ui, false, true, true, [&]() {
//			ui->set_text(text);
//			ui->set_period(period);
//		});
	}

	void ui_confirm(const char *title, const char *desc, const std::function<void()> &apply_cb,
					const std::function<void()> &cancel_cb, bool *processing_flag, int timeout) {
		if (!processing_flag || !*processing_flag) {
			if (processing_flag) {
				*processing_flag = true;
			}
			//std::lock_guard<std::recursive_timed_mutex> _lock(IvyAnim::instance().get_mutex());
//			auto ui = UI::build_ui<UI::UIActionBox>(Gui::GUI_ACTION);
//			LvglDriver::instance().load(ui, true, true, true, [&]() {
//				ui->update(title, desc, LANG(Lang::ui_apply_desc), LANG(Lang::ui_cancel_desc), apply_cb, [cancel_cb]() {
//					if (cancel_cb) {
//						cancel_cb();
//					}
//					LvglDriver &lvgl = LvglDriver::instance();
//					lvgl.set_locked(false);
//					lvgl.route_to_default();
//				}, [processing_flag]() {
//					if (processing_flag) {
//						*processing_flag = false;
//					}
//				}, timeout);
//			}, true);
		}
	}
}