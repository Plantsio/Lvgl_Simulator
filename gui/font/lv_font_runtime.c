//
// Created by Huwensong on 2025/6/11.
//

#include "lv_font_runtime.h"
#include "stdlib/lv_string.h"
#include "log.h"

typedef struct font_header_bin {
    uint16_t tables_count;
    uint16_t ascent;
    int16_t descent;
    uint16_t kerning_scale;
    uint8_t bits_per_pixel;
    uint8_t compression_id;
    uint8_t subpixels_mode;
    uint8_t glyph_id_format;
    int16_t underline_position;
    uint16_t underline_thickness;
} font_header_bin_t;

typedef struct cmap_table_bin {
    uint32_t data_offset;
    uint32_t range_start;
    uint16_t range_length;
    uint16_t glyph_id_start;
    uint16_t data_entries_count;
    uint8_t format_type;
    uint8_t padding;
} cmap_table_bin_t;


typedef struct {
    uint32_t gid_left;
    uint32_t gid_right;
} kern_pair_ref_t;

typedef struct {
    lv_fs_file_t *fp;
    uint32_t glyph_dsc_offset;
    uint32_t glyph_bitmap_offset;
}glyph_info;

void write_bin()
{
    lv_fs_file_t file,*fp;
    fp = &file;
    lv_fs_res_t fs_res = lv_fs_open(fp, "S:..\\resources\\cbin_16.cbin", LV_FS_MODE_WR);
    if(fs_res != LV_FS_RES_OK) return;

    const lv_font_t *font = &cbin_16;
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

    lv_fs_write(fp,&header_length, sizeof(uint32_t),NULL);
    lv_fs_write(fp,&header_bin, sizeof(font_header_bin_t),NULL);

    /*write cmap*/
    uint32_t cmap_num = fdsc->cmap_num;
    cmap_table_bin_t cmap_bin[fdsc->cmap_num];
    lv_memset(&cmap_bin, 0,sizeof(cmap_table_bin_t) * cmap_num);

    const lv_font_fmt_txt_cmap_t * cmaps = fdsc->cmaps;

    for (int i = 0; i < cmap_num;i ++)
    {
        cmap_bin[i].range_start = cmaps[i].range_start;
        cmap_bin[i].range_length = cmaps[i].range_length;
        cmap_bin[i].glyph_id_start = cmaps[i].glyph_id_start;
        cmap_bin[i].format_type = cmaps[i].type;
    }

    uint32_t cmap_length = sizeof(cmap_table_bin_t) * cmap_num + sizeof(uint32_t) * 2;
    lv_fs_write(fp,&cmap_length, sizeof(uint32_t),NULL);
    lv_fs_write(fp,&cmap_num, sizeof(uint32_t),NULL);
    lv_fs_write(fp,cmap_bin, sizeof(cmap_table_bin_t) * cmap_num,NULL);

    /*write kern*/
    lv_font_fmt_txt_kern_classes_t *kern = (lv_font_fmt_txt_kern_classes_t *)fdsc->kern_dsc;

    uint8_t kern_format_type = 3;
    int32_t padding = 0;

    uint16_t kern_class_mapping_length = 97;
    uint8_t kern_table_rows = kern->left_class_cnt;
    uint8_t kern_table_cols = kern->right_class_cnt;

    uint32_t kern_length = kern_class_mapping_length * 2 + kern_table_rows * kern_table_cols + sizeof(int32_t) * 3;

    lv_fs_write(fp,&kern_length, sizeof(uint32_t),NULL);

    lv_fs_write(fp,&kern_format_type, sizeof(uint8_t),NULL);
    lv_fs_write(fp,&padding,sizeof(uint8_t) * 3,NULL);

    lv_fs_write(fp,&kern_class_mapping_length,sizeof(uint16_t),NULL);
    lv_fs_write(fp,&kern_table_rows,sizeof(uint8_t),NULL);
    lv_fs_write(fp,&kern_table_cols,sizeof(uint8_t),NULL);

    lv_fs_write(fp,kern->left_class_mapping,kern_class_mapping_length,NULL);
    lv_fs_write(fp,kern->right_class_mapping,kern_class_mapping_length,NULL);
    lv_fs_write(fp,kern->class_pair_values,kern_table_rows * kern_table_cols * sizeof(int8_t),NULL);

    /*write glyph desc*/
    uint32_t glyph_dsc_num = 97;
    uint32_t glyph_dsc_length = sizeof(lv_font_fmt_txt_glyph_dsc_t) * glyph_dsc_num + sizeof(uint32_t);
    lv_font_fmt_txt_glyph_dsc_t *gdsc = (lv_font_fmt_txt_glyph_dsc_t *)fdsc->glyph_dsc;

    lv_fs_write(fp,&glyph_dsc_length, sizeof(uint32_t),NULL);
    lv_fs_write(fp,gdsc, sizeof(lv_font_fmt_txt_glyph_dsc_t) * glyph_dsc_num,NULL);

    /*write glyph bitmap*/
    uint32_t glyph_bitmap_length = 3542;
    const uint8_t *bitmap = fdsc->glyph_bitmap;

    lv_fs_write(fp,&glyph_bitmap_length, sizeof(uint32_t),NULL);
    lv_fs_write(fp,bitmap, glyph_bitmap_length,NULL);

    lv_fs_close(fp);

}

