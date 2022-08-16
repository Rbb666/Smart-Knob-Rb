#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(Window);

static lv_obj_t *lmeterSec;
static lv_obj_t *labelSec;
static lv_meter_indicator_t *indic1;
static lv_meter_indicator_t *indic2;
static lv_meter_indicator_t *indic3;
static lv_timer_t *LabelUpdate_timer;
static lv_obj_t *back_img;

static void back_main_menu_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED) {
        Page->Pop();
    }
}

static void SW_LabelUpdate(lv_timer_t *tmr) {
    if (tmr == LabelUpdate_timer) {
        static int now_num = 0;
        static int old_num = 0;

        if (now_num > old_num) {
        } else if (now_num < old_num) {
        }

        if (now_num == 0) {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, 0);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, 0);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, 0);
        } else if (now_num < 0) {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, 100 + now_num - 2);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, 100 + now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, 100 + now_num + 2);
        } else {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, now_num + 2);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, now_num - 2);
        }
    }
}

// 返回按钮
static void Back_Img_Create(lv_obj_t *win) {
    LV_IMG_DECLARE(IMG_Back);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_opa(&style_btn, LV_OPA_TRANSP);

    /*Create button*/
    lv_obj_t *btn = lv_btn_create(win);
    lv_obj_set_size(btn, 28, 28);
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(btn, &style_btn, LV_PART_MAIN);

    /*Create image*/
    back_img = lv_img_create(win);
    lv_img_set_src(back_img, &IMG_Back);
    lv_obj_align(back_img, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_add_event_cb(btn, back_main_menu_cb, LV_EVENT_ALL, 0);

    lv_amin_start(back_img, -80, -10,
                  1, 1200, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

static void set_lmeterSec_value(lv_meter_indicator_t *indic, int32_t v) {
    lv_meter_set_indicator_end_value(lmeterSec, indic, v);
}

/**
 * @brief  创建表盘
 * @param  无
 * @retval 无
 */
static void LmeterSec_Create(lv_obj_t *win) {
    lmeterSec = lv_meter_create(win);
    /*Remove the circle from the middle*/
    lv_obj_set_style_border_width(lmeterSec, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(lmeterSec, 170, 170);
    lv_obj_set_opa_scale(lmeterSec, LV_OPA_TRANSP);
    lv_obj_align(lmeterSec, LV_ALIGN_CENTER, 0, 0);

    lv_meter_scale_t *scale_min = lv_meter_add_scale(lmeterSec);
    lv_meter_set_scale_ticks(lmeterSec, scale_min, 30, 1, 15, lv_color_white());
    lv_meter_set_scale_range(lmeterSec, scale_min, 0, 100, 360, 180);

    indic1 = lv_meter_add_arc(lmeterSec, scale_min, 15, lv_palette_main(LV_PALETTE_BLUE), 0);
    indic2 = lv_meter_add_arc(lmeterSec, scale_min, 10, lv_palette_main(LV_PALETTE_RED), -15);
    indic3 = lv_meter_add_arc(lmeterSec, scale_min, 5, lv_palette_main(LV_PALETTE_GREEN), -25);

    lv_amin_start(lmeterSec, -80, 0,
                  1, 300, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

/**
 * @brief  创建数字标签
 * @param  无
 * @retval 无
 */
static void LabelTime_Create(void) {
    LV_FONT_DECLARE(HandGotn_35);

    labelSec = lv_label_create(lmeterSec);

    lv_label_set_recolor(labelSec, true);
    lv_obj_set_style_text_color(labelSec, lv_color_white(), 0);
    lv_obj_set_style_text_font(labelSec, &HandGotn_35, 0);
    lv_label_set_text(labelSec, "00");
    lv_obj_align(labelSec, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(labelSec, -120, 0, 1, 500, 200, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup(void) {
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    LmeterSec_Create(appWindow);
    LabelTime_Create();
    Back_Img_Create(appWindow);

    LabelUpdate_timer = lv_timer_create(SW_LabelUpdate, 100, nullptr);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit(void) {
    if (LabelUpdate_timer)
        lv_timer_del(LabelUpdate_timer);

    lv_amin_start(labelSec,
                  lv_obj_get_x(labelSec),
                  120,
                  1,
                  400,
                  0,
                  (lv_anim_exec_xcb_t) lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(lmeterSec,
                  lv_obj_get_x(lmeterSec),
                  80,
                  1,
                  500,
                  200,
                  (lv_anim_exec_xcb_t) lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(back_img,
                  lv_obj_get_y(back_img),
                  -80,
                  1,
                  800,
                  300,
                  (lv_anim_exec_xcb_t) lv_obj_set_y,
                  lv_anim_path_bounce);

    PageDelay(LV_ANIM_TIME_DEFAULT);
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