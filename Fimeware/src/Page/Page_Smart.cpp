#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

#include <Hal/motor.h>
#include "Hal/Ble_Keyboard.h"

#define scan_turn_time 100

PAGE_EXPORT(Smart);

Ble_Interface ble;

/*显示容器*/
static lv_obj_t *contKPaTemp;
/*返回图片*/
static lv_obj_t *back_img;
/*当前页面公共的标签样式*/
static lv_style_t style_label_public;
/*湿度值标签*/
static lv_obj_t *labelHmi;
/*温度值标签*/
static lv_obj_t *labelTemp;
static lv_obj_t *chart_cont;
static lv_chart_series_t *rssi0_curve;
static lv_chart_series_t *rssi1_curve;
static lv_obj_t *chart_fre_label;
static lv_obj_t *rx_quality_chart;
static lv_timer_t *scan_chart_timer;

static void page_scan_chart_timer_event(lv_timer_t *tmr)
{
    static int motor_position = 0;

    if (tmr == scan_chart_timer)
    {
        motor_position = get_motor_position();
        lv_chart_set_next_value(rx_quality_chart, rssi0_curve, motor_position);
    }
}

static void back_main_menu_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED)
    {
        Page->Pop();
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
}

/**
 * @brief  创建湿度和温度标签
 * @param  无
 * @retval 无
 */
static void labelHmiTemp_Create()
{
    LV_IMG_DECLARE(IMG_Pressure);
    lv_obj_t *imgP = lv_img_create(contKPaTemp);
    lv_img_set_src(imgP, &IMG_Pressure);
    lv_obj_align(imgP, LV_ALIGN_LEFT_MID, 8, -12);

    LV_IMG_DECLARE(IMG_Temperature);
    lv_obj_t *imgT = lv_img_create(contKPaTemp);
    lv_img_set_src(imgT, &IMG_Temperature);
    lv_obj_align(imgT, LV_ALIGN_LEFT_MID, 8, 12);

    LV_FONT_DECLARE(HandGotn_14);
    LV_FONT_DECLARE(HandGotn_20);
    static lv_style_t style_label_public;
    lv_style_set_text_color(&style_label_public, lv_color_white());
    lv_style_set_text_font(&style_label_public, &HandGotn_14);
    lv_style_set_text_font(&style_label_public, &HandGotn_20);
    lv_style_set_bg_grad_color(&style_label_public, lv_color_black()); // 渐变色

    static lv_style_t style2_label;
    style2_label = style_label_public;

    labelHmi = lv_label_create(contKPaTemp);
    lv_obj_add_style(labelHmi, &style_label_public, LV_PART_MAIN);
    lv_label_set_text(labelHmi, "00.0%");
    lv_obj_align_to(labelHmi, imgP, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    labelTemp = lv_label_create(contKPaTemp);
    lv_obj_add_style(labelTemp, &style2_label, LV_PART_MAIN);
    lv_label_set_text(labelTemp, "00.0C");
    lv_obj_align_to(labelTemp, imgT, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
}

/**
 * @brief  创建数据容器
 * @param  无
 * @retval 无
 */
static void ContKPaTemp_Create(lv_obj_t *win)
{
    contKPaTemp = lv_obj_create(win);
    lv_obj_set_size(contKPaTemp, 130, 70);
    lv_obj_align(contKPaTemp, LV_ALIGN_CENTER, 0, -30);
    lv_obj_set_opa_scale(contKPaTemp, LV_OPA_TRANSP);
    lv_obj_set_scrollbar_mode(contKPaTemp, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_cont;
    lv_style_set_border_opa(&style_cont, LV_OPA_COVER);
    lv_style_set_border_width(&style_cont, 2);
    lv_style_set_border_color(&style_cont, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_radius(&style_cont, 10);
    lv_obj_add_style(contKPaTemp, &style_cont, LV_PART_MAIN);
}

static void Page_scan_chart_create(lv_obj_t *win)
{
    chart_cont = lv_obj_create(win);
    lv_obj_remove_style_all(chart_cont);
    lv_obj_set_style_bg_opa(chart_cont, (lv_opa_t)0, LV_STATE_DEFAULT);
    lv_obj_set_size(chart_cont, 180, 100);
    lv_obj_align(chart_cont, LV_ALIGN_CENTER, 0, 50);

    rx_quality_chart = lv_chart_create(chart_cont);
    lv_obj_remove_style(rx_quality_chart, NULL, LV_PART_MAIN);
    lv_obj_set_size(rx_quality_chart, 130, 60);
    lv_obj_align(rx_quality_chart, LV_ALIGN_BOTTOM_MID, 5, 0);

    lv_chart_set_type(rx_quality_chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/
    lv_chart_set_range(rx_quality_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 80);
    lv_chart_set_range(rx_quality_chart, LV_CHART_AXIS_PRIMARY_X, 0, 8);

    lv_obj_set_style_bg_color(rx_quality_chart, lv_color_make(130, 130, 130), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(rx_quality_chart, 100, LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(rx_quality_chart, 255, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(rx_quality_chart, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_set_style_size(rx_quality_chart, 0, LV_PART_INDICATOR);
    lv_chart_set_div_line_count(rx_quality_chart, 5, 8);
    lv_chart_set_point_count(rx_quality_chart, 48);
    lv_chart_set_axis_tick(rx_quality_chart, LV_CHART_AXIS_PRIMARY_Y, 5, 3, 5, 2, true, 50);

    rssi0_curve = lv_chart_add_series(rx_quality_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    rssi1_curve = lv_chart_add_series(rx_quality_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    chart_fre_label = lv_label_create(chart_cont);
    lv_obj_set_style_text_font(chart_fre_label, &lv_font_montserrat_10, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(chart_fre_label, lv_color_make(255, 255, 255), LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(chart_fre_label, 255, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(chart_fre_label, "Signal quality");
    lv_obj_align_to(chart_fre_label, rx_quality_chart, LV_ALIGN_OUT_TOP_MID, 0, 0);

    scan_chart_timer = lv_timer_create(page_scan_chart_timer_event, scan_turn_time, NULL);

    lv_amin_start(chart_fre_label, 200, 10, 1, 300, 300, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    lv_amin_start(contKPaTemp, -70, -45, 1, 400, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    lv_amin_start(rx_quality_chart, -60, -15, 1, 500, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
    lv_amin_start(back_img, -80, -10, 1, 1200, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup()
{
    Serial.printf("Page Smart -------->\n");

    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    Back_Img_Create(appWindow);
    ContKPaTemp_Create(appWindow);
    labelHmiTemp_Create();
    Page_scan_chart_create(appWindow);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit()
{
    lv_amin_start(chart_fre_label,
                  lv_obj_get_y(chart_fre_label), 120,
                  1,
                  300,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    lv_amin_start(contKPaTemp,
                  lv_obj_get_y(contKPaTemp), -70,
                  1,
                  300,
                  100,
                  (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    lv_amin_start(rx_quality_chart,
                  lv_obj_get_y(rx_quality_chart), 60,
                  1,
                  500,
                  200,
                  (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    lv_amin_start(back_img,
                  lv_obj_get_y(back_img), -80,
                  1,
                  1200,
                  300,
                  (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    if (scan_chart_timer)
        lv_timer_del(scan_chart_timer);

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