static bool lvgl_load_font(lv_fs_file_t * fp, lv_font_t * font);

lv_font_t *lv_bin_runtime_create(const char* path)
{
    LV_ASSERT_NULL(path);

    static lv_fs_file_t file;
    lv_fs_res_t fs_res = lv_fs_open(&file, path, LV_FS_MODE_RD);
    if(fs_res != LV_FS_RES_OK) return NULL;

    lv_font_t * font = lv_malloc_zeroed(sizeof(lv_font_t));
    LV_ASSERT_MALLOC(font);

    if(!lvgl_load_font(&file, font)) {
        LV_LOG_WARN("Error loading font file: %s", path);
        /*
        * When `lvgl_load_font` fails it can leak some pointers.
        * All non-null pointers can be assumed as allocated and
        * `lv_binfont_destroy` should free them correctly.
        */
        lv_binfont_destroy(font);
        font = NULL;
    }

    //lv_fs_close(&file);

    return font;
}

void lv_bin_runtime_destroy(lv_font_t *font)
{

}

static int32_t read_label_length(lv_fs_file_t * fp, uint32_t start)
{
    lv_fs_seek(fp, start, LV_FS_SEEK_SET);

    int32_t length;

    if(lv_fs_read(fp, &length, 4, NULL) != LV_FS_RES_OK) {
        return -1;
    }

    return length;
}

static bool load_cmaps_tables(lv_fs_file_t * fp, lv_font_fmt_txt_dsc_t * font_dsc,
                              uint32_t cmaps_start, cmap_table_bin_t * cmap_table)
{
    if(lv_fs_read(fp, cmap_table, font_dsc->cmap_num * sizeof(cmap_table_bin_t), NULL) != LV_FS_RES_OK) {
        return false;
    }

    for(unsigned int i = 0; i < font_dsc->cmap_num; ++i) {
        lv_fs_res_t res = lv_fs_seek(fp, cmaps_start + cmap_table[i].data_offset, LV_FS_SEEK_SET);
        if(res != LV_FS_RES_OK) {
            return false;
        }

        lv_font_fmt_txt_cmap_t * cmap = (lv_font_fmt_txt_cmap_t *) & (font_dsc->cmaps[i]);

        cmap->range_start = cmap_table[i].range_start;
        cmap->range_length = cmap_table[i].range_length;
        cmap->glyph_id_start = cmap_table[i].glyph_id_start;
        cmap->type = cmap_table[i].format_type;

        switch(cmap_table[i].format_type) {
            case LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL: {
                uint8_t ids_size = (uint8_t)(sizeof(uint8_t) * cmap_table[i].data_entries_count);
                uint8_t * glyph_id_ofs_list = lv_malloc(ids_size);

                cmap->glyph_id_ofs_list = glyph_id_ofs_list;

                if(lv_fs_read(fp, glyph_id_ofs_list, ids_size, NULL) != LV_FS_RES_OK) {
                    return false;
                }

                cmap->list_length = cmap->range_length;
                break;
            }
            case LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY:
                break;
            case LV_FONT_FMT_TXT_CMAP_SPARSE_FULL:
            case LV_FONT_FMT_TXT_CMAP_SPARSE_TINY: {
                uint32_t list_size = sizeof(uint16_t) * cmap_table[i].data_entries_count;
                uint16_t * unicode_list = (uint16_t *)lv_malloc(list_size);

                cmap->unicode_list = unicode_list;
                cmap->list_length = cmap_table[i].data_entries_count;

                if(lv_fs_read(fp, unicode_list, list_size, NULL) != LV_FS_RES_OK) {
                    return false;
                }

                if(cmap_table[i].format_type == LV_FONT_FMT_TXT_CMAP_SPARSE_FULL) {
                    uint16_t * buf = lv_malloc(sizeof(uint16_t) * cmap->list_length);

                    cmap->glyph_id_ofs_list = buf;

                    if(lv_fs_read(fp, buf, sizeof(uint16_t) * cmap->list_length, NULL) != LV_FS_RES_OK) {
                        return false;
                    }
                }
                break;
            }
            default:
                LV_LOG_WARN("Unknown cmaps format type %d.", cmap_table[i].format_type);
                return false;
        }
    }
    return true;
}

