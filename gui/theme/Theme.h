//
// Created by Huwensong on 2024/11/13.
//

#ifndef FIRMWARE_THEME_H
#define FIRMWARE_THEME_H

#include <string>
#include <vector>
#include <unordered_map>
#include "CustomContainer.h"

#include "lvgl.h"
#include "Njson.hpp"
#include "Initializable.h"
/* *
 * language、text、font manager
 * */
class Theme : public Initializable
{
public:
	typedef enum
	{
		palette_success,
		palette_notice,
		palette_warning,
		palette_failure,
	} palette_t;

public:
	static Theme &instance();

public:
	void setLanguage(uint32_t language);

	void setFontSize(lv_obj_t *obj,int32_t size);

	std::string getText(int32_t index);

	std::string getText(int32_t index,int32_t option);

	std::string getText(int32_t index,palette_t palette);

public:
	static lv_color_t getPaletteRGB(palette_t palette);

	static std::string getPaletteHex(palette_t palette);

	static std::string getPaletteText(std::string content,palette_t palette);

private:
	bool _initialize() override;

	bool loadLanguage();

	void saveLanguage(uint32_t language);

	bool loadText();

	void loadFont();

private:
	static int32_t getFontSize(const std::string &filename);

    static const lv_font_t *getDefaultFont(int32_t size);

	static bool loadJson(nlohmann::json &json,const std::string &filename);

	static bool saveJson(const nlohmann::json &json,const std::string &filename);

private:
	uint32_t m_current_language = 0;

	std::vector<std::string> m_support_language_list;

	std::unordered_map<int32_t,const lv_font_t*> m_font_map;

	CustomUnorederMap<int32_t ,std::string> m_text_map;


};


#endif //FIRMWARE_THEME_H
