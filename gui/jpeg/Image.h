//
// Created by Huwensong on 2024/11/11.
//

#ifndef FIRMWARE_IMAGE_H
#define FIRMWARE_IMAGE_H

#include <string>

#include "lvgl.h"
#include "JPEGDEC.h"
#include "Initializable.h"
#include "CustomContainer.h"

namespace UI
{
    class Image
    {
    public:
        static void registerMjpegImageDecoder();

        static void unregisterMjpegImageDecoder();

    private:
        struct MjpegDecoderParams
        {
            uint8_t *mImageFrameCache = nullptr;
            uint16_t *usUnalignedPixels = nullptr;
            int16_t *sUnalignedMCUs = nullptr;
            uint8_t *ucFileBuff = nullptr;
            JPEGDEC *mImageDecoder = nullptr;
        };

    private:
        static lv_result_t decoderInfo(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc, lv_image_header_t *header);

        static lv_result_t decoderOpen(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);

        static void decoderClose(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc);

        static void _cleanUp(MjpegDecoderParams * info);

        static int _drawCB(JPEGDRAW *draw);
	};
}

#endif //FIRMWARE_IMAGE_H
