//
// Created by Huwensong on 2025/6/17.
//

#ifndef SIMULATOR_LV_BIN_DEFINES_H
#define SIMULATOR_LV_BIN_DEFINES_H

typedef struct font_header_bin {
    uint16_t tables_count;
    uint16_t ascent;
    int16_t descent;
    uint16_t kerning_scale;
    uint8_t bits_per_pixel;
    uint8_t compression_id;
    uint8_t subpixels_mode;
    uint8_t padding;
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

#endif //SIMULATOR_LV_BIN_DEFINES_H
