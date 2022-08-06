#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

#include <Hal/motor.h>

PAGE_EXPORT(Switch);

static lv_obj_t *meter;
static lv_obj_t *button;
static lv_style_t btn_style;
static lv_style_t style_pr;
static lv_meter_indicator_t *indic1 = NULL;

static lv_timer_t *timer = NULL;

static void onTimer(lv_timer_t *tmr)
{
    if (tmr == timer)
    {
        static int now_num = 0;
        static int old_num = 0;
        now_num = get_motor_position();

        if (now_num > old_num)
        {
            Serial.println("1");
            lv_event_send(button, LV_EVENT_PRESSED, NULL);
            old_num = get_motor_position();
            lv_event_send(button, LV_EVENT_RELEASED, NULL);
        }
        else if (now_num < old_num)
        {
            Serial.println("2");
            lv_event_send(button, LV_EVENT_PRESSED, NULL);
            old_num = get_motor_position();
            lv_event_send(button, LV_EVENT_RELEASED, NULL);
        }
    }
}

static void button_callback(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        static boolean btn_flg = false;

        if (btn == button && btn_flg == false)
        {
            lv_obj_set_style_bg_color(button, lv_palette_main(LV_PALETTE_GREY), 0);
            btn_flg = !btn_flg;
        }
        else if (btn == button && btn_flg == true)
        {
            lv_obj_set_style_bg_color(button, lv_color_white(), 0);
            btn_flg = !btn_flg;
        }
    }
}

static void set_value(lv_meter_indicator_t *indic, int32_t v)
{
    lv_meter_set_indicator_end_value(meter, indic, v);
}

static void btn_style_create(void)
{
    lv_style_init(&btn_style);

    lv_style_set_radius(&btn_style, 3);

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
    lv_style_set_outline_width(&style_pr, 30);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

    lv_style_set_translate_y(&style_pr, 3);
    lv_style_set_shadow_ofs_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 2));
    lv_style_set_bg_grad_color(&style_pr, lv_palette_darken(LV_PALETTE_GREY, 4));

    /*Add a transition to the the outline*/
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, LV_STYLE_PROP_INV};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 500, 0, NULL);

    lv_style_set_transition(&style_pr, &trans);
}

static void anim_size_cb(_lv_obj_t *var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

// 播放器表盘
static void sw_meter_create(lv_obj_t *win)
{
    lv_obj_set_style_bg_color(win, lv_color_make(225, 233, 236), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(win, lv_color_make(227, 216, 211), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(win, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(win, 200, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(win, 255, LV_STATE_DEFAULT);

    lv_obj_t *circle_bg = lv_obj_create(win);
    lv_obj_set_style_bg_color(circle_bg, lv_color_make(225, 234, 239), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(circle_bg, 2, LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_width(circle_bg, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(circle_bg, lv_palette_darken(LV_PALETTE_GREY, 2), LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(circle_bg, 4, LV_STATE_DEFAULT);

    lv_obj_set_style_radius(circle_bg, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(circle_bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *circle_font = lv_obj_create(circle_bg);
    lv_obj_set_style_bg_color(circle_font, lv_color_make(223, 232, 239), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(circle_font, 1, LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(circle_font, lv_color_make(204, 204, 204), LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_width(circle_font, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(circle_font, lv_color_make(175, 189, 192), LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(circle_font, 5, LV_STATE_DEFAULT);

    lv_obj_set_style_bg_grad_color(circle_font, lv_color_make(216, 223, 229), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(circle_font, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(circle_font, 150, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(circle_font, 255, LV_STATE_DEFAULT);

    lv_obj_set_style_radius(circle_font, LV_RADIUS_CIRCLE, 0);
    lv_obj_align(circle_font, LV_ALIGN_CENTER, 0, 0);

    btn_style_create();
    button = lv_btn_create(circle_font);
    lv_obj_remove_style_all(button);
    lv_obj_add_style(button, &btn_style, 0);
    lv_obj_add_style(button, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_style_radius(button, 35, LV_STATE_DEFAULT);
    lv_obj_set_size(button, 70, 70);
    lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_center(button);
    lv_obj_align(button, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(button, button_callback, LV_EVENT_ALL, NULL);

    LV_IMG_DECLARE(IMG_SW);
    /*Create image*/
    lv_obj_t *sw_img = lv_img_create(button);
    lv_img_set_src(sw_img, &IMG_SW);
    lv_obj_align(sw_img, LV_ALIGN_CENTER, 0, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, circle_bg);
    lv_anim_set_values(&a, 10, 180);
    lv_anim_set_time(&a, 300);
    lv_anim_set_repeat_delay(&a, 0);
    lv_anim_set_repeat_count(&a, 1);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_size_cb);
    lv_anim_start(&a);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup()
{
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    sw_meter_create(appWindow);

    timer = lv_timer_create(onTimer, 100, NULL);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit()
{
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