static int32_t load_cmaps(lv_fs_file_t * fp, lv_font_fmt_txt_dsc_t * font_dsc, uint32_t cmaps_start)
{
    int32_t cmaps_length = read_label_length(fp, cmaps_start);
    if(cmaps_length < 0) {
        return -1;
    }

    uint32_t cmaps_subtables_count;
    if(lv_fs_read(fp, &cmaps_subtables_count, sizeof(uint32_t), NULL) != LV_FS_RES_OK) {
        return -1;
    }

    lv_font_fmt_txt_cmap_t * cmaps =
            lv_malloc(cmaps_subtables_count * sizeof(lv_font_fmt_txt_cmap_t));

    lv_memset(cmaps, 0, cmaps_subtables_count * sizeof(lv_font_fmt_txt_cmap_t));

    font_dsc->cmaps = cmaps;
    font_dsc->cmap_num = cmaps_subtables_count;

    cmap_table_bin_t * cmaps_tables = lv_malloc(sizeof(cmap_table_bin_t) * font_dsc->cmap_num);

    bool success = load_cmaps_tables(fp, font_dsc, cmaps_start, cmaps_tables);

    lv_free(cmaps_tables);

    return success ? cmaps_length : -1;
}


static int unicode_list_compare(const void * ref, const void * element)
{
    return (*(uint16_t *)ref) - (*(uint16_t *)element);
}
static uint32_t get_glyph_dsc_id(const lv_font_fmt_txt_dsc_t *fdsc, uint32_t letter)
{
    if(letter == '\0') return 0;

    uint16_t i;
    for(i = 0; i < fdsc->cmap_num; i++) {

        /*Relative code point*/
        uint32_t rcp = letter - fdsc->cmaps[i].range_start;
        if(rcp >= fdsc->cmaps[i].range_length) continue;
        uint32_t glyph_id = 0;
        if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY) {
            glyph_id = fdsc->cmaps[i].glyph_id_start + rcp;
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL) {
            const uint8_t * gid_ofs_8 = fdsc->cmaps[i].glyph_id_ofs_list;
            /* The first character is always valid and should have offset = 0
             * However if a character is missing it also has offset=0.
             * So if there is a 0 not on the first position then it's a missing character */
            if(gid_ofs_8[rcp] == 0 && letter != fdsc->cmaps[i].range_start) continue;
            glyph_id = fdsc->cmaps[i].glyph_id_start + gid_ofs_8[rcp];
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_TINY) {
            uint16_t key = rcp;
            uint16_t * p = lv_utils_bsearch(&key, fdsc->cmaps[i].unicode_list, fdsc->cmaps[i].list_length,
                                            sizeof(fdsc->cmaps[i].unicode_list[0]), unicode_list_compare);

            if(p) {
                lv_uintptr_t ofs = p - fdsc->cmaps[i].unicode_list;
                glyph_id = fdsc->cmaps[i].glyph_id_start + (uint32_t) ofs;
            }
        }
        else if(fdsc->cmaps[i].type == LV_FONT_FMT_TXT_CMAP_SPARSE_FULL) {
            uint16_t key = rcp;
            uint16_t * p = lv_utils_bsearch(&key, fdsc->cmaps[i].unicode_list, fdsc->cmaps[i].list_length,
                                            sizeof(fdsc->cmaps[i].unicode_list[0]), unicode_list_compare);

            if(p) {
                lv_uintptr_t ofs = p - fdsc->cmaps[i].unicode_list;
                const uint16_t * gid_ofs_16 = fdsc->cmaps[i].glyph_id_ofs_list;
                glyph_id = fdsc->cmaps[i].glyph_id_start + gid_ofs_16[ofs];
            }
        }

        return glyph_id;
    }

    return 0;
}


