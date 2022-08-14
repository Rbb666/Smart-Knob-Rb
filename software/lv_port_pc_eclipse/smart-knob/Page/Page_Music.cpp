#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(Music);

static lv_obj_t *img_bg;
static lv_obj_t *gif_img;
static lv_obj_t *scroll_cont;
static lv_obj_t *main_cont;
static lv_obj_t *lmeter;
static lv_obj_t *prev_btn;
static lv_obj_t *next_btn;
static lv_obj_t *play_btn;
static lv_obj_t *pause_btn;
static lv_obj_t *exit_btn;
static lv_meter_indicator_t *indic = NULL;
static lv_timer_t *timer_float = NULL;
static lv_timer_t *timer_display = NULL;

static void Music_meter_create(lv_obj_t *win);

static void Music_btn_create(lv_obj_t *win);

static void onTimer(lv_timer_t *timer) {
    if (timer == timer_float) {
        lv_amin_start(scroll_cont, lv_obj_get_y(scroll_cont), 140,
                      1, 300, 0, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
    }
}

static void onTimer_display(lv_timer_t *timer) {
    if (timer == timer_display) {
        Music_meter_create(appWindow);
        Music_btn_create(appWindow);
    }
}

static void button_callback(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED) {
        if (btn == prev_btn) {
        } else if (btn == next_btn) {
        } else if (btn == play_btn) {
        } else if (btn == pause_btn) {
        } else {
            Page->Pop();
        }
    }
}

// 暂停图片
static void Pause_Img_Create(void) {
    LV_IMG_DECLARE(IMG_Pause_MC);

    /*Create image*/
    lv_obj_t *pause_img = lv_img_create(pause_btn);
    lv_img_set_src(pause_img, &IMG_Pause_MC);
    lv_obj_align(pause_img, LV_ALIGN_CENTER, 0, 0);
}

// 播放图片
static void Play_Img_Create(void) {
    LV_IMG_DECLARE(IMG_Play_Mc);

    /*Create image*/
    lv_obj_t *play_img = lv_img_create(play_btn);
    lv_img_set_src(play_img, &IMG_Play_Mc);
    lv_obj_align(play_img, LV_ALIGN_CENTER, 0, 0);
}

// 上一首图片
static void Prev_Img_Create(void) {
    LV_IMG_DECLARE(IMG_Prev_Mc);

    /*Create image*/
    lv_obj_t *prev_img = lv_img_create(prev_btn);
    lv_img_set_src(prev_img, &IMG_Prev_Mc);
    lv_obj_align(prev_img, LV_ALIGN_CENTER, 0, 0);
}

// 下一首图片
static void Next_Img_Create(void) {
    LV_IMG_DECLARE(IMG_Next_Mc);

    /*Create image*/
    lv_obj_t *next_img = lv_img_create(next_btn);
    lv_img_set_src(next_img, &IMG_Next_Mc);
    lv_obj_align(next_img, LV_ALIGN_CENTER, 0, 0);
}

