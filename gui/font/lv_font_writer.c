//
// Created by Huwensong on 2025/6/17.
//

#include "lv_font_writer.h"
#include "lv_bin_defines.h"
#include <math.h>

static bool write_header(lv_fs_file_t *fp, const lv_font_t *font);
static bool write_cmaps(lv_fs_file_t *fp, lv_font_fmt_txt_dsc_t *fdsc);
static bool write_kern(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc);
static bool write_glyph_dsc(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc);
static bool write_glyph_bitmap(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc);

bool lv_font_write(const lv_font_t *font,const char *path)
{
    lv_fs_file_t file;
    lv_fs_res_t fs_res = lv_fs_open(&file, path, LV_FS_MODE_WR);
    if(fs_res != LV_FS_RES_OK) return false;

    lv_font_fmt_txt_dsc_t *fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

    if (!write_header(&file,font))
    {
        lv_fs_close(&file);
        return false;
    }


    if (!write_cmaps(&file,fdsc))
    {
        lv_fs_close(&file);
        return false;
    }

    if (!write_kern(&file,fdsc))
    {
        lv_fs_close(&file);
        return false;
    }

    if (!write_glyph_dsc(&file,fdsc))
    {
        lv_fs_close(&file);
        return false;
    }

    if (!write_glyph_bitmap(&file,fdsc))
    {
        lv_fs_close(&file);
        return false;
    }

    lv_fs_close(&file);
    return true;
}

static bool write_header(lv_fs_file_t *fp, const lv_font_t *font)
{
    if (!font)
        return false;

    lv_font_fmt_txt_dsc_t *fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

    /*write header*/
    font_header_bin_t header_bin = {0};
    uint32_t header_length = sizeof(font_header_bin_t) + sizeof(uint32_t);

    header_bin.tables_count = 5;
    header_bin.descent = (int16_t)(-font->base_line);
    header_bin.ascent = header_bin.descent + font->line_height;
    header_bin.kerning_scale = fdsc->kern_scale;
    header_bin.bits_per_pixel = fdsc->bpp;
    header_bin.compression_id = fdsc->bitmap_format;
    header_bin.subpixels_mode = font->subpx;
    header_bin.underline_position = (int16_t)font->underline_position;
    header_bin.underline_thickness = (int16_t)font->underline_thickness;

    if (!fdsc->kern_classes)
    {
        const lv_font_fmt_txt_kern_pair_t *kern_pair = (lv_font_fmt_txt_kern_pair_t *)fdsc->kern_dsc;
        header_bin.glyph_id_format = kern_pair->glyph_ids_size;
    }

    if (lv_fs_write(fp,&header_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,&header_bin, sizeof(font_header_bin_t),NULL) != LV_FS_RES_OK)
        return false;

    return true;
}

static bool write_cmaps(lv_fs_file_t *fp, lv_font_fmt_txt_dsc_t *fdsc)
{
    uint32_t cmap_num = fdsc->cmap_num;
    const lv_font_fmt_txt_cmap_t * cmaps = fdsc->cmaps;

    cmap_table_bin_t cmap_bin[fdsc->cmap_num];
    lv_memset(&cmap_bin, 0,sizeof(cmap_table_bin_t) * cmap_num);

    uint32_t sum_data_length = 0;
    for (int i = 0; i < cmap_num; ++i)
    {
        cmap_bin[i].range_start = cmaps[i].range_start;
        cmap_bin[i].range_length = cmaps[i].range_length;
        cmap_bin[i].glyph_id_start = cmaps[i].glyph_id_start;
        cmap_bin[i].format_type = cmaps[i].type;
        cmap_bin[i].data_entries_count = cmaps[i].list_length;

        if (cmaps[i].list_length > 0)
        {
            if (cmaps[i].type == LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL )
            {
                sum_data_length += cmaps[i].list_length;

                if (i >= 1)
                    cmap_bin[i].data_offset =  cmap_bin[i - 1].data_offset  +  cmap_bin[i - 1].data_entries_count;

            }
            else if (cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_FULL || cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_TINY)
            {

                sum_data_length += cmaps[i].list_length * sizeof(uint16_t);

                if (i >= 1)
                    cmap_bin[i].data_offset =  cmap_bin[i - 1].data_offset  +
                            cmap_bin[i - 1].data_entries_count * sizeof(uint16_t);
            }
        }
    }

    uint32_t cmaps_length = sizeof(uint32_t) * 2 + sizeof(cmap_table_bin_t) * cmap_num + sum_data_length;

    if (lv_fs_write(fp,&cmaps_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,&cmap_num, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,cmap_bin, sizeof(cmap_table_bin_t) * cmap_num,NULL) != LV_FS_RES_OK)
        return false;


    for (int i = 0; i < cmap_num; ++i)
    {
        switch (cmap_bin[i].format_type)
        {
            case LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL:
            {
                if (lv_fs_write(fp,cmaps[i].glyph_id_ofs_list, cmap_bin[i].data_entries_count,NULL) != LV_FS_RES_OK)
                    return false;
                break;
            }
            case LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY:
                break;
            case LV_FONT_FMT_TXT_CMAP_SPARSE_FULL:
            case LV_FONT_FMT_TXT_CMAP_SPARSE_TINY:
            {
                if (lv_fs_write(fp,cmaps[i].unicode_list, sizeof(uint16_t) * cmap_bin[i].data_entries_count,NULL) != LV_FS_RES_OK)
                    return false;

                if(cmap_bin[i].format_type == LV_FONT_FMT_TXT_CMAP_SPARSE_FULL)
                {
                    if (lv_fs_write(fp,cmaps[i].glyph_id_ofs_list, sizeof(uint16_t) * cmap_bin[i].data_entries_count,NULL) != LV_FS_RES_OK)
                        return false;
                }
                break;
            }
            default:
                LV_LOG_WARN("Unknown cmaps format type %d.", cmap_bin[i].format_type);
                return false;
        }
    }

    return true;
}

