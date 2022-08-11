#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(OTA);

static lv_style_t style_btn;
static lv_style_t style_pr;
static lv_style_t style_fd;
static lv_style_t style_def;

static lv_obj_t *wifi_img_cont;
static lv_obj_t *wifi_button;

static void onTimer(lv_timer_t *timer)
{
    Page->Pop();
}

static void button_style_init(void)
{
    static lv_style_prop_t props[] = {
        LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, LV_STYLE_PROP_INV};

    static lv_style_transition_dsc_t transition_dsc_def;
    static lv_style_transition_dsc_t transition_dsc_pr;

    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);
    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, -10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    lv_style_init(&style_fd);
    lv_style_set_border_color(&style_fd, lv_color_make(127, 127, 127));

    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 5);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_make(127, 127, 127));
    lv_style_set_bg_color(&style_btn, lv_color_white());
}

static void wifi_ui_create(lv_obj_t *win)
{
    wifi_img_cont = lv_obj_create(win);
    lv_obj_set_scrollbar_mode(wifi_img_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(wifi_img_cont, 60, 60);
    lv_obj_set_style_bg_opa(wifi_img_cont, 0, 0);
    lv_obj_set_style_border_width(wifi_img_cont, 3, 0);
    lv_obj_set_style_bg_color(wifi_img_cont, lv_color_make(215, 215, 215), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(wifi_img_cont, 10, 0);
    lv_obj_align(wifi_img_cont, LV_ALIGN_LEFT_MID, 40, 0);

    LV_IMG_DECLARE(IMG_WIFI);
    /*Create image*/
    lv_obj_t *wifi_img = lv_img_create(wifi_img_cont);
    lv_img_set_src(wifi_img, &IMG_WIFI);
    lv_obj_align(wifi_img, LV_ALIGN_CENTER, 0, 0);
}

static void wifi_btn_create(lv_obj_t *win)
{
    wifi_button = lv_btn_create(win);
    lv_obj_set_size(wifi_button, 65, 35);

    lv_obj_add_style(wifi_button, &style_btn, LV_PART_MAIN);
    lv_obj_add_style(wifi_button, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(wifi_button, &style_fd, LV_STATE_FOCUSED);
    
    lv_obj_add_style(wifi_button, &style_def, 0);
    // lv_obj_add_event_cb(wifi_button, button_event_cb, LV_EVENT_ALL, 0);
    // lv_obj_set_user_data(wifi_button, user_data);
    lv_obj_align(wifi_button, LV_ALIGN_CENTER, 40, 0);

    LV_FONT_DECLARE(HandGotn_20);
    static lv_style_t style_label_public;
    lv_style_set_text_color(&style_label_public, lv_color_black());
    lv_style_set_text_font(&style_label_public, &HandGotn_20);

    lv_obj_t *label = lv_label_create(wifi_button);
    lv_obj_add_style(label, &style_label_public, LV_PART_MAIN);
    lv_label_set_text(label, "Wi-Fi");
    lv_obj_center(label);
}

static void wifi_ip_label_create(lv_obj_t *win)
{
    LV_FONT_DECLARE(HandGotn_14);
    static lv_style_t style_label_public;
    lv_style_set_text_color(&style_label_public, lv_color_make(51, 51, 51));
    lv_style_set_text_font(&style_label_public, &HandGotn_14);

    lv_obj_t *label = lv_label_create(win);
    lv_obj_add_style(label, &style_label_public, LV_PART_MAIN);
    lv_label_set_text(label, "192.168.122.1");
    lv_obj_center(label);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);
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

    button_style_init();
    wifi_ui_create(appWindow);
    wifi_btn_create(appWindow);
    wifi_ip_label_create(appWindow);
    // timer2 = lv_timer_create(onTimer, 2000, NULL);
    // lv_timer_set_repeat_count(timer2, 1);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit()
{
    lv_obj_clean(appWindow);
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