static int32_t load_kern(lv_fs_file_t * fp, lv_font_fmt_txt_dsc_t * font_dsc, uint8_t format, uint32_t start)
{
    int32_t kern_length = read_label_length(fp, start);
    if(kern_length < 0) {
        return -1;
    }

    uint8_t kern_format_type;
    int32_t padding;
    uint32_t br;
    if(lv_fs_read(fp, &kern_format_type, sizeof(uint8_t), NULL) != LV_FS_RES_OK ||
       lv_fs_read(fp, &padding, 3 * sizeof(uint8_t), &br) != LV_FS_RES_OK) {
        return -1;
    }

    if(0 == kern_format_type) { /*sorted pairs*/
        lv_font_fmt_txt_kern_pair_t * kern_pair = lv_malloc(sizeof(lv_font_fmt_txt_kern_pair_t));

        lv_memset(kern_pair, 0, sizeof(lv_font_fmt_txt_kern_pair_t));

        font_dsc->kern_dsc = kern_pair;
        font_dsc->kern_classes = 0;

        uint32_t glyph_entries;
        if(lv_fs_read(fp, &glyph_entries, sizeof(uint32_t), NULL) != LV_FS_RES_OK) {
            return -1;
        }

        int ids_size;
        if(format == 0) {
            ids_size = sizeof(int8_t) * 2 * glyph_entries;
        }
        else {
            ids_size = sizeof(int16_t) * 2 * glyph_entries;
        }

        uint8_t * glyph_ids = lv_malloc(ids_size);
        int8_t * values = lv_malloc(glyph_entries);

        kern_pair->glyph_ids_size = format;
        kern_pair->pair_cnt = glyph_entries;
        kern_pair->glyph_ids = glyph_ids;
        kern_pair->values = values;

        if(lv_fs_read(fp, glyph_ids, ids_size, NULL) != LV_FS_RES_OK) {
            return -1;
        }

        if(lv_fs_read(fp, values, glyph_entries, NULL) != LV_FS_RES_OK) {
            return -1;
        }
    }
    else if(3 == kern_format_type) { /*array M*N of classes*/

        lv_font_fmt_txt_kern_classes_t * kern_classes = lv_malloc(sizeof(lv_font_fmt_txt_kern_classes_t));

        lv_memset(kern_classes, 0, sizeof(lv_font_fmt_txt_kern_classes_t));

        font_dsc->kern_dsc = kern_classes;
        font_dsc->kern_classes = 1;

        uint16_t kern_class_mapping_length;
        uint8_t kern_table_rows;
        uint8_t kern_table_cols;

        if(lv_fs_read(fp, &kern_class_mapping_length, sizeof(uint16_t), NULL) != LV_FS_RES_OK ||
           lv_fs_read(fp, &kern_table_rows, sizeof(uint8_t), NULL) != LV_FS_RES_OK ||
           lv_fs_read(fp, &kern_table_cols, sizeof(uint8_t), NULL) != LV_FS_RES_OK) {
            return -1;
        }

        int kern_values_length = sizeof(int8_t) * kern_table_rows * kern_table_cols;

        uint8_t * kern_left = lv_malloc(kern_class_mapping_length);
        uint8_t * kern_right = lv_malloc(kern_class_mapping_length);
        int8_t * kern_values = lv_malloc(kern_values_length);

        kern_classes->left_class_mapping  = kern_left;
        kern_classes->right_class_mapping = kern_right;
        kern_classes->left_class_cnt = kern_table_rows;
        kern_classes->right_class_cnt = kern_table_cols;
        kern_classes->class_pair_values = kern_values;

        uint32_t br1,br2,br3;

        if(lv_fs_read(fp, kern_left, kern_class_mapping_length, &br1) != LV_FS_RES_OK ||
           lv_fs_read(fp, kern_right, kern_class_mapping_length, &br2) != LV_FS_RES_OK ||
           lv_fs_read(fp, kern_values, kern_values_length, &br3) != LV_FS_RES_OK) {
            return -1;
        }
    }
    else {
        LV_LOG_WARN("Unknown kern_format_type: %d", kern_format_type);
        return -1;
    }

    return kern_length;
}