static uint32_t get_glyph_num(const lv_font_fmt_txt_dsc_t *fdsc)
{
    const lv_font_fmt_txt_cmap_t *cmap_last = &fdsc->cmaps[fdsc->cmap_num - 1];

    return cmap_last->list_length ? cmap_last->glyph_id_start + cmap_last->list_length : cmap_last->glyph_id_start + 1;
}

static bool write_kern_format0(lv_fs_file_t *fp,const lv_font_fmt_txt_dsc_t *fdsc)
{
    const lv_font_fmt_txt_kern_pair_t *kern_pair = (lv_font_fmt_txt_kern_pair_t * )fdsc->kern_dsc;

    uint32_t glyph_entries = kern_pair->pair_cnt;
    if (lv_fs_write(fp,&glyph_entries, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    uint32_t ids_size = kern_pair->glyph_ids_size == 0 ? sizeof(int8_t) * 2 * glyph_entries : sizeof(int16_t) * 2 * glyph_entries;

    if (lv_fs_write(fp,kern_pair->glyph_ids, ids_size,NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,kern_pair->values, glyph_entries,NULL) != LV_FS_RES_OK)
        return false;

    return true;
}

static bool write_kern_format3(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc,uint16_t mapping_length)
{
    const lv_font_fmt_txt_kern_classes_t *kern = (lv_font_fmt_txt_kern_classes_t *)fdsc->kern_dsc;

    uint16_t kern_class_mapping_length = mapping_length;
    uint8_t kern_table_rows = kern->left_class_cnt;
    uint8_t kern_table_cols = kern->right_class_cnt;

    lv_fs_write(fp,&kern_class_mapping_length,sizeof(uint16_t),NULL);
    lv_fs_write(fp,&kern_table_rows,sizeof(uint8_t),NULL);
    lv_fs_write(fp,&kern_table_cols,sizeof(uint8_t),NULL);

    lv_fs_write(fp,kern->left_class_mapping,kern_class_mapping_length,NULL);
    lv_fs_write(fp,kern->right_class_mapping,kern_class_mapping_length,NULL);
    lv_fs_write(fp,kern->class_pair_values,kern_table_rows * kern_table_cols * sizeof(int8_t),NULL);

    return true;
}

static bool write_kern(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc)
{
    uint32_t kern_length = 0;
    uint32_t kern_format_type = 0;
    uint32_t kern_class_mapping_length = get_glyph_num(fdsc);

    bool is_kern_class = fdsc->kern_classes;

    if (is_kern_class)
    {
        const lv_font_fmt_txt_kern_classes_t *kern = (lv_font_fmt_txt_kern_classes_t *)fdsc->kern_dsc;
        kern_length = sizeof(uint32_t) * 2 + sizeof(uint32_t) * 3 + kern_class_mapping_length * 2 + kern->left_class_cnt * kern->right_class_cnt;
        kern_format_type = 3;
    }
    else
    {
        const lv_font_fmt_txt_kern_pair_t *kern_pair = (lv_font_fmt_txt_kern_pair_t * )fdsc->kern_dsc;
        uint32_t glyph_entries = kern_pair->pair_cnt;
        uint32_t ids_size = kern_pair->glyph_ids_size == 0 ? sizeof(int8_t) * 2 * glyph_entries : sizeof(int16_t) * 2 * glyph_entries;
        kern_length = sizeof(uint32_t) * 2 + sizeof(uint32_t) + ids_size + glyph_entries;
        kern_format_type = 0;
    }

    if (lv_fs_write(fp,&kern_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,&kern_format_type, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    return  is_kern_class ? write_kern_format3(fp,fdsc,kern_class_mapping_length) : write_kern_format0(fp,fdsc);
}

static bool write_glyph_dsc(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc)
{
    const lv_font_fmt_txt_cmap_t *cmap_last = &fdsc->cmaps[fdsc->cmap_num - 1];

    uint32_t glyph_dsc_num = get_glyph_num(fdsc);
    uint32_t glyph_dsc_byte_length = sizeof(uint32_t) + sizeof(lv_font_fmt_txt_glyph_dsc_t) * glyph_dsc_num;

    if (lv_fs_write(fp,&glyph_dsc_byte_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,fdsc->glyph_dsc, sizeof(lv_font_fmt_txt_glyph_dsc_t) * glyph_dsc_num,NULL) != LV_FS_RES_OK)
        return false;

    return true;
}

static bool write_glyph_bitmap(lv_fs_file_t *fp, const lv_font_fmt_txt_dsc_t *fdsc)
{
    uint32_t glyph_dsc_num = get_glyph_num(fdsc);
    const lv_font_fmt_txt_glyph_dsc_t * glyph_dsc_last = &fdsc->glyph_dsc[glyph_dsc_num - 1];

    uint32_t glyph_bitmap_length = glyph_dsc_last->bitmap_index +
                                   (uint32_t)ceil((glyph_dsc_last->box_w * glyph_dsc_last->box_h * 1.0) / (8.0 / fdsc->bpp));

    uint32_t glyph_bitmap_byte_length = sizeof(uint32_t) + glyph_bitmap_length;

    if (lv_fs_write(fp,&glyph_bitmap_byte_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
        return false;

    if (lv_fs_write(fp,fdsc->glyph_bitmap, glyph_bitmap_length,NULL) != LV_FS_RES_OK)
        return false;

    return true;
}
