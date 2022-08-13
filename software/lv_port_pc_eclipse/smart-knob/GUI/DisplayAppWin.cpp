#include "DisplayPrivate.h"

static lv_obj_t *appWindow_Grp[PAGE_MAX];

lv_obj_t *AppWindow_GetObj(uint8_t pageID) {
    return (pageID < PAGE_MAX) ? appWindow_Grp[pageID] : NULL;
}

// 设置主题背景样式 创建page窗体
void AppWindow_Create(lv_obj_t *par) {
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);
    lv_style_set_border_width(&style, 0);
    // lv_style_set_bg_grad_color(&style, lv_color_black());
    lv_style_set_radius(&style, LV_RADIUS_CIRCLE);
    lv_style_set_bg_color(&style, lv_color_black());

    for (int i = 0; i < PAGE_MAX; i++) {
        lv_obj_t *obj = lv_obj_create(par);
        lv_obj_add_style(obj, &style, LV_PART_MAIN);
        lv_obj_set_size(obj, lv_obj_get_width(par), lv_obj_get_height(par));
        lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);

        appWindow_Grp[i] = obj;
    }
}