static int kern_pair_8_compare(const void * ref, const void * element)
{
    const kern_pair_ref_t * ref8_p = ref;
    const uint8_t * element8_p = element;

    /*If the MSB is different it will matter. If not return the diff. of the LSB*/
    if(ref8_p->gid_left != element8_p[0]) return ref8_p->gid_left - element8_p[0];
    else return ref8_p->gid_right - element8_p[1];
}

static int kern_pair_16_compare(const void * ref, const void * element)
{
    const kern_pair_ref_t * ref16_p = ref;
    const uint16_t * element16_p = element;

    /*If the MSB is different it will matter. If not return the diff. of the LSB*/
    if(ref16_p->gid_left != element16_p[0]) return ref16_p->gid_left - element16_p[0];
    else return ref16_p->gid_right - element16_p[1];
}

static int8_t get_kern_value(const lv_font_t * font, uint32_t gid_left, uint32_t gid_right)
{
    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *)font->dsc;

    int8_t value = 0;

    if(fdsc->kern_classes == 0) {
        /*Kern pairs*/
        const lv_font_fmt_txt_kern_pair_t * kdsc = fdsc->kern_dsc;
        if(kdsc->glyph_ids_size == 0) {
            /*Use binary search to find the kern value.
             *The pairs are ordered left_id first, then right_id secondly.*/
            const uint16_t * g_ids = kdsc->glyph_ids;
            kern_pair_ref_t g_id_both = {gid_left, gid_right};
            uint16_t * kid_p = lv_utils_bsearch(&g_id_both, g_ids, kdsc->pair_cnt, 2, kern_pair_8_compare);

            /*If the `g_id_both` were found get its index from the pointer*/
            if(kid_p) {
                lv_uintptr_t ofs = kid_p - g_ids;
                value = kdsc->values[ofs];
            }
        }
        else if(kdsc->glyph_ids_size == 1) {
            /*Use binary search to find the kern value.
             *The pairs are ordered left_id first, then right_id secondly.*/
            const uint32_t * g_ids = kdsc->glyph_ids;
            kern_pair_ref_t g_id_both = {gid_left, gid_right};
            uint32_t * kid_p = lv_utils_bsearch(&g_id_both, g_ids, kdsc->pair_cnt, 4, kern_pair_16_compare);

            /*If the `g_id_both` were found get its index from the pointer*/
            if(kid_p) {
                lv_uintptr_t ofs = kid_p - g_ids;
                value = kdsc->values[ofs];
            }

        }
        else {
            /*Invalid value*/
        }
    }
    else {
        /*Kern classes*/
        const lv_font_fmt_txt_kern_classes_t * kdsc = fdsc->kern_dsc;
        uint8_t left_class = kdsc->left_class_mapping[gid_left];
        uint8_t right_class = kdsc->right_class_mapping[gid_right];

        /*If class = 0, kerning not exist for that glyph
         *else got the value form `class_pair_values` 2D array*/
        if(left_class > 0 && right_class > 0) {
            value = kdsc->class_pair_values[(left_class - 1) * kdsc->right_class_cnt + (right_class - 1)];
        }

    }
    return value;
}
/* Get info about glyph of `unicode_letter` in `font` font.
 * Store the result in `dsc_out`.
 * The next letter (`unicode_letter_next`) might be used to calculate the width required by this glyph (kerning)
 */
