//
// Created by Huwensong on 2025/7/7.
//

#include "UIImage.h"
#include "filepath.h"
#include "log.h"

namespace UI
{
    UIImage::UIImage():
    Base(nullptr),
    image_1(lv_image_create(m_scr)),
    image_2(lv_image_create(m_scr))
    {}

    bool UIImage::_initialize()
    {
        std::string file_path_1 = path({ASSETS_DIR,"cloud_1.mjpeg"});
        std::string lv_path_1 = "S:" + file_path_1;
        log_d("file path = %s",lv_path_1.c_str());
        lv_image_set_src(image_1, lv_path_1.c_str());
        lv_obj_align(image_1,LV_ALIGN_LEFT_MID,0,0);

//        std::string file_path_2 = path({ASSETS_DIR,"cloud_2.mjpeg"});
//        std::string lv_path_2 = "S:" + file_path_2;
//        log_d("file path = %s",lv_path_2.c_str());
//        lv_image_set_src(image_2, file_path_2.c_str());
//        lv_obj_align_to(image_2,image_1,LV_ALIGN_OUT_RIGHT_MID,0,0);
        return true;
    }
} // UI