#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(Switch);

extern "C"
{
LV_IMG_DECLARE(IMG_Bedroom) ;
LV_IMG_DECLARE(IMG_Livingrm) ;
}

typedef struct {
    const void *src_img;
    const char *info;
    const uint8_t pageID;
    lv_obj_t *obj;
} Panel_TypeDef;

#define PANEL_DEF(name, info)    \
    {                            \
        &IMG_##name, #name, NULL \
    }

static Panel_TypeDef Panel_Grp[] = {
        PANEL_DEF(Bedroom, "Bed room"),
        PANEL_DEF(Livingrm, "living room"),
};

static lv_style_t btn_style;
static lv_style_t style_pr;
static lv_obj_t *contTemp;
static lv_obj_t *labelTime;
static lv_obj_t *panel;

static lv_obj_t *circle_bg[__Sizeof(Panel_Grp)];
static lv_obj_t *circle_font[__Sizeof(Panel_Grp)];
static lv_obj_t *button[__Sizeof(Panel_Grp)];
static lv_obj_t *scroll_cont[__Sizeof(Panel_Grp)];
static lv_obj_t *img_icon[__Sizeof(Panel_Grp)];

static lv_timer_t *timer = NULL;

static void onTimer(lv_timer_t *tmr) {
    if (tmr == timer) {
    }
}

static void button_callback(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED) {
        static bool btn_flg = false;

        if (btn == button[0] && btn_flg == false) {
            lv_obj_set_style_bg_color(button[0], lv_palette_main(LV_PALETTE_GREY), 0);
            btn_flg = !btn_flg;
        } else if (btn == button[0] && btn_flg == true) {
            lv_obj_set_style_bg_color(button[0], lv_color_white(), 0);
            btn_flg = !btn_flg;
        }
        if (btn == button[1] && btn_flg == false) {
            lv_obj_set_style_bg_color(button[1], lv_palette_main(LV_PALETTE_GREY), 0);
            btn_flg = !btn_flg;
        } else if (btn == button[1] && btn_flg == true) {
            lv_obj_set_style_bg_color(button[1], lv_color_white(), 0);
            btn_flg = !btn_flg;
        }
    }
}

