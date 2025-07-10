//
// Created by Huwensong on 2024/11/11.
//

#include "Image.h"
#include "misc/lv_fs_private.h"
#include "draw/lv_image_decoder_private.h"
#include "core/lv_global.h"
#include "log.h"

#define image_cache_draw_buf_handlers &(LV_GLOBAL_DEFAULT()->image_cache_draw_buf_handlers)

namespace UI
{
	void Image::registerMjpegImageDecoder()
	{
        lv_image_decoder_t *decoder = lv_image_decoder_create();
        lv_image_decoder_set_info_cb(decoder, decoderInfo);
        lv_image_decoder_set_open_cb(decoder, decoderOpen);
        lv_image_decoder_set_close_cb(decoder, decoderClose);
	}

    void Image::unregisterMjpegImageDecoder()
    {
        lv_image_decoder_t *dec = NULL;
        while((dec = lv_image_decoder_get_next(dec)) != NULL) {
            if(dec->info_cb == decoderInfo) {
                lv_image_decoder_delete(dec);
                break;
            }
        }
    }

    lv_result_t Image::decoderInfo(lv_image_decoder_t *decoder, lv_image_decoder_dsc_t *dsc, lv_image_header_t *header)
    {
        lv_image_src_t src_type = dsc->src_type;

        if (src_type != LV_IMAGE_SRC_FILE)
            return LV_RESULT_INVALID;

        const char *img_full_name = (const char *)dsc->src;
        log_d("img_full_name = %s",img_full_name);
        if (lv_strcmp(lv_fs_get_ext(img_full_name),"mjpeg"))
            return LV_RESULT_INVALID;

        uint32_t img_frame_size = 0;
        uint32_t rn = 0;
        uint32_t pos = 0;
        lv_fs_read(&dsc->file, &img_frame_size, sizeof(uint32_t), &rn);
        log_d("frame size = %d",img_frame_size);
        if (img_frame_size == 0 || rn != sizeof(uint32_t))
            return LV_RESULT_INVALID;

        auto params = (MjpegDecoderParams *)malloc(sizeof(MjpegDecoderParams));
        dsc->user_data = params;

        params->ucFileBuff = (uint8_t *) _aligned_malloc(JPEG_FILE_BUF_SIZE,8);
        params->usUnalignedPixels = (uint16_t *) _aligned_malloc((MAX_BUFFERED_PIXELS + 16) * sizeof(uint16_t),16);

        const int mcuBufferSize = (8 + (DCTSIZE * MAX_MCU_COUNT)) * sizeof(int16_t);
        params->sUnalignedMCUs = (int16_t *) _aligned_malloc(mcuBufferSize + 16,16);

        params->mImageFrameCache = (uint8_t *)heap_caps_malloc(img_frame_size,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

        params->mImageDecoder = new JPEGDEC;

        if (!params->ucFileBuff || !params->usUnalignedPixels || !params->sUnalignedMCUs || !params->mImageFrameCache)
        {
            _cleanUp(params);
            return LV_RESULT_INVALID;
        }

        log_d("read all frame...");
        lv_fs_read(&dsc->file, params->mImageFrameCache, img_frame_size, &rn);
        if (rn != img_frame_size)
            return LV_RESULT_INVALID;

        params->mImageDecoder->openRAM(params->mImageFrameCache,img_frame_size, _drawCB,params->usUnalignedPixels,params->sUnalignedMCUs,params->ucFileBuff);

        uint32_t img_width  = params->mImageDecoder->getWidth();
        uint32_t img_height = params->mImageDecoder->getHeight();

        lv_draw_buf_t *decoded = nullptr;
        decoded = lv_draw_buf_create_ex(image_cache_draw_buf_handlers, img_width, img_height, LV_COLOR_FORMAT_NATIVE, LV_STRIDE_AUTO);
        if (!decoded)
            return LV_RESULT_INVALID;

        dsc->decoded = decoded;

        log_d("img_width = %d, img_height = %d",img_width,img_height);

        header->w  = img_width;
        header->h  = img_height;
        header->cf = LV_COLOR_FORMAT_NATIVE;
        header->magic = LV_IMAGE_HEADER_MAGIC;

        log_d("decoder info over");

        return LV_RESULT_OK;
    }

    lv_result_t Image::decoderOpen(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
    {
        lv_image_src_t src_type = dsc->src_type;
        if (src_type != LV_IMAGE_SRC_FILE)
            return LV_RESULT_INVALID;

        auto decoderParams = static_cast<MjpegDecoderParams *>(dsc->user_data);
        if (!decoderParams)
            return LV_RESULT_INVALID;

        decoderParams->mImageDecoder->setUserPointer((lv_draw_buf_t *)dsc->decoded);
        decoderParams->mImageDecoder->decode(0,0,0);
        decoderParams->mImageDecoder->close();

        log_d("decoder open over");
        
        return LV_RESULT_OK;
    }

    void Image::decoderClose(lv_image_decoder_t * decoder, lv_image_decoder_dsc_t * dsc)
    {
        auto decoderParams = static_cast<MjpegDecoderParams *>(dsc->user_data);
        if (decoderParams)
            _cleanUp(decoderParams);

        if(dsc->decoded)
            lv_draw_buf_destroy((lv_draw_buf_t *)dsc->decoded);

        log_d("decoder close");
    }

    void Image::_cleanUp(MjpegDecoderParams *info)
    {
        if (info)
        {
            if (info->ucFileBuff) {
                _aligned_free(info->ucFileBuff);
                info->ucFileBuff = nullptr;
                log_d("11111111111");
            }
            if (info->usUnalignedPixels) {
                _aligned_free(info->usUnalignedPixels);
                info->usUnalignedPixels = nullptr;
                log_d("22222222222");
            }
            if (info->sUnalignedMCUs) {
                _aligned_free(info->sUnalignedMCUs);
                info->sUnalignedMCUs = nullptr;
                log_d("33333333333");
            }
            if (info->mImageFrameCache)
            {
                free(info->mImageFrameCache);
                info->mImageFrameCache = nullptr;
                log_d("4444444444");
            }
            if (info->mImageDecoder)
            {
                delete info->mImageDecoder;
                info->mImageDecoder = nullptr;
                log_d("55555555555");
            }

            free(info);
            log_d("666666666");
        }
    }

    int Image::_drawCB(JPEGDRAW *draw)
    {
        auto drawBuff = (lv_draw_buf_t *)draw->pUser;

        auto frame_date = (uint16_t *)drawBuff->data;
        log_d("w = %d,stride = %d",drawBuff->header.w,drawBuff->header.stride);
        log_d("draw width = %d,draw height = %d",draw->iWidth,draw->iHeight);
        for (int i = 0; i < draw->iHeight;i ++)
        {
            memcpy((drawBuff->data + (drawBuff->header.w * (i + draw->y) + draw->x) * sizeof(uint16_t)),(draw->pPixels + draw->iWidth * i),draw->iWidth * sizeof(uint16_t));
        }
        return true;
    }
}

