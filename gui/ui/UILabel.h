////
//// Created by Huwensong on 2023/8/11.
////

#ifndef MI_PLANTS_IVY_UILABEL_H
#define MI_PLANTS_IVY_UILABEL_H

//#include "lvgl.h"
//
//#include <string>
//
//#include "ThemeInterface.h"
//#include "Lang.h"
//
//namespace UI
//{
//
//    class UILabel
//    {
//    public:
//        UILabel(lv_obj_t *_parent);
//
//        UILabel();
//
//        ~UILabel() = default;
//
//    public:
//        void set_label(lv_obj_t *label);
//
//        void set_default_font(Lang::lang_desc desc);
//
//
//
//        void set_size(lv_coord_t w, lv_coord_t h);
//
//        void set_width(lv_coord_t w );
//
//        void set_height(lv_coord_t h);
//
//        void set_pos(lv_coord_t x, lv_coord_t y);
//
//        void align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
//
//        void align_to(const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
//
//        bool load_custom_config();
//
//        void update(Lang::ui_text_t content_id,palette_t palette = palette_null,int32_t lang_option = -1);
//
//    private:
//        void set_text(Lang::ui_text_t content_id,palette_t palette);
//
//        void overwrite(Lang::ui_text_t content_id,palette_t palette);
//
//        void set_palette_string(std::string str,palette_t palette);
//
//    private:
//        lv_obj_t *m_default_label;
//
//        static std::unordered_map<Lang::ui_text_t,std::vector<std::string>> m_custom_content_index_list;
//
//        std::shared_ptr<Lang::Font> m_font_ptr;
//
//        Lang::lang_desc m_lang_desc;
//
//        bool is_switch_font = false;
//
//        std::string font_name;
//    };
//}
#endif //MI_PLANTS_IVY_UILABEL_H
