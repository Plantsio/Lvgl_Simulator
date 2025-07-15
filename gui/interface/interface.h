//
// Created by Huwensong on 2025/5/14.
//

#ifndef SIMULATOR_INTERFACE_H
#define SIMULATOR_INTERFACE_H

#include <cstdio>
#include <variant>
#include <memory>

#include "lvgl.h"

#define MALLOC_CAP_SPIRAM 0
#define MALLOC_CAP_8BIT   0
#define portMAX_DELAY     0xffffffffUL

namespace DeviceDp
{
    typedef enum
    {
        START = 0,
        PERSIST_START = START,  /* note: required field */
        //region persist value
        VALUE_START = PERSIST_START,
        plant_id,
        media_file_version,
        hardware_version,
        enable_plant_detect,
        plant_detect_sensitivity,
        enable_manual_plant_exist,
        enable_auto_brightness,
        brightness,
        enable_smart_sleep,
        enable_night_sleep,
        night_sleep_start,
        night_sleep_period,
        enable_ear,
        ear_sensitivity,
        enable_touch,
        language,
        enable_location,
        longitude,
        latitude,
        pot_color,
        temperature_unit,
        time_mode,
        plant_create_time,
        pomodoro_total_num,
        pomodoro_total_minutes,
        time_zone,
        tutorial_complete,

        adc_ref_vol,        /* ref voltage from voltage ref chip at 0Db attenuation */

        enable_vibration,
        enable_touch_vibration,

        VALUE_END,
        //endregion

        //region persist string
        STRING_START = VALUE_END,
        plant_uuid,
        STRING_END,
        //endregion

        PERSIST_END = STRING_END,        /* note: required field */
    } persist;

    typedef enum
    {
        NVS_START = PERSIST_END, /* note: required field */
        factory_pass,
        NVS_END, /* note: required field */
    } nvs;

    typedef enum
    {
        RAM_START = NVS_END, /* note: required field */
        waterlevel,
        light,
        temperature,
        humidity,
        soilPercent,
        battPercent,
        water_status,
        illumination_status,
        temperature_status,
        humidity_status,
        moisture_status,
        isPowered,
        water_manual_calibration,
        system_command,
        ip,
        touching_type,
        touch_left,
        touch_right,
        touch_plant,
        plantExist,
        evolve_status,
        pomodoro_custom_time,
        dli,
        RAM_END, /* note: required field */

        END = RAM_END,
    } ram;

    using KeyType = std::variant<persist, nvs, ram>;
}
typedef enum {
    RR_OK,
    RR_FAIL,
    RR_NO_RUN,
    RR_EXIT,
} routine_ret_t;

void *heap_caps_malloc(size_t size, char caps);

void line_set_style(lv_obj_t *line, lv_coord_t width, lv_color_t color, bool is_rounded = true);

void label_set_style(lv_obj_t *label, const lv_font_t *font, lv_color_t color = lv_color_white(),lv_text_align_t align = LV_TEXT_ALIGN_CENTER);

void bar_set_default_style(lv_obj_t *bar);

long long Millis();

class SystemStore
{
public:
    template<typename T>
    static T get(DeviceDp::KeyType index) {
        static_assert(std::is_arithmetic_v<T>|| std::is_same_v<T, bool>,"Return type must be uint32、int、float、bool");

        T value = 50;
        return value;
    }
};

namespace Sys
{
    void shutdown();
}

enum NetStatus
{
    IOT_NOT_CONNECTED,
    IOT_CONNECTED,
    IOT_ACTIVATED,
    IOT_ONLINE,
    IOT_DISCONNECTED,
    IOT_OFFLINE,
};

class IOTManager
{
public:
    //region Singleton
    static IOTManager &instance()
    {
        static std::shared_ptr<IOTManager> instance(new IOTManager());
        return *instance;
    }

    NetStatus getNetStatus(){return IOT_ONLINE;}
};


#endif //SIMULATOR_INTERFACE_H
