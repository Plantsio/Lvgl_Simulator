//
// Created by Huwensong on 2025/1/14.
//

#include "UIOTA.h"

#include "IOTManager.h"

#include "ThemeInterface.h"
#include "Lang.h"
#include "tuya_cutsom_ifs.h"

namespace UI
{
#ifdef __cplusplus
    extern "C" {
#endif
    extern TKL_OTA_HANDLE_S tkl_ota_handle;
#ifdef __cplusplus
    }
#endif

    UIOTA::UIOTA(ObjPtr obj):Base(std::move(obj)), mProgress(m_scr)
    {

    }

    bool UIOTA::_initialize()
    {
        mProgress.update_title(THEME_TEXT_CONTENT(Lang::ui_ota_title));
        mProgress.update_status(THEME_TEXT_CONTENT(Lang::ui_ota_status_upgrading));
        return true;
    }

    void UIOTA::routine()
    {
        switch (tkl_ota_handle.state)
        {
            case TKL_OTA_BEGIN:
                mProgress.update_status(THEME_TEXT_CONTENT(Lang::ui_ota_remote));
                mProgress.update_desc(THEME_TEXT_APPEND_COLOR(Lang::ui_evolve_not_shutdown, Theme::palette_warning));
                break;
            case TKL_OTA_IN_PROGRESS:
            {
                auto progress = (float)tkl_ota_handle.offset_image_size / (float) tkl_ota_handle.image_size * 100;
                mProgress.update_progress((uint32_t)progress);
                break;
            }
            case TKL_OTA_SUCCESS:
                prevFinish();
                mProgress.update_status(THEME_TEXT_CONTENT(Lang::ui_ota_status_success));
                break;
            default:
                break;
        }
    }

    void UIOTA::prevFinish()
    {
        //todo: 对屏幕、SD等一些设备做关机前的准备。
    }
}