bool lv_font_runtime_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter,
                                   uint32_t unicode_letter_next)
{
    bool is_tab = unicode_letter == '\t';
    if(is_tab) {
        unicode_letter = ' ';
    }
    lv_font_fmt_txt_dsc_t  *fdsc = (lv_font_fmt_txt_dsc_t *)font->dsc;
    uint32_t gid = get_glyph_dsc_id(fdsc, unicode_letter);
    if(!gid) return false;

    int8_t kvalue = 0;
    if(fdsc->kern_dsc) {
        uint32_t gid_next = get_glyph_dsc_id(fdsc, unicode_letter_next);
        if(gid_next) {
            kvalue = get_kern_value(font, gid, gid_next);
        }
    }

    glyph_info *info = (glyph_info *)font->user_data;
    if (!info)
        return false;

    lv_fs_file_t *fp = info->fp;

    uint32_t glyph_dsc_num;
    lv_font_fmt_txt_glyph_dsc_t gdsc;

    lv_fs_seek(fp,info->glyph_dsc_offset,LV_FS_SEEK_SET);

    if (lv_fs_read(fp, &glyph_dsc_num, sizeof(uint32_t), NULL) != LV_FS_RES_OK)
        return false;
    if (lv_fs_seek(fp,gid * sizeof(lv_font_fmt_txt_glyph_dsc_t),LV_FS_SEEK_CUR) != LV_FS_RES_OK
        || lv_fs_read(fp, &gdsc, sizeof(lv_font_fmt_txt_glyph_dsc_t), NULL) != LV_FS_RES_OK)
        return false;

    int32_t kv = ((int32_t)((int32_t)kvalue * fdsc->kern_scale) >> 4);

    uint32_t adv_w = gdsc.adv_w;
    if(is_tab) adv_w *= 2;

    adv_w += kv;
    adv_w  = (adv_w + (1 << 3)) >> 4;

    dsc_out->adv_w = adv_w;
    dsc_out->box_h = gdsc.box_h;
    dsc_out->box_w = gdsc.box_w;
    dsc_out->ofs_x = gdsc.ofs_x;
    dsc_out->ofs_y = gdsc.ofs_y;

    if(fdsc->stride == 0) dsc_out->stride = 0;
    else {
        /*e.g. font_dsc stride ==  4 means align to 4 byte boundary.
         *In glyph_dsc store the actual line length in bytes*/
        dsc_out->stride = LV_ROUND_UP(dsc_out->box_w, fdsc->stride);
    }

    dsc_out->format = (uint8_t)fdsc->bpp;
    dsc_out->is_placeholder = false;
    dsc_out->gid.index = gid;

    if(is_tab) dsc_out->box_w = dsc_out->box_w * 2;

    return true;
}

static const uint8_t opa4_table[16] = {0,  17, 34,  51,
                                       68, 85, 102, 119,
                                       136, 153, 170, 187,
                                       204, 221, 238, 255
};