static void Music_btn_create(lv_obj_t *win) {
    pause_btn = lv_btn_create(win);
    lv_obj_set_size(pause_btn, 30, 30);
    lv_obj_align(pause_btn, LV_ALIGN_TOP_MID, 0, 0);
    extern void button_style_create(lv_obj_t *obj);
    button_style_create(pause_btn);
    lv_obj_set_style_bg_color(pause_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(pause_btn, button_callback, LV_EVENT_ALL, 0);
    Pause_Img_Create();
    lv_amin_start(pause_btn, -30, 42, 1, 200, 200, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);

    play_btn = lv_btn_create(win);
    lv_obj_set_size(play_btn, 30, 30);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    button_style_create(play_btn);
    lv_obj_set_style_bg_color(play_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(play_btn, button_callback, LV_EVENT_ALL, 0);
    Play_Img_Create();
    lv_amin_start(play_btn, 30, -42, 1, 400, 200, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);

    next_btn = lv_btn_create(win);
    lv_obj_set_size(next_btn, 30, 30);
    lv_obj_align(next_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    button_style_create(next_btn);
    lv_obj_set_style_bg_color(next_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(next_btn, button_callback, LV_EVENT_ALL, 0);
    Next_Img_Create();
    lv_amin_start(next_btn, 30, -42, 1, 600, 200, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    prev_btn = lv_btn_create(win);
    lv_obj_set_size(prev_btn, 30, 30);
    lv_obj_align(prev_btn, LV_ALIGN_LEFT_MID, 0, 0);
    button_style_create(prev_btn);
    lv_obj_set_style_bg_color(prev_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(prev_btn, button_callback, LV_EVENT_ALL, 0);
    Prev_Img_Create();
    lv_amin_start(prev_btn, -30, 42, 1, 800, 200, (lv_anim_exec_xcb_t) lv_obj_set_x, lv_anim_path_bounce);

    exit_btn = lv_btn_create(win);
    lv_obj_set_size(exit_btn, 45, 45);
    lv_obj_align(exit_btn, LV_ALIGN_CENTER, 0, 0);
    button_style_create(exit_btn);
    lv_obj_set_style_bg_color(exit_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_set_style_radius(exit_btn, 35, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(exit_btn, button_callback, LV_EVENT_ALL, 0);
}

static void Music_view_create(lv_obj_t *win) {
    LV_IMG_DECLARE(IMG_Water_BG);
    // lv_obj_set_style_bg_color(win, lv_color_make(76, 15, 10), LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(win, LV_SCROLLBAR_MODE_OFF);

    img_bg = lv_img_create(win);
    lv_img_set_src(img_bg, &IMG_Water_BG);
    lv_obj_set_style_radius(img_bg, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_align(img_bg, LV_ALIGN_CENTER, 0, 0);
}

static void Gif_create(lv_obj_t *win) {
    scroll_cont = lv_obj_create(win);
    lv_obj_set_size(scroll_cont, 140, 120);
    lv_obj_set_style_bg_color(scroll_cont, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(scroll_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scroll_cont, 0, 0);
    lv_obj_set_style_radius(scroll_cont, 10, LV_STATE_DEFAULT);
    lv_obj_align(scroll_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    LV_IMG_DECLARE(music_gif);
    gif_img = lv_gif_create(scroll_cont);
    lv_gif_set_src(gif_img, &music_gif);
    lv_obj_align(gif_img, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(scroll_cont, 120, -10, 1, 500, 100, (lv_anim_exec_xcb_t) lv_obj_set_y, lv_anim_path_bounce);
}

static void set_value(lv_meter_indicator_t *indic, int32_t v) {
    lv_meter_set_indicator_end_value(lmeter, indic, v);
}

// 播放器表盘
static void Music_meter_create(lv_obj_t *win) {
    lmeter = lv_meter_create(win);
    /*Remove the circle from the middle*/
    lv_obj_remove_style(lmeter, NULL, LV_PART_ITEMS);
    lv_obj_remove_style(lmeter, NULL, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(lmeter, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(lmeter, 170, 170);
    lv_obj_set_opa_scale(lmeter, LV_OPA_TRANSP);
    lv_obj_align(lmeter, LV_ALIGN_CENTER, 0, 0);

    lv_meter_scale_t *scale = lv_meter_add_scale(lmeter);
    lv_meter_set_scale_ticks(lmeter, scale, 0, 0, 45, lv_color_black());
    lv_meter_set_scale_range(lmeter, scale, 0, 100, 360, 180);

    indic = lv_meter_add_arc(lmeter, scale, 45, lv_color_white(), 0);

    /*Create an animation to set the value*/
    lv_amin_start(indic, 0, 100, 1, 500, 0,
                  (lv_anim_exec_xcb_t) set_value, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup() {
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);

    Music_view_create(appWindow);

    Gif_create(appWindow);

    timer_float = lv_timer_create(onTimer, 4500, NULL);
    lv_timer_set_repeat_count(timer_float, 1);

    // Music_meter_create(appWindow);
    // Music_btn_create(appWindow);

    timer_display = lv_timer_create(onTimer_display, 5000, NULL);
    lv_timer_set_repeat_count(timer_display, 1);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit() {
    lv_amin_start(exit_btn,
                  lv_obj_get_x(exit_btn), 120,
                  0,
                  100,
                  0,
                  (lv_anim_exec_xcb_t) lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(indic,
                  100, 0,
                  1,
                  200,
                  0,
                  (lv_anim_exec_xcb_t) set_value,
                  lv_anim_path_bounce);

    lv_amin_start(pause_btn,
                  lv_obj_get_y(pause_btn), -40,
                  0,
                  300,
                  100,
                  (lv_anim_exec_xcb_t) lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(play_btn,
                  lv_obj_get_y(play_btn), 40,
                  0,
                  400,
                  200,
                  (lv_anim_exec_xcb_t) lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(prev_btn,
                  lv_obj_get_x(prev_btn), -40,
                  0,
                  500,
                  200,
                  (lv_anim_exec_xcb_t) lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(next_btn,
                  lv_obj_get_x(next_btn), 40,
                  0,
                  500,
                  200,
                  (lv_anim_exec_xcb_t) lv_obj_set_x,
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
static void Event(void *obj, uint8_t event) {}