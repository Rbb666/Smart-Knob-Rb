#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

#include <Hal/motor.h>
#include "Hal/Ble_Keyboard.h"
#include <FastLED.h>

PAGE_EXPORT(Window);

extern CRGB leds[NUM_LEDS];

static lv_obj_t *lmeterSec;
static lv_obj_t *labelSec;
static lv_meter_indicator_t *indic1;
static lv_meter_indicator_t *indic2;
static lv_meter_indicator_t *indic3;
static lv_timer_t *LabelUpdate_timer;
static lv_obj_t *back_img;

Ble_Interface ble_dev;

static void back_main_menu_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED)
    {
        Page->Pop();
    }
}

static void Led_show(int count)
{
    int led_num = abs(count) % NUM_LEDS;
    leds[led_num] = CRGB::SkyBlue;
    FastLED.show();
    if (led_num != 0)
    {
        leds[--led_num] = CRGB::Black;
        FastLED.show();
    }
    leds[++led_num] = CRGB::Black;
    FastLED.show();
}

static void SW_LabelUpdate(lv_timer_t *tmr)
{
    if (tmr == LabelUpdate_timer)
    {
        static int now_num = 0;
        static int old_num = 0;
        now_num = get_motor_position();

        if (now_num > old_num)
        {
            old_num = get_motor_position();
            ble_dev.keyboard_turn_up_volume();
        }
        else if (now_num < old_num)
        {
            old_num = get_motor_position();
            ble_dev.keyboard_turn_down_volume();
        }

        if (now_num == 0)
        {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, 0);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, 0);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, 0);
        }
        else if (now_num < 0)
        {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, 60 + now_num - 2);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, 60 + now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, 60 + now_num + 2);
        }
        else
        {
            lv_label_set_text_fmt(labelSec, "%d", now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic1, now_num + 2);
            lv_meter_set_indicator_end_value(lmeterSec, indic2, now_num);
            lv_meter_set_indicator_end_value(lmeterSec, indic3, now_num - 2);
        }
    }
}

static void Back_Img_Create(lv_obj_t *win)
{
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

    lv_amin_start(back_img, -80, -10, 1, 1200, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

static void set_lmeterSec_value(lv_meter_indicator_t *indic, int32_t v)
{
    lv_meter_set_indicator_end_value(lmeterSec, indic, v);
}

/**
 * @brief  创建表盘
 * @param  无
 * @retval 无
 */
static void LmeterSec_Create(lv_obj_t *win)
{
    static lv_style_t style_lmeter;

    lv_style_set_pad_left(&style_lmeter, 10);
    lv_style_set_bg_color(&style_lmeter, LV_COLOR_MAKE(0xFF, 0x00, 0x00));
    lv_style_set_bg_grad_color(&style_lmeter, LV_COLOR_MAKE(0xFF, 0x00, 0x00));
    lv_style_set_border_width(&style_lmeter, 0);
    lv_style_set_line_color(&style_lmeter, LV_COLOR_MAKE(0xC0, 0xC0, 0xC0));
    lv_style_set_text_color(&style_lmeter, lv_color_black());
    // lv_style_set_text_opa(&style_lmeter, 0);
    lv_style_set_radius(&style_lmeter, 10);

    lmeterSec = lv_meter_create(win);
    /*Remove the circle from the middle*/
    lv_obj_remove_style(lmeterSec, NULL, LV_PART_ITEMS);
    lv_obj_remove_style(lmeterSec, NULL, LV_PART_INDICATOR);
    lv_obj_add_style(lmeterSec, &style_lmeter, LV_PART_MAIN);
    lv_obj_set_size(lmeterSec, 170, 170);
    lv_obj_set_opa_scale(lmeterSec, LV_OPA_TRANSP);
    lv_obj_align(lmeterSec, LV_ALIGN_CENTER, 0, 0);

    lv_meter_scale_t *scale_min = lv_meter_add_scale(lmeterSec);
    lv_meter_set_scale_ticks(lmeterSec, scale_min, 30, 1, 15, lv_color_white());
    // lv_meter_set_scale_major_ticks(lmeterSec, scale_min, 1, 2, 15, lv_color_white(), 10);
    lv_meter_set_scale_range(lmeterSec, scale_min, 0, 60, 360, 180);

    indic1 = lv_meter_add_arc(lmeterSec, scale_min, 15, lv_palette_main(LV_PALETTE_BLUE), 0);
    indic2 = lv_meter_add_arc(lmeterSec, scale_min, 10, lv_palette_main(LV_PALETTE_RED), -15);
    indic3 = lv_meter_add_arc(lmeterSec, scale_min, 5, lv_palette_main(LV_PALETTE_GREEN), -25);

    lv_amin_start(lmeterSec, -80, 0, 1, 300, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

/**
 * @brief  创建数字标签
 * @param  无
 * @retval 无
 */
static void LabelTime_Create(void)
{
    LV_FONT_DECLARE(HandGotn_35);

    labelSec = lv_label_create(lmeterSec);

    static lv_style_t style_label1;

    lv_style_set_text_font(&style_label1, &HandGotn_35);
    lv_style_set_text_color(&style_label1, lv_color_white());
    lv_style_set_bg_grad_color(&style_label1, LV_COLOR_MAKE(0xFF, 0x00, 0x00)); // 渐变色
    lv_obj_add_style(labelSec, &style_label1, LV_PART_MAIN);

    lv_label_set_recolor(labelSec, true);
    // lv_label_set_text(labelSec, "0#FF0000 0#");
    lv_label_set_text(labelSec, "00");
    lv_obj_align(labelSec, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(labelSec, -120, 0, 1, 500, 200, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup(void)
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    LmeterSec_Create(appWindow);
    LabelTime_Create();
    Back_Img_Create(appWindow);

    LabelUpdate_timer = lv_timer_create(SW_LabelUpdate, 100, NULL);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit(void)
{
    if (LabelUpdate_timer)
        lv_timer_del(LabelUpdate_timer);

    lv_amin_start(labelSec,
                  lv_obj_get_x(labelSec),
                  120 * 2,
                  1,
                  400,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(lmeterSec,
                  lv_obj_get_x(lmeterSec),
                  80 * 2,
                  1,
                  500,
                  200,
                  (lv_anim_exec_xcb_t)lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(back_img,
                  lv_obj_get_y(back_img),
                  -80,
                  1,
                  800,
                  300,
                  (lv_anim_exec_xcb_t)lv_obj_set_y,
                  lv_anim_path_bounce);

    PageDelay(LV_ANIM_TIME_DEFAULT);
    lv_obj_clean(appWindow);

    update_motor_config(1);
    update_page_status(CHECKOUT_PAGE);
}

/**
 * @brief  页面事件
 * @param  obj:发生事件的对象
 * @param  event:事件编号
 * @retval 无
 */
static void Event(void *obj, uint8_t event)
{
}