#if LV_USE_FONT_COMPRESSED
static const uint8_t opa3_table[8] = {0, 36, 73, 109, 146, 182, 218, 255};
#endif
static const uint8_t opa2_table[4] = {0, 85, 170, 255};
/* Get the bitmap of `unicode_letter` from `font`. */
const void *lv_font_runtime_get_glyph_bitmap(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf)
{
    const lv_font_t * font = g_dsc->resolved_font;

    glyph_info *info = (glyph_info *)g_dsc->resolved_font->user_data;
    if (!info)
        return NULL;

    lv_fs_file_t *fp = (lv_fs_file_t *)info->fp;

    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *)font->dsc;
    uint32_t gid = g_dsc->gid.index;
    if(!gid) return NULL;

    lv_font_fmt_txt_glyph_dsc_t gdsc = {0};
    lv_fs_seek(fp,info->glyph_dsc_offset,LV_FS_SEEK_SET);

    if (lv_fs_seek(fp,gid * sizeof(lv_font_fmt_txt_glyph_dsc_t) + sizeof(uint32_t ),LV_FS_SEEK_CUR) != LV_FS_RES_OK
        || lv_fs_read(fp, &gdsc, sizeof(lv_font_fmt_txt_glyph_dsc_t), NULL) != LV_FS_RES_OK)
        return false;


    if(g_dsc->req_raw_bitmap) return &fdsc->glyph_bitmap[gdsc.bitmap_index];

    uint8_t * bitmap_out = draw_buf->data;
    int32_t gsize = (int32_t) gdsc.box_w * gdsc.box_h;
    if(gsize == 0) return NULL;

    uint16_t stride_in = g_dsc->stride;

    if(fdsc->bitmap_format == LV_FONT_FMT_TXT_PLAIN) {

        uint32_t px_byte_size = gsize / (8 / fdsc->bpp);
        lv_fs_seek(fp,info->glyph_bitmap_offset,LV_FS_SEEK_SET);

        uint32_t glyph_bitmap_length;
        uint8_t *bitmap_in = (uint8_t *)lv_malloc_zeroed(px_byte_size);

        if (lv_fs_read(fp,&glyph_bitmap_length, sizeof(uint32_t),NULL) != LV_FS_RES_OK)
            return NULL;
        if (lv_fs_seek(fp,gdsc.bitmap_index,LV_FS_SEEK_CUR) != LV_FS_RES_OK ||
            lv_fs_read(fp,bitmap_in,px_byte_size,NULL) != LV_FS_RES_OK)
            return NULL;

        //uint8_t * bitmap_in = &fdsc->glyph_bitmap[gdsc->bitmap_index];


        uint8_t * bitmap_out_tmp = bitmap_out;
        int32_t i = 0;
        int32_t x, y;
        uint32_t stride_out = lv_draw_buf_width_to_stride(gdsc.box_w, LV_COLOR_FORMAT_A8);
        if(fdsc->bpp == 1) {
            for(y = 0; y < gdsc.box_h; y ++) {
                uint16_t line_rem = stride_in != 0 ? stride_in : gdsc.box_w;
                for(x = 0; x < gdsc.box_w; x++, i++) {
                    i = i & 0x7;
                    if(i == 0) bitmap_out_tmp[x] = (*bitmap_in) & 0x80 ? 0xff : 0x00;
                    else if(i == 1) bitmap_out_tmp[x] = (*bitmap_in) & 0x40 ? 0xff : 0x00;
                    else if(i == 2) bitmap_out_tmp[x] = (*bitmap_in) & 0x20 ? 0xff : 0x00;
                    else if(i == 3) bitmap_out_tmp[x] = (*bitmap_in) & 0x10 ? 0xff : 0x00;
                    else if(i == 4) bitmap_out_tmp[x] = (*bitmap_in) & 0x08 ? 0xff : 0x00;
                    else if(i == 5) bitmap_out_tmp[x] = (*bitmap_in) & 0x04 ? 0xff : 0x00;
                    else if(i == 6) bitmap_out_tmp[x] = (*bitmap_in) & 0x02 ? 0xff : 0x00;
                    else if(i == 7) {
                        bitmap_out_tmp[x] = (*bitmap_in) & 0x01 ? 0xff : 0x00;
                        line_rem--;
                        bitmap_in++;
                    }
                }
                /*Handle stride*/
                if(stride_in) {
                    i = 0;  /*If there is a stride start from the next byte in the next line*/
                    bitmap_in += line_rem;
                }
                bitmap_out_tmp += stride_out;
            }
        }
        else if(fdsc->bpp == 2) {
            for(y = 0; y < gdsc.box_h; y ++) {
                uint16_t line_rem = stride_in != 0 ? stride_in : gdsc.box_w;
                for(x = 0; x < gdsc.box_w; x++, i++) {
                    i = i & 0x3;
                    if(i == 0) bitmap_out_tmp[x] = opa2_table[(*bitmap_in) >> 6];
                    else if(i == 1) bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 4) & 0x3];
                    else if(i == 2) bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 2) & 0x3];
                    else if(i == 3) {
                        bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 0) & 0x3];
                        line_rem--;
                        bitmap_in++;
                    }
                }

                /*Handle stride*/
                if(stride_in) {
                    i = 0;  /*If there is a stride start from the next byte in the next line*/
                    bitmap_in += line_rem;
                }
                bitmap_out_tmp += stride_out;
            }

        }
        else if(fdsc->bpp == 4) {
            for(y = 0; y < gdsc.box_h; y ++) {
                uint16_t line_rem = stride_in != 0 ? stride_in : gdsc.box_w;
                for(x = 0; x < gdsc.box_w; x++, i++) {
                    i = i & 0x1;
                    if(i == 0) {
                        bitmap_out_tmp[x] = opa4_table[(*bitmap_in) >> 4];
                    }
                    else if(i == 1) {
                        bitmap_out_tmp[x] = opa4_table[(*bitmap_in) & 0xF];
                        line_rem--;
                        bitmap_in++;
                    }
                }

                /*Handle stride*/
                if(stride_in) {
                    i = 0;  /*If there is a stride start from the next byte in the next line*/
                    bitmap_in += line_rem;
                }
                bitmap_out_tmp += stride_out;
            }
        }
        else if(fdsc->bpp == 8) {
            for(y = 0; y < gdsc.box_h; y ++) {
                uint16_t line_rem = stride_in != 0 ? stride_in : gdsc.box_w;
                for(x = 0; x < gdsc.box_w; x++, i++) {
                    bitmap_out_tmp[x] = *bitmap_in;
                    line_rem--;
                    bitmap_in++;
                }
                bitmap_out_tmp += stride_out;
                bitmap_in += line_rem;
            }
        }

        lv_draw_buf_flush_cache(draw_buf, NULL);
        return draw_buf;
    }
        /*Handle compressed bitmap*/
    else {
#if LV_USE_FONT_COMPRESSED
        bool prefilter = fdsc->bitmap_format == LV_FONT_FMT_TXT_COMPRESSED;
        decompress(&fdsc->glyph_bitmap[gdsc->bitmap_index], bitmap_out, gdsc->box_w, gdsc->box_h,
                   (uint8_t)fdsc->bpp, prefilter);
        lv_draw_buf_flush_cache(draw_buf, NULL);
        return draw_buf;
#else /*!LV_USE_FONT_COMPRESSED*/
        LV_LOG_WARN("Compressed fonts is used but LV_USE_FONT_COMPRESSED is not enabled in lv_conf.h");
        return NULL;
#endif
    }

    /*If not returned earlier then the letter is not found in this font*/
    return NULL;
}
/*
 * Loads a `lv_font_t` from a binary file, given a `lv_fs_file_t`.
 *
 * Memory allocations on `lvgl_load_font` should be immediately zeroed and
 * the pointer should be set on the `lv_font_t` data before any possible return.
 *
 * When something fails, it returns `false` and the memory on the `lv_font_t`
 * still needs to be freed using `lv_binfont_destroy`.
 *
 * `lv_binfont_destroy` will assume that all non-null pointers are allocated and
 * should be freed.
 */
