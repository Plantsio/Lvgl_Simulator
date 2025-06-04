//
// Created by Huwensong on 2025/5/30.
//

#include "Lottie.h"
#include "filepath.h"
#include "log.h"

namespace UI
{
    Lottie::Lottie():
    Base(nullptr),
    lottie(lv_lottie_create(m_scr))
    {}

    bool Lottie::_initialize()
    {
//        FILE *f = fopen(path({REPO_DIR,"lottie","approve.json"}).c_str(),"r");
//        if (!f)
//            log_d("??????????");
//        else
//            log_d("vvvvvvvv");
        lv_lottie_set_src_file(lottie, path({REPO_DIR,"lottie","think.json"}).c_str());

#if LV_DRAW_BUF_ALIGN == 4 && LV_DRAW_BUF_STRIDE_ALIGN == 1
        /*If there are no special requirements, just declare a buffer
          x4 because the Lottie is rendered in ARGB8888 format*/
        static uint8_t buf[128 * 128 * 4];
        log_d("buffer == %p",&buf);
        lv_lottie_set_buffer(lottie, 128, 128, buf);
#else
        /*For GPUs and special alignment/strid setting use a draw_buf instead*/
    LV_DRAW_BUF_DEFINE(draw_buf, 64, 64, LV_COLOR_FORMAT_ARGB8888);
    lv_lottie_set_draw_buf(lottie, &draw_buf);
#endif

        lv_obj_center(lottie);



        return true;
    }
}