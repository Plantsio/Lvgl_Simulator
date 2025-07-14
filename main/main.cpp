
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include "lvgl.h"
#include "lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#include <memory>
#include "log.h"

#include "UITutorial.h"
#include "FontTTF.h"
#include "Lottie.h"
#include "RuntimeFont.h"
#include "lv_font_runtime.h"
#include "lv_font_writer.h"
#include "UIImage.h"
#include "Image.h"

#define LV_TEST   0

void sys_init()
{
    Theme::instance().initialize();
    UI::Image::registerMjpegImageDecoder();
}

std::shared_ptr<UI::Base> ui_init()
{
    std::shared_ptr<UI::Base> ui = nullptr;

    //region UI make
    //ui = std::make_shared<UI::UITutorial>(nullptr);
    //ui = std::make_shared<UI::FontTTF>();
    //ui = std::make_shared<UI::Lottie>();
    //ui = std::make_shared<UI::RuntimeFont>();
    ui = std::make_shared<UI::UIImage>();
    //endregion

    if (ui && ui->load())
    {
        log_d("UI load success");
    }
    else
    {
        log_d("UI load failed");
    }

    return ui;
}

void font_test()
{
    lv_font_write(&lt_pro_normal,"S:..\\resources\\lt_pro_normal.cbin");

    //lv_bin_runtime_create("S:..\\resources\\ba_16.cbin");
}

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(320,240);

    /*Open a demo or an example*/
//  if (argc == 0){
//      lv_demo_widgets();
//      //  lv_example_chart_1();
//  }
//  else
//  {
//      if (!lv_demos_create(&argv[1], argc - 1))
//      {
//          lv_demos_show_help();
//          goto demo_end;
//      }
//  }

#if LV_TEST

   font_test();
#else

    sys_init();

  auto UI = ui_init();
#endif



  while(1) {

#if LV_TEST

#else
      UI->routine();
#endif

      /* Periodically call the lv_task handler.
       * It could be done in a timer interrupt or an OS task too.*/
      lv_timer_handler();
      usleep(5 * 1000);
  }

//  demo_end:
//    lv_deinit();
//    return 0;

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{
    lv_group_set_default(lv_group_create());

    lv_display_t * disp = lv_sdl_window_create(w, h);

    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_set_group(mouse, lv_group_get_default());
    lv_indev_set_display(mouse, disp);
    lv_display_set_default(disp);

    LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    lv_obj_t * cursor_obj;
    cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
    lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_display(mousewheel, disp);

    lv_indev_t * keyboard = lv_sdl_keyboard_create();
    lv_indev_set_display(keyboard, disp);
    lv_indev_set_group(keyboard, lv_group_get_default());

    return disp;
}
