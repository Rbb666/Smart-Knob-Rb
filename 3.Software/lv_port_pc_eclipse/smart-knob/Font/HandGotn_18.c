/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef HANDGOTN_18
#define HANDGOTN_18 1
#endif

#if HANDGOTN_18

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0041 "A" */
    0x7, 0x0, 0x7c, 0x3, 0xe0, 0x1b, 0x1, 0xdc,
    0xc, 0xe0, 0xe3, 0x87, 0x1c, 0x7f, 0xf3, 0xff,
    0x98, 0x1d, 0xc0, 0x7e, 0x3, 0x80,

    /* U+0042 "B" */
    0xff, 0xdf, 0xff, 0x83, 0xf0, 0x7e, 0xf, 0xff,
    0xbf, 0xf7, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xff,
    0xef, 0xf8,

    /* U+0043 "C" */
    0x1f, 0xe7, 0xfd, 0xc0, 0x78, 0xe, 0x1, 0xc0,
    0x38, 0x7, 0x0, 0xe0, 0x1c, 0x1, 0xc0, 0x1f,
    0xf1, 0xfe,

    /* U+0044 "D" */
    0xff, 0x8f, 0xfc, 0xe0, 0xee, 0x7, 0xe0, 0x7e,
    0x7, 0xe0, 0x7e, 0x7, 0xe0, 0x7e, 0x7, 0xe0,
    0xef, 0xfc, 0xff, 0x80,

    /* U+0045 "E" */
    0x1f, 0xe7, 0xfd, 0xc0, 0x70, 0xe, 0x1, 0xff,
    0xff, 0xff, 0x0, 0xe0, 0x1c, 0x1, 0xc0, 0x1f,
    0xf1, 0xfe,

    /* U+0046 "F" */
    0x1f, 0xef, 0xfd, 0xc0, 0x70, 0xe, 0x1, 0xff,
    0xff, 0xff, 0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70,
    0xe, 0x0,

    /* U+0047 "G" */
    0x1f, 0xf3, 0xff, 0x70, 0xf, 0x0, 0xe0, 0xe,
    0x7f, 0xe7, 0xfe, 0x7, 0xe0, 0x7e, 0x7, 0x70,
    0x73, 0xff, 0x1f, 0xf0,

    /* U+0048 "H" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xff,
    0xff, 0xff, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xe,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0xfe,

    /* U+004A "J" */
    0x3, 0x81, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0xe,
    0x7, 0x3, 0x81, 0xc1, 0xff, 0xef, 0xe0,

    /* U+004B "K" */
    0xe0, 0xfc, 0x3b, 0x8e, 0x73, 0xce, 0xf1, 0xdc,
    0x3f, 0x7, 0x70, 0xef, 0x1c, 0xf3, 0x8e, 0x70,
    0xee, 0xe,

    /* U+004C "L" */
    0xe0, 0x70, 0x38, 0x1c, 0xe, 0x7, 0x3, 0x81,
    0xc0, 0xe0, 0x70, 0x3c, 0xf, 0xf3, 0xf8,

    /* U+004D "M" */
    0xf0, 0x3f, 0xf0, 0x7f, 0xe0, 0xff, 0xc1, 0xfe,
    0xc7, 0xfd, 0x8d, 0xfb, 0x1b, 0xf7, 0x77, 0xe6,
    0xcf, 0xcd, 0x9f, 0x9f, 0x3f, 0x1c, 0x7e, 0x38,
    0xe0,

    /* U+004E "N" */
    0xf0, 0xfe, 0x1f, 0xe3, 0xfc, 0x7e, 0xcf, 0xd9,
    0xf9, 0xbf, 0x37, 0xe3, 0xfc, 0x7f, 0x8f, 0xf0,
    0xfe, 0x1e,

    /* U+004F "O" */
    0x1f, 0xc1, 0xff, 0x1e, 0x3d, 0xe0, 0xfe, 0x3,
    0xf0, 0x1f, 0x80, 0xfc, 0x7, 0xe0, 0x3f, 0x83,
    0xde, 0x3c, 0x7f, 0xc1, 0xfc, 0x0,

    /* U+0050 "P" */
    0xff, 0x9f, 0xfb, 0x87, 0xf0, 0x7e, 0xf, 0xc3,
    0xff, 0xf7, 0xfc, 0xe0, 0x1c, 0x3, 0x80, 0x70,
    0xe, 0x0,

    /* U+0051 "Q" */
    0x1f, 0xc0, 0xff, 0x87, 0x8f, 0x3c, 0x1e, 0xe0,
    0x3b, 0x80, 0xee, 0x3, 0xb8, 0xe, 0xe0, 0x3b,
    0xc1, 0xe7, 0x8f, 0xf, 0xfc, 0x1f, 0xf8,

    /* U+0052 "R" */
    0xff, 0x8f, 0xfc, 0xe0, 0xee, 0xe, 0xe0, 0xee,
    0xe, 0xff, 0xcf, 0xf8, 0xe3, 0x8e, 0x18, 0xe1,
    0xce, 0xe, 0xe0, 0xe0,

    /* U+0053 "S" */
    0x3f, 0xef, 0xfe, 0xe0, 0xe, 0x0, 0xe0, 0xf,
    0xfe, 0x7f, 0xf0, 0x7, 0x0, 0x70, 0x7, 0x0,
    0x77, 0xfe, 0x7f, 0xc0,

    /* U+0054 "T" */
    0xff, 0xff, 0xfc, 0x38, 0x7, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0,

    /* U+0055 "U" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0xc7, 0xbf,
    0xe3, 0xf8,

    /* U+0056 "V" */
    0x70, 0x1d, 0xc0, 0x77, 0x83, 0x8e, 0xe, 0x38,
    0x30, 0x71, 0xc1, 0xc7, 0x3, 0xb8, 0xe, 0xe0,
    0x1f, 0x0, 0x7c, 0x1, 0xf0, 0x3, 0x80,

    /* U+0057 "W" */
    0xe0, 0xf0, 0x7e, 0xf, 0x7, 0x70, 0xf0, 0xe7,
    0x1f, 0x8e, 0x71, 0xf8, 0xe3, 0x19, 0x8c, 0x39,
    0x99, 0xc3, 0xb9, 0xdc, 0x1b, 0x9d, 0x81, 0xf0,
    0xd8, 0x1f, 0xf, 0x80, 0xf0, 0xf0, 0xf, 0x7,
    0x0,

    /* U+0058 "X" */
    0xe0, 0xe7, 0x1e, 0x39, 0xc3, 0xb8, 0x1f, 0x80,
    0xf0, 0xe, 0x0, 0xf0, 0x1f, 0x83, 0xb8, 0x79,
    0xc7, 0xe, 0xe0, 0xe0,

    /* U+0059 "Y" */
    0xe0, 0x7b, 0x83, 0x8e, 0x38, 0x73, 0xc1, 0xdc,
    0x7, 0xc0, 0x3e, 0x0, 0xe0, 0x7, 0x0, 0x38,
    0x1, 0xc0, 0xe, 0x0, 0x70, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xfc, 0x7, 0x81, 0xe0, 0x78, 0xe,
    0x3, 0x80, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0x7f,
    0xff, 0xfe,

    /* U+0061 "a" */
    0x7f, 0x1f, 0xe0, 0x1c, 0x7, 0x7f, 0xff, 0xfe,
    0x1f, 0x87, 0xff, 0xdf, 0xf0,

    /* U+0062 "b" */
    0xe0, 0x38, 0xe, 0x3, 0xfc, 0xff, 0xb8, 0xfe,
    0x1f, 0x87, 0xe1, 0xf8, 0x7e, 0x3b, 0xfe, 0xfe,
    0x0,

    /* U+0063 "c" */
    0x3f, 0xbf, 0xdc, 0x1c, 0xe, 0x7, 0x3, 0x80,
    0xe0, 0x7f, 0x9f, 0xc0,

    /* U+0064 "d" */
    0x1, 0xc0, 0x70, 0x1c, 0xff, 0x7f, 0xfc, 0x7e,
    0x1f, 0x87, 0xe1, 0xf8, 0x7f, 0x1d, 0xff, 0x3f,
    0xc0,

    /* U+0065 "e" */
    0x3f, 0xbf, 0xf8, 0x1c, 0xf, 0xff, 0xff, 0x81,
    0xc0, 0x7f, 0x9f, 0xc0,

    /* U+0066 "f" */
    0x3e, 0x7c, 0xe7, 0xff, 0xe7, 0xe, 0x1c, 0x38,
    0x70, 0xe1, 0xc3, 0x80,

    /* U+0067 "g" */
    0x3f, 0xdf, 0xf7, 0x1f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xc7, 0x7f, 0xcf, 0xf0, 0x1c, 0x7, 0x7f,
    0x9f, 0xe0,

    /* U+0068 "h" */
    0xe0, 0x70, 0x38, 0x1f, 0xef, 0xff, 0x1f, 0x8f,
    0xc7, 0xe3, 0xf1, 0xf8, 0xfc, 0x7e, 0x38,

    /* U+0069 "i" */
    0xfc, 0x7f, 0xff, 0xff, 0xfe,

    /* U+006A "j" */
    0x1c, 0x70, 0x7, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c, 0x71, 0xfe, 0xf8,

    /* U+006B "k" */
    0xe0, 0x38, 0xe, 0x3, 0x87, 0xe3, 0xb9, 0xce,
    0xe3, 0xf0, 0xfc, 0x3b, 0x8e, 0x73, 0x8e, 0xe3,
    0xc0,

    /* U+006C "l" */
    0xff, 0xff, 0xff, 0xff, 0xfe,

    /* U+006D "m" */
    0xff, 0xf3, 0xff, 0xfe, 0x71, 0xf9, 0xc7, 0xe7,
    0x1f, 0x9c, 0x7e, 0x71, 0xf9, 0xc7, 0xe7, 0x1f,
    0x9c, 0x70,

    /* U+006E "n" */
    0xff, 0x7f, 0xf8, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xc7, 0xe3, 0xf1, 0xc0,

    /* U+006F "o" */
    0x1e, 0x1f, 0xe7, 0x3b, 0x87, 0xe1, 0xf8, 0x7e,
    0x1d, 0xce, 0x7f, 0x87, 0x80,

    /* U+0070 "p" */
    0xff, 0x3f, 0xee, 0x3b, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0x8f, 0xff, 0xbf, 0xce, 0x3, 0x80, 0xe0,
    0x38, 0x0,

    /* U+0071 "q" */
    0x3f, 0xdf, 0xff, 0x1f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xc7, 0x7f, 0xcf, 0xf0, 0x1c, 0x7, 0x1,
    0xc0, 0x70,

    /* U+0072 "r" */
    0xfd, 0xff, 0xbf, 0x7e, 0x1c, 0x38, 0x70, 0xe1,
    0xc0,

    /* U+0073 "s" */
    0x7f, 0x7f, 0xb8, 0x1c, 0xf, 0xf3, 0xfc, 0xe,
    0x7, 0xff, 0xff, 0x80,

    /* U+0074 "t" */
    0x38, 0x70, 0xe7, 0xff, 0xe7, 0xe, 0x1c, 0x38,
    0x70, 0xe1, 0xf1, 0xe0,

    /* U+0075 "u" */
    0xe3, 0xf1, 0xf8, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xc7, 0xff, 0xbf, 0xc0,

    /* U+0076 "v" */
    0xe0, 0xec, 0x19, 0xc7, 0x38, 0xc3, 0xb8, 0x77,
    0x6, 0xc0, 0xf8, 0x1e, 0x1, 0xc0,

    /* U+0077 "w" */
    0xe3, 0x87, 0xe3, 0xc6, 0x63, 0xc6, 0x67, 0xce,
    0x76, 0xee, 0x36, 0xec, 0x36, 0x7c, 0x3e, 0x7c,
    0x1c, 0x78, 0x1c, 0x38,

    /* U+0078 "x" */
    0xe1, 0xdc, 0xe3, 0xf0, 0xfc, 0x1e, 0x7, 0x83,
    0xe0, 0xfc, 0x73, 0xb8, 0x70,

    /* U+0079 "y" */
    0xe3, 0xf1, 0xf8, 0xfc, 0x7e, 0x3f, 0x1f, 0x8f,
    0xc7, 0xff, 0xbf, 0xc0, 0xe0, 0x7f, 0xf7, 0xf0,

    /* U+007A "z" */
    0xff, 0xff, 0xc1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1,
    0xe0, 0xff, 0xff, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 208, .box_w = 13, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 217, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 189, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 222, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 195, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 96, .adv_w = 195, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 214, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 209, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 89, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 161, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 193, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 160, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 276, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 224, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 223, .box_w = 13, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 214, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 223, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 216, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 217, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 351, .adv_w = 178, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 221, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 202, .box_w = 14, .box_h = 13, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 410, .adv_w = 319, .box_w = 20, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 443, .adv_w = 186, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 179, .box_w = 13, .box_h = 13, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 485, .adv_w = 211, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 503, .adv_w = 183, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 176, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 152, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 545, .adv_w = 175, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 562, .adv_w = 152, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 574, .adv_w = 113, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 586, .adv_w = 176, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 604, .adv_w = 173, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 619, .adv_w = 77, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 624, .adv_w = 77, .box_w = 6, .box_h = 17, .ofs_x = -2, .ofs_y = -4},
    {.bitmap_index = 637, .adv_w = 173, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 76, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 659, .adv_w = 253, .box_w = 14, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 677, .adv_w = 173, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 689, .adv_w = 175, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 176, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 720, .adv_w = 176, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 738, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 747, .adv_w = 171, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 759, .adv_w = 119, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 173, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 783, .adv_w = 176, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 797, .adv_w = 251, .box_w = 16, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 817, .adv_w = 159, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 830, .adv_w = 173, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 846, .adv_w = 168, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 27,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    1, 1,
    1, 20,
    1, 21,
    1, 22,
    1, 23,
    1, 25,
    2, 22,
    2, 23,
    2, 25,
    3, 19,
    6, 1,
    6, 20,
    6, 27,
    7, 1,
    7, 20,
    7, 25,
    11, 1,
    11, 3,
    11, 15,
    11, 21,
    11, 23,
    11, 25,
    11, 31,
    11, 41,
    11, 47,
    11, 51,
    12, 1,
    12, 15,
    12, 20,
    12, 21,
    12, 22,
    12, 23,
    12, 25,
    12, 51,
    15, 24,
    15, 25,
    16, 1,
    16, 27,
    16, 31,
    16, 41,
    16, 45,
    18, 27,
    18, 31,
    18, 41,
    18, 47,
    18, 51,
    19, 19,
    20, 1,
    20, 20,
    20, 27,
    20, 29,
    20, 31,
    20, 41,
    20, 44,
    20, 45,
    20, 47,
    20, 49,
    20, 51,
    21, 1,
    22, 1,
    22, 27,
    22, 31,
    22, 41,
    22, 47,
    23, 1,
    23, 27,
    23, 31,
    23, 41,
    24, 3,
    24, 15,
    24, 31,
    25, 1,
    25, 3,
    25, 15,
    25, 27,
    25, 31,
    25, 41,
    25, 47,
    32, 49
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    11, -26, -5, -5, -5, -17, -5, -5,
    -10, 5, -12, 5, -6, 5, -5, 5,
    8, -8, -8, -10, -5, 8, -5, -5,
    -5, -8, 5, -10, -40, -5, -21, -9,
    -32, -5, -5, -5, -18, -8, -5, -5,
    -5, -5, -10, -10, -5, -5, 5, -26,
    7, -36, -36, -36, -36, -25, -25, -23,
    -29, -29, -5, -5, -13, -13, -13, -8,
    -5, -8, -8, -8, -5, -5, -8, -17,
    -5, -5, -26, -26, -26, -21, 11
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 79,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t HandGotn_18 = {
#else
lv_font_t HandGotn_18 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if HANDGOTN_18*/