static void ContTime_Create(lv_obj_t *win) {
    contTemp = lv_obj_create(win);
    lv_obj_set_size(contTemp, 135, 50);
    lv_obj_align(contTemp, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_opa_scale(contTemp, LV_OPA_TRANSP);
    lv_obj_set_scrollbar_mode(contTemp, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_border_opa(contTemp, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(contTemp, 2, 0);
    lv_obj_set_style_border_color(contTemp, lv_color_white(), 0);
    lv_obj_set_style_radius(contTemp, 10, 0);
    lv_amin_start(contTemp, -50, 10, 1, 500, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

static void labelTime_Create(void) {
    LV_FONT_DECLARE(HandGotn_20);

    labelTime = lv_label_create(contTemp);
    lv_obj_set_style_text_color(labelTime, lv_color_white(), 0);
    lv_obj_set_style_text_font(labelTime, &HandGotn_20, 0);
    lv_label_set_text(labelTime, "Time-12:59");
    lv_obj_align(labelTime, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
}

static void btn_style_create(void) {
    lv_style_init(&btn_style);

    lv_style_set_radius(&btn_style, LV_RADIUS_CIRCLE);
    lv_style_set_bg_opa(&btn_style, LV_OPA_70);
    lv_style_set_bg_color(&btn_style, lv_color_white());
    lv_style_set_border_color(&btn_style, lv_color_make(251, 153, 28));
    lv_style_set_border_width(&btn_style, 1);
    // lv_style_set_bg_grad_color(&btn_style, lv_palette_darken(LV_PALETTE_GREY, 2));
    // lv_style_set_bg_grad_dir(&btn_style, LV_GRAD_DIR_VER);

    lv_style_set_shadow_width(&btn_style, 5);
    lv_style_set_shadow_color(&btn_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_ofs_y(&btn_style, 5);

    lv_style_set_outline_opa(&btn_style, LV_OPA_COVER);
    lv_style_set_outline_color(&btn_style, lv_color_make(251, 153, 28));

    lv_style_set_text_color(&btn_style, lv_color_white());
    lv_style_set_pad_all(&btn_style, 10);

    /*Init the pressed btn_style*/
    lv_style_init(&style_pr);

    /*Ad a large outline when pressed*/
    lv_style_set_outline_width(&style_pr, 10);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

    lv_style_set_translate_y(&style_pr, 2);
    lv_style_set_shadow_ofs_y(&style_pr, 2);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 2));
    lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 4));

    /*Add a transition to the the outline*/
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 500, 0, NULL);

    lv_style_set_transition(&style_pr, &trans);
}

static void anim_size_cb(_lv_obj_t *var, int32_t v) {
    lv_obj_set_size(var, v, v);
}

static void button_create(lv_obj_t *parent, uint8_t panel_num) {
    circle_bg[panel_num] = lv_obj_create(parent);
    lv_obj_set_size(circle_bg[panel_num], 55, 55);
    lv_obj_align(circle_bg[panel_num], LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_scrollbar_mode(circle_bg[panel_num], LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(circle_bg[panel_num], lv_color_make(225, 234, 239), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(circle_bg[panel_num], 2, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(circle_bg[panel_num], lv_color_make(210, 221, 225), LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_width(circle_bg[panel_num], 4, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(circle_bg[panel_num], lv_palette_darken(LV_PALETTE_GREY, 2), LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(circle_bg[panel_num], 4, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(circle_bg[panel_num], LV_RADIUS_CIRCLE, 0);

    circle_font[panel_num] = lv_obj_create(parent);
    lv_obj_set_size(circle_font[panel_num], 40, 40);
    lv_obj_align(circle_font[panel_num], LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_scrollbar_mode(circle_font[panel_num], LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(circle_font[panel_num], lv_color_make(223, 232, 239), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(circle_font[panel_num], 1, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(circle_font[panel_num], lv_color_make(210, 221, 225), LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_width(circle_font[panel_num], 4, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(circle_font[panel_num], lv_color_make(175, 189, 192), LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(circle_font[panel_num], 4, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(circle_font[panel_num], LV_RADIUS_CIRCLE, 0);

    button[panel_num] = lv_btn_create(circle_font[panel_num]);
    lv_obj_set_size(button[panel_num], 20, 20);

    lv_obj_remove_style_all(button[panel_num]);
    lv_obj_add_style(button[panel_num], &btn_style, 0);
    lv_obj_add_style(button[panel_num], &style_pr, LV_STATE_PRESSED);
    lv_obj_set_style_radius(button[panel_num], LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_add_flag(button[panel_num], LV_OBJ_FLAG_CHECKABLE);
    lv_obj_align(button[panel_num], LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(button[panel_num], button_callback, LV_EVENT_ALL, NULL);

    LV_IMG_DECLARE(IMG_SW);
    /*Create image*/
    lv_obj_t *sw_img = lv_img_create(button[panel_num]);
    lv_img_set_src(sw_img, &IMG_SW);
    lv_obj_align(sw_img, LV_ALIGN_CENTER, 0, 0);
}

static void panel_create(lv_obj_t *par, uint8_t page_num, const void *image, const char *infos) {
    /*Create cont*/
    scroll_cont[page_num] = lv_obj_create(par);
    lv_obj_set_scrollbar_mode(scroll_cont[page_num], LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(scroll_cont[page_num], 155, 120);
    lv_obj_set_style_bg_opa(scroll_cont[page_num], (lv_opa_t) 250, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(scroll_cont[page_num], 0, 0);
    lv_obj_set_style_bg_color(scroll_cont[page_num], lv_color_make(225, 234, 239), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scroll_cont[page_num], 15, 0);

    /*Create image*/
    img_icon[page_num] = lv_img_create(scroll_cont[page_num]);
    lv_img_set_src(img_icon[page_num], image);
    lv_obj_align(img_icon[page_num], LV_ALIGN_LEFT_MID, 10, 0);

    button_create(scroll_cont[page_num], page_num);
}

static void sw_meter_create(lv_obj_t *win) {
    panel = lv_obj_create(win);
    lv_obj_set_size(panel, 170, 135);
    lv_obj_set_scroll_snap_y(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_bg_opa(panel, (lv_opa_t) LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 0, 0);
    lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);

    for (int i = 0; i < __Sizeof(Panel_Grp); i++) {
        panel_create(panel, i, (void *) Panel_Grp[i].src_img, Panel_Grp[i].info);
    }

    lv_obj_update_snap(panel, LV_ANIM_ON);

    lv_amin_start(panel, 135, -5, 1, 400, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup() {
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    ContTime_Create(appWindow);
    labelTime_Create();
    btn_style_create();
    sw_meter_create(appWindow);

    // timer = lv_timer_create(onTimer, 100, NULL);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit() {
    lv_amin_start(contTemp, lv_obj_get_y(contTemp),
                  -135, 1, 400, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);

    lv_amin_start(panel, lv_obj_get_y(panel), 170,
                  1, 500, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);

    lv_obj_clean(appWindow);
}

/**
 * @brief  页面事件
 * @param  obj:发生事件的对象
 * @param  event:事件编号
 * @retval 无
 */
static void Event(void *obj, uint8_t event) {
}