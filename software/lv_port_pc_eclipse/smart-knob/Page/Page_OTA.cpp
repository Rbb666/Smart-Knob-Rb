#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(OTA);

extern "C"
{
LV_IMG_DECLARE(lock_16px) ;
LV_IMG_DECLARE(unlock_16px) ;
}

const char *wifi_ssid[5] = {
        "501",
        "CUCC",
        "NIHAOZBR",
        "15542415323",
        "501@5G"
};

static lv_style_t style_btn;
static lv_style_t style_pr;
static lv_style_t style_fd;
static lv_style_t style_def;

static lv_obj_t *img_disconn;
static lv_obj_t *line;
static lv_obj_t *wifi_info_cont;
static lv_obj_t *wifi_scan_cont;
static lv_obj_t *wifi_img_cont;
static lv_obj_t *wifi_button;
static lv_obj_t *wifi_conn_button;
static lv_obj_t *wifi_ap_ota_btn;
static lv_obj_t *wifi_sta_ota_btn;
static lv_obj_t *wifi_exit_button;

static lv_timer_t *conn_wifi_timer;
static lv_anim_timeline_t *anim_timeline;

static void wifi_function_list(lv_obj_t *win);

static void wifi_scan_list(lv_obj_t *win);

static void add_wifi_scan_list(void);

static void show_info_connect_wifi(lv_obj_t *win);

static void onTimer(lv_timer_t *timer) {
    // 自动弹出 -- 连接信息
    if (timer == conn_wifi_timer) {
        show_info_connect_wifi(appWindow);
    }
}

static void button_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED) {
        if (btn == wifi_button) {
            wifi_function_list(appWindow);
            lv_obj_del(wifi_button);
            wifi_button = nullptr;
        } else if (btn == wifi_conn_button) {
            wifi_scan_list(appWindow);
            add_wifi_scan_list();
        }
    }
}

