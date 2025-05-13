////
//// Created by Huwensong on 2023/8/11.
////
//
//#include "UILabel.h"
//
//#include <fstream>
//
//#include "Prop.h"
//#include "Njson.hpp"
//#include "SD_MMC.h"
//#include "oldfilepath.h"
//#include "fat_interface.h"
//
//namespace UI
//{
//    std::unordered_map<Lang::ui_text_t,std::vector<std::string>> UILabel::m_custom_content_index_list;
//
//    const std::array<std::string,Lang::lang_max> lang_path = {
//            "/zh",
//            "/en",
//            "/jp",
//            "/fr",
//    };
//
//    UILabel::UILabel(lv_obj_t *_parent) : m_default_label(lv_label_create(_parent))
//    {
//        lv_obj_set_style_text_color(m_default_label,lv_color_white(),0);
//    }
//
//    UILabel::UILabel()
//    {
//
//    }
//
//    void UILabel::set_label(lv_obj_t *label)
//    {
//        m_default_label = label;
//    }
//
//    void UILabel::set_default_font(Lang::lang_desc desc)
//    {
//        m_lang_desc = desc;
//        //Lang::Language::instance().lang_set_style_text_font(m_default_label,m_lang_desc);
//    }
//
//    void UILabel::set_size(lv_coord_t w, lv_coord_t h)
//    {
//        lv_obj_set_size(m_default_label,w,h);
//    }
//
//    void UILabel::set_width(lv_coord_t w)
//    {
//        lv_obj_set_width(m_default_label,w);
//    }
//
//    void UILabel::set_height(lv_coord_t h)
//    {
//        lv_obj_set_height(m_default_label,h);
//    }
//
//    void UILabel::align(lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
//    {
//        lv_obj_align(m_default_label,align,x_ofs,y_ofs);
//    }
//
//    void UILabel::align_to(const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
//    {
//        lv_obj_align_to(m_default_label,base,align,x_ofs,y_ofs);
//    }
//
//    void UILabel::set_pos(lv_coord_t x, lv_coord_t y)
//    {
//        lv_obj_set_pos(m_default_label,x,y);
//    }
//
//    bool UILabel::load_custom_config()
//    {
//        std::string custom_file_name = "/UICustomText.json";
//        std::string url = std::string(PATH_CUSTOM_TEXT) + custom_file_name;
//
//        nlohmann::json js;
//        if (SD_MMC.exists(url.c_str())) {
//            std::ifstream file(longer_path(url.c_str()));
//
//            if (file.peek() == std::ifstream::traits_type::eof())
//            {
//                log_i("custom config file is empty!");
//                file.close();
//                return false;
//            }
//            else
//            {
//                file >> js;
//                file.close();
//
//                if (js.empty())
//                {
//                    log_d("custom json empty");
//                    return false;
//                }
//
//                for (const auto &item : js.items())
//                {
//                    nlohmann::json js_text = item.value();
//
//                    std::vector<std::string> custom_content(Lang::lang_max);
//
//                    custom_content[Lang::zh] = js_text["custom_content"]["zh"].get<std::string>();
//                    custom_content[Lang::en] = js_text["custom_content"]["en"].get<std::string>();
//                    custom_content[Lang::jp] = js_text["custom_content"]["jp"].get<std::string>();
//                    custom_content[Lang::fr] = js_text["custom_content"]["fr"].get<std::string>();
//
//                    m_custom_content_index_list.insert(std::make_pair(js_text["index"].get<Lang::ui_text_t>(),custom_content));
//                }
//
//                return true;
//            }
//        }
//        else
//        {
//            return false;
//        }
//    }
//
//    void UILabel::update(Lang::ui_text_t content_id,palette_t palette,int32_t lang_option)
//    {
//        if (load_custom_config())
//        {
//            set_text(content_id,palette);
//        }
//        else
//        {
//            if (lang_option < 0)
//            {
//                set_palette_string(LANG(content_id),palette);
//            }
//            else
//            {
//                set_palette_string(LANG_OPTION(content_id,lang_option),palette);
//            }
//        }
//    }
//
//    void UILabel::set_text(Lang::ui_text_t content_id,palette_t palette)
//    {
//        auto it = m_custom_content_index_list.find(content_id);
//
//        if (it == m_custom_content_index_list.end())
//        {
//            if (is_switch_font)
//            {
//                Lang::Language::instance().lang_set_style_text_font(m_default_label,m_lang_desc);
//                is_switch_font = false;
//            }
//
//            set_palette_string(LANG(content_id),palette);
//        }
//        else  // overwrite label
//        {
//            is_switch_font = true;
//            overwrite(content_id,palette);
//        }
//    }
//
//    void UILabel::overwrite(Lang::ui_text_t content_id,palette_t palette)
//    {
//        int lang = Prop::get<int>(Prop::language);
//
//        std::string custom_content = m_custom_content_index_list.find(content_id)->second.at(lang);
//
//        std::string suffix = ".bin";
//
//        std::string font_name = std::to_string(content_id);
//
//        std::string url = std::string (PATH_CUSTOM_FONT) + "/" + font_name + suffix;
//
//        m_font_ptr = std::make_shared<Lang::Font>(url);
//
//        lv_obj_set_style_text_font(m_default_label,m_font_ptr->get_font(),0);
//
//        set_palette_string(custom_content,palette);
//    }
//
//    void UILabel::set_palette_string(std::string str,palette_t palette)
//    {
//        if (palette != palette_null)
//            lv_label_set_text(m_default_label,LANG_COLOR(palette, str).c_str());
//        else
//            lv_label_set_text(m_default_label, str.c_str());
//    }
//}
//
