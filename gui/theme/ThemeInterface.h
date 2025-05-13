//
// Created by Huwensong on 2024/11/28.
//

#ifndef FIRMWARE_THEMEINTERFACE_H
#define FIRMWARE_THEMEINTERFACE_H

#include "Theme.h"

template<typename T>
std::string toString(T value) {
	return std::to_string(value);
}

template<>
std::string toString<std::string>(std::string value);


/**
 * format a string by replacing {} with values
 * @param str
 * @param values
 * @return
 */
template<typename T>
std::string format_string(const std::string &str, std::vector<T> values) {
	try {
		std::string ret = str;
		std::string::size_type n = 0;
		std::string placeholder = "{}";
		int replace_index = 0;
		while ((n = ret.find(placeholder, n)) != std::string::npos) {
			std::string replace_str;
			if (replace_index < values.size()) {
				replace_str = toString<T>(values[replace_index++]);
			} else {
				replace_str = " ";
			}
			ret.replace(n, placeholder.size(), replace_str);
			n += replace_str.size();
		}
		return ret;
	} catch (...) {
		return str;
	}
}


template<typename T>
std::string THEME_FORMAT_TEXT(int32_t index, std::vector<T> values) {
	return format_string<T>(Theme::instance().getText(index), values);
}

inline void THEME_SET_LANGUAGE(uint32_t size)
{
	Theme::instance().setLanguage(size);
}

inline void THEME_SET_FONT_SIZE(lv_obj_t *obj,uint32_t size)
{
	Theme::instance().setFontSize(obj,size);
}

inline std::string THEME_TEXT_CONTENT(int32_t index)
{
	return Theme::instance().getText(index);
}

inline std::string THEME_TEXT_OPTION(int32_t index, int32_t option)
{
	return Theme::instance().getText(index,option);
}

inline std::string THEME_TEXT_IN_COLOR(int32_t index, Theme::palette_t palette)
{
	return Theme::instance().getText(index,palette);
}

inline std::string THEME_TEXT_APPEND_COLOR(int32_t index, Theme::palette_t palette)
{
	return std::string("#") + Theme::getPaletteHex(palette) + " " + THEME_TEXT_CONTENT(index) + "#";
}

#endif //FIRMWARE_THEMEINTERFACE_H