static bool lvgl_load_font(lv_fs_file_t * fp, lv_font_t * font)
{
    lv_font_fmt_txt_dsc_t *font_dsc = (lv_font_fmt_txt_dsc_t *)
            lv_malloc(sizeof(lv_font_fmt_txt_dsc_t));

    glyph_info *info = (glyph_info *)lv_malloc(sizeof(glyph_info));

    font->user_data = info;
    info->fp = fp;

    lv_memset(font_dsc, 0, sizeof(lv_font_fmt_txt_dsc_t));

    font->dsc = font_dsc;

    /*header*/
    int32_t header_length = read_label_length(fp, 0);
    if (header_length < 0)
    {
        return false;
    }

    font_header_bin_t font_header;
    if (lv_fs_read(fp, &font_header, sizeof(font_header_bin_t), NULL) != LV_FS_RES_OK)
    {
        return false;
    }

    font->base_line = -font_header.descent;
    font->line_height = font_header.ascent - font_header.descent;
    font->get_glyph_dsc = lv_font_runtime_get_glyph_dsc;
    font->get_glyph_bitmap = lv_font_runtime_get_glyph_bitmap;
    font->subpx = font_header.subpixels_mode;
    font->underline_position = (int8_t) font_header.underline_position;
    font->underline_thickness = (int8_t) font_header.underline_thickness;

    font_dsc->bpp = font_header.bits_per_pixel;;
    font_dsc->kern_scale = font_header.kerning_scale;
    font_dsc->bitmap_format = font_header.compression_id;

    /*cmaps*/
    uint32_t cmaps_start = header_length;
    int32_t cmaps_length = load_cmaps(fp, font_dsc, cmaps_start);
    if(cmaps_length < 0) {
        return false;
    }

    /*kerning*/
    if(font_header.tables_count < 4) {
        font_dsc->kern_dsc = NULL;
        font_dsc->kern_classes = 0;
        font_dsc->kern_scale = 0;
        return true;
    }


    uint32_t kern_start = cmaps_start + cmaps_length;
    int32_t kern_length = load_kern(fp, font_dsc, font_header.glyph_id_format, kern_start);

    info->glyph_dsc_offset = kern_start + kern_length;
    uint32_t glyph_dsc_length  = read_label_length(fp, info->glyph_dsc_offset);

    info->glyph_bitmap_offset = info->glyph_dsc_offset + glyph_dsc_length;

    return kern_length >= 0;
    return true;
}