//
// Created by Huwensong on 2024/11/13.
//

#include "Theme.h"
#include <fstream>
#include <dirent.h>
#include <memory>

#include "log.h"
#include "filepath.h"

#define LANGUAGE_FILE_PATH     "/theme/language/language.json"
#define FONT_FILE_PATH         "/theme/font/"
#define TEXT_FILE_PATH         "/theme/text/UIText.json"

extern std::string defaultEnText;

Theme &Theme::instance() {
    static std::shared_ptr<Theme> instance(new Theme());
    return *instance;
}

void Theme::setLanguage(uint32_t language) {
    m_current_language = language;
    saveLanguage(language);
}

void Theme::setFontSize(lv_obj_t *obj, int32_t size) {
    int closest = 0;
    int min_diff = 120;

    if (m_font_map.empty())
        return;

    for (auto item: m_font_map) {
        int diff = std::abs(size - item.first);
        if (diff < min_diff) {
            min_diff = diff;
            closest = item.first;
        }
    }

    lv_obj_set_style_text_font(obj, m_font_map.find(closest)->second, 0);
}

std::string Theme::getText(int32_t index) {
    if (m_text_map.empty()) {
        log_e("text map is empty");
        return {};
    }

    auto it = m_text_map.find(index);
    if (it == m_text_map.end()) {
        log_d("text map is not find");
        return {};
    }

    return it->second;
}

std::string Theme::getText(int32_t index, int32_t option) {
    try {
        nlohmann::json js = nlohmann::json::parse(getText(index));
        for (auto &opt: js) {
            if (opt.is_array() && opt[0] == option) {
                return opt[1];
            }
        }
        return js[0];
    } catch (...) {
        return "";
    }
}

std::string Theme::getText(int32_t index, palette_t palette) {
    return getPaletteText(getText(index), palette);
}

lv_color_t Theme::getPaletteRGB(palette_t palette) {
    static const lv_color_t colors[] = {
            LV_COLOR_MAKE(52, 192, 0), LV_COLOR_MAKE(245, 203, 38), LV_COLOR_MAKE(255, 153, 102),
            LV_COLOR_MAKE(219, 0, 0)
    };
    return colors[palette];
}

std::string Theme::getPaletteHex(palette_t palette) {
    static const std::string colors[] = {
            "34c000", "f5cb26", "ff9966", "db0000"
    };
    return colors[palette];
}

std::string Theme::getPaletteText(std::string content, palette_t palette) {
    //#adlsa#
    //#00ff00 adlsaj#

    std::string init_str = content;
    std::string color_limit = "#";
    size_t first_pos = init_str.find_first_of(color_limit);
    if (first_pos == std::string::npos) {
        std::string ret = "num in str not found";
        return ret;
    }
    init_str.insert(first_pos + 1, (getPaletteHex(palette) + " "));

    return init_str;
}

bool Theme::_initialize() {
    loadLanguage();

    loadFont();

    loadText();

    return true;
}

bool Theme::loadLanguage() {
    try{
        nlohmann::json language_json;
        if (loadJson(language_json, path({REPO_DIR, LANGUAGE_FILE_PATH}))) {
            m_current_language = language_json["current"].get<uint32_t>();

            log_d("current = %d", m_current_language);

            m_support_language_list = language_json["supported"].get<std::vector<std::string>>();

            log_d("supported language = %s", m_support_language_list.at(0).c_str());
        } else {
            m_current_language = 0;                  //default: en
            m_support_language_list.clear();
            m_support_language_list.shrink_to_fit();
        }
        return true;
    }
    catch (std::exception &e)
    {
        log_e("%s",e.what());
        return false;
    }
}

void Theme::saveLanguage(uint32_t language) {
    nlohmann::json language_json;
    loadJson(language_json, path({REPO_DIR, LANGUAGE_FILE_PATH}));
    language_json["current"] = language;
    saveJson(language_json, path({REPO_DIR, LANGUAGE_FILE_PATH}));
}

bool Theme::loadText() {
    try{
        nlohmann::json text_json;
        if (loadJson(text_json, path({REPO_DIR, TEXT_FILE_PATH}))) {
            for (const auto &it: text_json.items()) {
                nlohmann::json js_text = it.value();
                if (m_support_language_list.empty())
                    m_text_map.insert(std::make_pair(js_text["index"].get<int32_t>(),
                                                     js_text["content"]["en"].get<std::string>()));
                else
                    m_text_map.insert(std::make_pair(js_text["index"].get<int32_t>(),
                                                     js_text["content"][m_support_language_list.at(
                                                             m_current_language)].get<std::string>()));
            }
        } else    //load default text
        {
            text_json = nlohmann::json::parse(defaultEnText);
            for (const auto &it: text_json.items()) {
                nlohmann::json js_text = it.value();
                m_text_map.insert(std::make_pair(js_text["index"].get<uint32_t>(),
                                                 js_text["content"]["en"].get<std::string>()));
            }
        }
        return true;
    }
    catch (std::exception &e)
    {
        log_e("%s",e.what());
        return false;
    }

}

void Theme::loadFont() {
    std::string font_dir;

    if (m_support_language_list.empty()) {
        font_dir = path({REPO_DIR, FONT_FILE_PATH, "en"});
    } else {
        font_dir = path({REPO_DIR, FONT_FILE_PATH, m_support_language_list.at(m_current_language)});
    }

    std::string font_path = font_dir;

    DIR *dir = opendir(font_path.c_str());

    if (dir != nullptr) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string font_name = entry->d_name;

            if (font_name == "." || font_name == "..") {
                continue;
            }

            int32_t front_size = getFontSize(font_name);

            auto font_full_path = std::string("S:/")+ font_dir.substr(MOUNT_NAME.length()) + "/" + font_name;

            lv_font_t *font = lv_font_load(font_full_path.c_str());
            m_font_map.insert(std::make_pair(front_size, font));
        }
        closedir(dir);
    } else {
        std::vector fontSizeList = {16, 30, 40, 120};

        for (auto &size: fontSizeList) {
            m_font_map.insert(std::make_pair(size, getDefaultFont(size)));
        }
        log_d("Theme: Failed to open dir,load default font!");
    }
}

// font name eg: en_font_16
int32_t Theme::getFontSize(const std::string &filename) {
    size_t start = filename.find_last_of('_');
    return std::stoi(filename.substr(start + 1));
}

const lv_font_t *Theme::getDefaultFont(int32_t size) {
    const lv_font_t *font = nullptr;
    switch (size) {
        case 16:
            font = &ba_16;
            break;
        case 30:
            font = &ba_30;
            break;
        case 40:
            font = &ba_40;
            break;
        case 120:
            font = &ba_120;
            break;
        default:
            break;
    }
    return font;
}

bool Theme::loadJson(nlohmann::json &json, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    file >> json;

    file.close();

    return true;
}

bool Theme::saveJson(const nlohmann::json &json, const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open())
        return false;

    file << json;

    file.close();

    return true;
}