static void disconnect_icon_create(lv_obj_t *win) {
    LV_IMG_DECLARE(IMG_Disconnect);
    /*Create image*/
    img_disconn = lv_img_create(win);
    lv_img_set_src(img_disconn, &IMG_Disconnect);
    lv_obj_align(img_disconn, LV_ALIGN_TOP_MID, 0, 0);

    lv_amin_start(img_disconn, -20, 0,
                  1, 500, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

static void wifi_function_list(lv_obj_t *win) {
    lv_obj_set_scrollbar_mode(win, LV_SCROLLBAR_MODE_OFF);
    /*Create line style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, lv_color_hex(0xff0000));
    line = lv_line_create(win);
    static lv_point_t line_points[] = {{115, 0},
                                       {115, 135}};
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, LV_PART_MAIN);

    /*Create connect button*/
    wifi_conn_button = lv_btn_create(win);
    lv_obj_set_size(wifi_conn_button, 70, 25);

    lv_obj_set_style_bg_color(wifi_conn_button, lv_color_make(51, 51, 51), LV_STATE_DEFAULT);
    lv_obj_add_style(wifi_conn_button, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(wifi_conn_button, &style_def, 0);

    lv_obj_add_event_cb(wifi_conn_button, button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_align(wifi_conn_button, LV_ALIGN_TOP_RIGHT, -15, 42);

    LV_FONT_DECLARE(HandGotn_14);
    lv_obj_t *label_conn_btn = lv_label_create(wifi_conn_button);
    lv_obj_set_style_text_color(label_conn_btn, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_conn_btn, &HandGotn_14, 0);
    lv_label_set_text(label_conn_btn, "Wi-Fi");
    lv_obj_center(label_conn_btn);

    /*Create ota button*/
    wifi_ap_ota_btn = lv_btn_create(win);
    lv_obj_set_size(wifi_ap_ota_btn, 70, 25);

    lv_obj_set_style_bg_color(wifi_ap_ota_btn, lv_color_make(51, 51, 51), LV_STATE_DEFAULT);
    lv_obj_add_style(wifi_ap_ota_btn, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(wifi_ap_ota_btn, &style_def, 0);

    lv_obj_add_event_cb(wifi_ap_ota_btn, button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_align(wifi_ap_ota_btn, LV_ALIGN_TOP_RIGHT, -15, 77);

    lv_obj_t *label_ap_ota_btn = lv_label_create(wifi_ap_ota_btn);
    lv_obj_set_style_text_color(label_ap_ota_btn, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_ap_ota_btn, &HandGotn_14, 0);
    lv_label_set_text(label_ap_ota_btn, "AP-OTA");
    lv_obj_center(label_ap_ota_btn);

    /*Create ota button*/
    wifi_sta_ota_btn = lv_btn_create(win);
    lv_obj_set_size(wifi_sta_ota_btn, 70, 25);

    lv_obj_set_style_bg_color(wifi_sta_ota_btn, lv_color_make(51, 51, 51), LV_STATE_DEFAULT);
    lv_obj_add_style(wifi_sta_ota_btn, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(wifi_sta_ota_btn, &style_def, 0);

    lv_obj_add_event_cb(wifi_sta_ota_btn, button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_align(wifi_sta_ota_btn, LV_ALIGN_TOP_RIGHT, -15, 112);

    lv_obj_t *label_sta_ota_btn = lv_label_create(wifi_sta_ota_btn);
    lv_obj_set_style_text_color(label_sta_ota_btn, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_sta_ota_btn, &HandGotn_14, 0);
    lv_label_set_text(label_sta_ota_btn, "STA-OTA");
    lv_obj_center(label_sta_ota_btn);

    /*Create exit button*/
    wifi_exit_button = lv_btn_create(win);
    lv_obj_set_size(wifi_exit_button, 70, 25);

    lv_obj_set_style_bg_color(wifi_exit_button, lv_color_make(51, 51, 51), LV_STATE_DEFAULT);
    lv_obj_add_style(wifi_exit_button, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(wifi_exit_button, &style_def, 0);

    lv_obj_add_event_cb(wifi_exit_button, button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_align(wifi_exit_button, LV_ALIGN_TOP_RIGHT, -15, 147);

    lv_obj_t *label_exit_btn = lv_label_create(wifi_exit_button);
    lv_obj_set_style_text_color(label_exit_btn, lv_color_white(), 0);
    lv_obj_set_style_text_font(label_exit_btn, &HandGotn_14, 0);
    lv_label_set_text(label_exit_btn, "EXIT");
    lv_obj_center(label_exit_btn);

    lv_amin_start(wifi_button, lv_obj_get_x(wifi_button), 65,
                  1, 200, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    lv_amin_start(line, -130, 40,
                  1, 300, 100, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);

    lv_amin_start(wifi_conn_button, 70, -15,
                  1, 400, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    lv_amin_start(wifi_ap_ota_btn, 70, -15,
                  1, 500, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    lv_amin_start(wifi_sta_ota_btn, -134, -15,
                  1, 600, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    lv_amin_start(wifi_exit_button, -134, -15,
                  1, 700, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

}

static void wifi_scan_list(lv_obj_t *win) {
    lv_obj_t *contTemp = lv_obj_create(win);
    lv_obj_set_size(contTemp, 80, 130);
    lv_obj_align(contTemp, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_scrollbar_mode(contTemp, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(contTemp, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(contTemp, 2, 0);
    lv_obj_set_style_border_color(contTemp, lv_color_white(), 0);
    lv_obj_set_style_radius(contTemp, 10, 0);

    wifi_scan_cont = lv_obj_create(contTemp);
    lv_obj_set_scrollbar_mode(wifi_scan_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(wifi_scan_cont, 75, 125);
    lv_obj_set_style_border_width(wifi_scan_cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(wifi_scan_cont, LV_OPA_COVER, 0);
    lv_obj_align(wifi_scan_cont, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(contTemp, 125, 0,
                  1, 300, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

static void add_wifi_scan_list(void) {
    for (int i = 0, duration = 100, ops_y = 0; i < 5; i++, duration += 100, ops_y += 30) {
        lv_obj_t *wifiname_list_btn = lv_list_add_btn(wifi_scan_cont, nullptr, wifi_ssid[i]);
        lv_obj_set_size(wifiname_list_btn, 75, 30);
        lv_obj_align(wifiname_list_btn, LV_ALIGN_TOP_MID, 0, ops_y);
        lv_obj_t *label = lv_obj_get_child(wifiname_list_btn, 0);
        lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
        lv_amin_start(wifiname_list_btn, 75, 0,
                      1, 500 + duration, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);
    }

    conn_wifi_timer = lv_timer_create(onTimer, 3000, NULL);
    lv_timer_set_repeat_count(conn_wifi_timer, 1);
}

static void button_style_init(void) {
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
    lv_style_set_radius(&style_btn, 8);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_make(127, 127, 127));
    lv_style_set_bg_color(&style_btn, lv_color_white());
}

static void wifi_ui_create(lv_obj_t *win) {
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

    lv_amin_start(wifi_img_cont, -60, 40,
                  1, 300, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);
}

static void wifi_btn_create(lv_obj_t *win) {
    wifi_button = lv_btn_create(win);
    lv_obj_set_size(wifi_button, 65, 35);

    lv_obj_add_style(wifi_button, &style_btn, LV_PART_MAIN);
    lv_obj_add_style(wifi_button, &style_pr, LV_STATE_PRESSED);

    lv_obj_add_style(wifi_button, &style_def, 0);
    lv_obj_add_event_cb(wifi_button, button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_align(wifi_button, LV_ALIGN_CENTER, 40, 0);

    LV_FONT_DECLARE(HandGotn_20);
    lv_obj_t *label = lv_label_create(wifi_button);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_style_text_font(label, &HandGotn_20, 0);
    lv_label_set_text(label, "Wi-Fi");
    lv_obj_center(label);

    lv_amin_start(wifi_button, 65, 40,
                  1, 300, 0, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);
}

static void wifi_ip_label_create(lv_obj_t *win) {
    LV_FONT_DECLARE(HandGotn_14);
    lv_obj_t *label = lv_label_create(win);
    lv_obj_set_style_text_color(label, lv_color_make(51, 51, 51), 0);
    lv_obj_set_style_text_font(label, &HandGotn_14, 0);
    lv_label_set_text(label, "192.168.122.1");
    lv_obj_center(label);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);
}

static void show_info_connect_wifi(lv_obj_t *win) {
    wifi_info_cont = lv_obj_create(win);
    lv_obj_set_scrollbar_mode(wifi_info_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(wifi_info_cont, 195, 140);
    lv_obj_align(wifi_info_cont, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_style_border_width(wifi_info_cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(wifi_info_cont, lv_color_make(216, 228, 229), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(wifi_info_cont, 12, LV_STATE_DEFAULT);

    LV_IMG_DECLARE(IMG_Return);
    /*Create image*/
    lv_obj_t *img = lv_img_create(wifi_info_cont);
    lv_img_set_src(img, &IMG_Return);
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, -5, -5);

    LV_FONT_DECLARE(HandGotn_20);
    lv_obj_t *info_label = lv_label_create(wifi_info_cont);
    lv_obj_set_size(info_label, 195, 140);
    lv_obj_set_style_text_font(info_label, &HandGotn_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(info_label, lv_color_make(85, 85, 85), LV_STATE_DEFAULT);
    lv_label_set_recolor(info_label, true);
    lv_label_set_long_mode(info_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(info_label, "\nPlease connect\n"
                                  "#000000 @rbb# and enter \n"
                                  "  #000000 192.168.121.103#");
//    lv_label_set_text(shadow_label, lv_label_get_text(info_label));

    /*Shift the second label down and to the right by 2 pixel*/
//    lv_obj_align_to(shadow_label, info_label, LV_ALIGN_TOP_LEFT, 1, 1);

    anim_timeline = lv_anim_timeline_create();
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    lv_anim_timeline_wrapper_t wrapper[] =
            {
                    ANIM_DEF(0, wifi_info_cont, width, 0, lv_obj_get_style_width(wifi_info_cont, 0)),
                    LV_ANIM_TIMELINE_WRAPPER_END};

    lv_anim_timeline_add_wrapper(anim_timeline, wrapper);

    lv_anim_timeline_start(anim_timeline);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup() {
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    disconnect_icon_create(appWindow);
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
static void Exit() {
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