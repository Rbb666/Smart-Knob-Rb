#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

#include "spectrum_11.h"

#include <Hal/motor.h>
#include <Hal/Ble_Keyboard.h>

PAGE_EXPORT(Music);

#define ACTIVE_TRACK_CNT 3
#define INTRO_TIME 800
#define BAR_COLOR1 lv_color_hex(0xe9dbfc)
#define BAR_COLOR2 lv_color_hex(0x6f8af6)
#define BAR_COLOR3 lv_color_hex(0xffffff)
#define BAR_COLOR1_STOP 80
#define BAR_COLOR2_STOP 100
#define BAR_COLOR3_STOP (2 * LV_HOR_RES / 3)
#define BAR_CNT 20
#define DEG_STEP (180 / BAR_CNT)
#define BAND_CNT 4
#define BAR_PER_BAND_CNT (BAR_CNT / BAND_CNT)

static lv_obj_t *img_bg;
static lv_obj_t *gif_img;
static lv_obj_t *scroll_cont;
static lv_obj_t *lmeter;
static lv_obj_t *prev_btn;
static lv_obj_t *next_btn;
static lv_obj_t *play_btn;
static lv_obj_t *pause_btn;
static lv_obj_t *exit_btn;

static lv_meter_indicator_t *indic;
static lv_timer_t *timer_float;
static lv_timer_t *timer_display;

static lv_obj_t *main_cont;
static lv_obj_t *spectrum_obj;
static lv_obj_t *music_img_obj;
static uint32_t spectrum_i = 0;
static uint32_t spectrum_i_pause = 0;
static uint32_t bar_ofs = 0;
static uint32_t spectrum_lane_ofs_start = 0;
static uint32_t bar_rot = 0;
static const lv_font_t *font_small;
static const lv_font_t *font_large;
static bool start_anim;
static bool playing;
static lv_coord_t start_anim_values[40];
static const uint16_t (*spectrum)[4];
static uint32_t spectrum_len;
static const uint16_t rnd_array[30] = {994, 285, 553, 11, 792, 707, 966, 641, 852, 827, 44, 352, 146, 581, 490, 80, 729,
                                       58, 695, 940, 724, 561, 124, 653, 27, 292, 557, 506, 382, 199};

static lv_timer_t *spect_timer;

static void Music_view_create(lv_obj_t *win);

static lv_obj_t *Music_meter_create(lv_obj_t *win);

static void spectrum_anim_create(lv_obj_t *win);

static void Music_btn_create(lv_obj_t *win);

void lv_music_resume(void);
extern Ble_Interface ble_dev;

static void onTimer(lv_timer_t *timer)
{
    if (timer == timer_float)
    {
        lv_amin_start(scroll_cont, lv_obj_get_y(scroll_cont), 140,
                      1, 300, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
        lv_obj_del_delayed(scroll_cont, 100);
        scroll_cont = nullptr;
    }
    if (timer == spect_timer)
    {
        lv_music_resume();
    }
}

static void onTimer_display(lv_timer_t *timer)
{
    if (timer == timer_display)
    {
        Music_view_create(appWindow);
        spectrum_anim_create(appWindow);
    }
}

static void button_callback(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_PRESSED)
    {
        if (btn == prev_btn)
        {
            ble_dev.keyboard_player_prev();
        }
        else if (btn == next_btn)
        {
            ble_dev.keyboard_player_next();
        }
        else if (btn == play_btn)
        {
            ble_dev.keyboard_player_pause_play();
        }
        else if (btn == pause_btn)
        {
            ble_dev.keyboard_player_pause_play();
        }
        else
        {
            Page->Pop();
        }
    }
}

// 暂停图片
static void Pause_Img_Create(void)
{
    LV_IMG_DECLARE(IMG_Pause_MC);

    /*Create image*/
    lv_obj_t *pause_img = lv_img_create(pause_btn);
    lv_img_set_src(pause_img, &IMG_Pause_MC);
    lv_obj_align(pause_img, LV_ALIGN_CENTER, 0, 0);
}

// 播放图片
static void Play_Img_Create(void)
{
    LV_IMG_DECLARE(IMG_Play_Mc);

    /*Create image*/
    lv_obj_t *play_img = lv_img_create(play_btn);
    lv_img_set_src(play_img, &IMG_Play_Mc);
    lv_obj_align(play_img, LV_ALIGN_CENTER, 0, 0);
}

// 上一首图片
static void Prev_Img_Create(void)
{
    LV_IMG_DECLARE(IMG_Prev_Mc);

    /*Create image*/
    lv_obj_t *prev_img = lv_img_create(prev_btn);
    lv_img_set_src(prev_img, &IMG_Prev_Mc);
    lv_obj_align(prev_img, LV_ALIGN_CENTER, 0, 0);
}

// 下一首图片
static void Next_Img_Create(void)
{
    LV_IMG_DECLARE(IMG_Next_Mc);

    /*Create image*/
    lv_obj_t *next_img = lv_img_create(next_btn);
    lv_img_set_src(next_img, &IMG_Next_Mc);
    lv_obj_align(next_img, LV_ALIGN_CENTER, 0, 0);
}

static void Music_btn_create(lv_obj_t *win)
{
    pause_btn = lv_btn_create(win);
    lv_obj_set_size(pause_btn, 30, 30);
    lv_obj_align(pause_btn, LV_ALIGN_TOP_MID, 0, 0);
    extern void button_style_create(lv_obj_t * obj);
    button_style_create(pause_btn);
    lv_obj_set_style_bg_color(pause_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(pause_btn, button_callback, LV_EVENT_ALL, 0);
    Pause_Img_Create();
    lv_amin_start(pause_btn, -30, 42,
                  1, 200, 200, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    play_btn = lv_btn_create(win);
    lv_obj_set_size(play_btn, 30, 30);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    button_style_create(play_btn);
    lv_obj_set_style_bg_color(play_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(play_btn, button_callback, LV_EVENT_ALL, 0);
    Play_Img_Create();
    lv_amin_start(play_btn, 30, -42,
                  1, 400, 200, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    next_btn = lv_btn_create(win);
    lv_obj_set_size(next_btn, 30, 30);
    lv_obj_align(next_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    button_style_create(next_btn);
    lv_obj_set_style_bg_color(next_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(next_btn, button_callback, LV_EVENT_ALL, 0);
    Next_Img_Create();
    lv_amin_start(next_btn, 30, -42,
                  1, 600, 200, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_bounce);

    prev_btn = lv_btn_create(win);
    lv_obj_set_size(prev_btn, 30, 30);
    lv_obj_align(prev_btn, LV_ALIGN_LEFT_MID, 0, 0);
    button_style_create(prev_btn);
    lv_obj_set_style_bg_color(prev_btn, lv_color_white(), LV_STATE_DEFAULT);

    lv_obj_add_event_cb(prev_btn, button_callback, LV_EVENT_ALL, 0);
    Prev_Img_Create();
    lv_amin_start(prev_btn, -30, 42,
                  1, 800, 200, (lv_anim_exec_xcb_t)lv_obj_set_x, lv_anim_path_bounce);

    exit_btn = lv_btn_create(win);
    lv_obj_set_size(exit_btn, 45, 45);
    lv_obj_align(exit_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_opa(exit_btn, LV_OPA_TRANSP, 0);
    lv_obj_add_event_cb(exit_btn, button_callback, LV_EVENT_ALL, 0);
}

static lv_obj_t *create_cont(lv_obj_t *parent)
{
    /*A transparent container in which the player section will be scrolled*/
    main_cont = lv_obj_create(parent);
    lv_obj_clear_flag(main_cont, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(main_cont, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_remove_style_all(main_cont); /*Make it transparent*/
    lv_obj_set_size(main_cont, lv_pct(100), lv_pct(100));
    lv_obj_set_scroll_snap_y(main_cont, LV_SCROLL_SNAP_CENTER); /*Snap the children to the center*/
    lv_obj_set_style_bg_opa(main_cont, LV_OPA_0, 0);

    /*Create a container for the player*/
    lv_obj_t *player = lv_obj_create(main_cont);
    lv_obj_set_y(player, -20);
    lv_obj_set_size(player, LV_HOR_RES, LV_VER_RES + 20 * 2);
    lv_obj_clear_flag(player, LV_OBJ_FLAG_SNAPABLE);

    lv_obj_set_style_border_width(player, 0, 0);
    lv_obj_set_scroll_dir(player, LV_DIR_VER);
    lv_obj_update_layout(main_cont);

    return player;
}

int32_t get_cos(int32_t deg, int32_t a)
{
    int32_t r = (lv_trigo_cos(deg) * a);

    r += LV_TRIGO_SIN_MAX / 2;
    return r >> LV_TRIGO_SHIFT;
}

int32_t get_sin(int32_t deg, int32_t a)
{
    int32_t r = lv_trigo_sin(deg) * a;

    r += LV_TRIGO_SIN_MAX / 2;
    return r >> LV_TRIGO_SHIFT;
}

static void spectrum_draw_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_REFR_EXT_DRAW_SIZE)
    {
        lv_event_set_ext_draw_size(e, LV_VER_RES);
    }
    else if (code == LV_EVENT_COVER_CHECK)
    {
        lv_event_set_cover_res(e, LV_COVER_RES_NOT_COVER);
    }
    else if (code == LV_EVENT_DRAW_POST)
    {
        lv_obj_t *obj = lv_event_get_target(e);
        lv_draw_ctx_t *draw_ctx = lv_event_get_draw_ctx(e);

        lv_opa_t opa = lv_obj_get_style_opa(obj, LV_PART_MAIN);
        if (opa < LV_OPA_MIN)
            return;

        lv_point_t poly[4];
        lv_point_t center;
        center.x = obj->coords.x1 + lv_obj_get_width(obj) / 2;
        center.y = obj->coords.y1 + lv_obj_get_height(obj) / 2;

        lv_draw_rect_dsc_t draw_dsc;
        lv_draw_rect_dsc_init(&draw_dsc);
        draw_dsc.bg_opa = LV_OPA_90;

        uint16_t r[64];
        uint32_t i;

        lv_coord_t min_a = 5;
        lv_coord_t r_in = 77;
        r_in = (r_in * lv_obj_get_style_transform_zoom(music_img_obj, 0)) >> 8;

        for (i = 0; i < BAR_CNT; i++)
            r[i] = r_in + min_a;

        uint32_t s;
        for (s = 0; s < 4; s++)
        {
            uint32_t f;
            uint32_t band_w = 0; /*Real number of bars in this band.*/
            switch (s)
            {
            case 0:
                band_w = 20;
                break;
            case 1:
                band_w = 8;
                break;
            case 2:
                band_w = 4;
                break;
            case 3:
                band_w = 2;
                break;
            }

            /* Add "side bars" with cosine characteristic.*/
            for (f = 0; f < band_w; f++)
            {
                uint32_t ampl_main = spectrum[spectrum_i][s];
                int32_t ampl_mod = get_cos(f * 360 / band_w + 180, 180) + 180;
                int32_t t = BAR_PER_BAND_CNT * s - band_w / 2 + f;
                if (t < 0)
                    t = BAR_CNT + t;
                if (t >= BAR_CNT)
                    t = t - BAR_CNT;
                r[t] += (ampl_main * ampl_mod) >> 9;
            }
        }

        uint32_t amax = 20;
        int32_t animv = spectrum_i - spectrum_lane_ofs_start;
        if (animv > amax)
            animv = amax;
        for (i = 0; i < BAR_CNT; i++)
        {
            uint32_t deg_space = 1;
            uint32_t deg = i * DEG_STEP + 90;
            uint32_t j = (i + bar_rot + rnd_array[bar_ofs % 10]) % BAR_CNT;
            uint32_t k = (i + bar_rot + rnd_array[(bar_ofs + 1) % 10]) % BAR_CNT;

            uint32_t v = (r[k] * animv + r[j] * (amax - animv)) / amax;
            if (start_anim)
            {
                v = r_in + start_anim_values[i];
                deg_space = v >> 7;
                if (deg_space < 1)
                    deg_space = 1;
            }

            if (v < BAR_COLOR1_STOP)
                draw_dsc.bg_color = BAR_COLOR1;
            else if (v > BAR_COLOR3_STOP)
            {
                draw_dsc.bg_opa = LV_OPA_0;
                //                draw_dsc.bg_color = BAR_COLOR3;
            }
            else if (v > BAR_COLOR2_STOP)
                draw_dsc.bg_color = lv_color_mix(BAR_COLOR3, BAR_COLOR2, ((v - BAR_COLOR2_STOP) * 255) / (BAR_COLOR3_STOP - BAR_COLOR2_STOP));
            else
                draw_dsc.bg_color = lv_color_mix(BAR_COLOR2, BAR_COLOR1,
                                                 ((v - BAR_COLOR1_STOP) * 255) / (BAR_COLOR2_STOP - BAR_COLOR1_STOP));

            uint32_t di = deg + deg_space;

            int32_t x1_out = get_cos(di, v);
            poly[0].x = center.x + x1_out;
            poly[0].y = center.y + get_sin(di, v);

            int32_t x1_in = get_cos(di, r_in);
            poly[1].x = center.x + x1_in;
            poly[1].y = center.y + get_sin(di, r_in);
            di += DEG_STEP - deg_space * 2;

            int32_t x2_in = get_cos(di, r_in);
            poly[2].x = center.x + x2_in;
            poly[2].y = center.y + get_sin(di, r_in);

            int32_t x2_out = get_cos(di, v);
            poly[3].x = center.x + x2_out;
            poly[3].y = center.y + get_sin(di, v);

            lv_draw_polygon(draw_ctx, &draw_dsc, poly, 4);

            poly[0].x = center.x - x1_out;
            poly[1].x = center.x - x1_in;
            poly[2].x = center.x - x2_in;
            poly[3].x = center.x - x2_out;
            lv_draw_polygon(draw_ctx, &draw_dsc, poly, 4);
        }
    }
}

/**
 * @brief  播放器频谱+容器
 * @param  parent
 * @retval 无
 */
static lv_obj_t *music_img_create(lv_obj_t *parent)
{
    spectrum = spectrum_11;
    spectrum_len = sizeof(spectrum_11) / sizeof(spectrum_11[0]);

    lv_obj_t *meter = Music_meter_create(parent);
    Music_btn_create(appWindow);

    return meter;
}

static lv_obj_t *create_spectrum_obj(lv_obj_t *parent)
{
    /*Create the spectrum visualizer*/
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, 250, 250);
    lv_obj_align(obj, LV_ALIGN_CENTER, -15, -15);
    lv_obj_set_style_bg_opa(obj, LV_OPA_0, 0);

    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(obj, spectrum_draw_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_refresh_ext_draw_size(obj);
    music_img_obj = music_img_create(obj);
    return obj;
}

static void start_anim_cb(void *a, int32_t v)
{
    lv_coord_t *av = (lv_coord_t *)a;
    *av = v;
    lv_obj_invalidate(spectrum_obj);
}

static void spectrum_anim_cb(void *a, int32_t v)
{
    lv_obj_t *obj = (lv_obj_t *)a;
    if (start_anim)
    {
        lv_obj_invalidate(obj);
        return;
    }

    spectrum_i = v;
    lv_obj_invalidate(obj);

    static uint32_t bass_cnt = 0;
    static int32_t last_bass = -1000;
    static int32_t dir = 1;
    if (spectrum[spectrum_i][0] > 12)
    {
        if (spectrum_i - last_bass > 5)
        {
            bass_cnt++;
            last_bass = spectrum_i;
            if (bass_cnt >= 2)
            {
                bass_cnt = 0;
                spectrum_lane_ofs_start = spectrum_i;
                bar_ofs++;
            }
        }
    }
    if (spectrum[spectrum_i][0] < 4)
        bar_rot += dir;

    lv_obj_set_style_transform_zoom(music_img_obj, LV_IMG_ZOOM_NONE + spectrum[spectrum_i][0], 0);
}

void lv_music_resume(void)
{
    playing = true;
    spectrum_i = spectrum_i_pause;
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_values(&a, spectrum_i, spectrum_len - 1);
    lv_anim_set_exec_cb(&a, spectrum_anim_cb);
    lv_anim_set_var(&a, spectrum_obj);
    lv_anim_set_time(&a, ((spectrum_len - spectrum_i) * 1000) / 30);
    lv_anim_set_playback_time(&a, 0);
    lv_anim_start(&a);
}

static void stop_start_anim(lv_timer_t *t)
{
    LV_UNUSED(t);
    start_anim = false;
    lv_obj_refresh_ext_draw_size(spectrum_obj);
}

static void lv_music_pause(void)
{
    playing = false;
    spectrum_i_pause = spectrum_i;
    spectrum_i = 0;
    lv_anim_del(spectrum_obj, spectrum_anim_cb);
    lv_obj_invalidate(spectrum_obj);
}

static void Music_view_create(lv_obj_t *win)
{
    lv_obj_set_style_bg_opa(win, LV_OPA_0, LV_STATE_DEFAULT);
    LV_IMG_DECLARE(IMG_Water_BG);
    lv_obj_set_scrollbar_mode(win, LV_SCROLLBAR_MODE_OFF);

    img_bg = lv_img_create(win);
    lv_img_set_src(img_bg, &IMG_Water_BG);
    lv_obj_align(img_bg, LV_ALIGN_CENTER, 0, 0);
}

static void spectrum_anim_create(lv_obj_t *win)
{
    lv_obj_t *cont = create_cont(win);
    lv_obj_set_style_bg_opa(cont, LV_OPA_0, 0);
    spectrum_obj = create_spectrum_obj(cont);

    start_anim = true;

    // 开始播放！！
    lv_timer_t *timer = lv_timer_create(stop_start_anim, INTRO_TIME, nullptr);
    lv_timer_set_repeat_count(timer, 1);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_path_cb(&a, lv_anim_path_bounce);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)start_anim_cb);
    lv_anim_set_values(&a, LV_HOR_RES, 5);
    lv_anim_set_var(&a, &start_anim_values);
    lv_anim_set_delay(&a, 0);
    lv_anim_set_time(&a, 0);
    lv_anim_start(&a);
    lv_music_resume();
}

#if LV_USE_GIF

static void Gif_create(lv_obj_t *win)
{
    scroll_cont = lv_obj_create(win);
    lv_obj_set_size(scroll_cont, 140, 120);
    lv_obj_set_style_bg_color(scroll_cont, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(scroll_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scroll_cont, 0, 0);
    lv_obj_set_style_radius(scroll_cont, 10, LV_STATE_DEFAULT);
    lv_obj_align(scroll_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    LV_IMG_DECLARE(IMG_Music_GIF);
    gif_img = lv_gif_create(scroll_cont);
    lv_gif_set_src(gif_img, &IMG_Music_GIF);
    lv_obj_align(gif_img, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(scroll_cont, 120, -10,
                  1, 500, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

#endif

static void set_value(void *indic, int32_t v)
{
    lv_meter_set_indicator_end_value(lmeter, (lv_meter_indicator_t *)indic, v);
}

// 播放器表盘
static lv_obj_t *Music_meter_create(lv_obj_t *win)
{
    lmeter = lv_meter_create(win);
    /*Remove the circle from the middle*/
    lv_obj_remove_style(lmeter, nullptr, LV_PART_ITEMS);
    lv_obj_remove_style(lmeter, nullptr, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(lmeter, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(lmeter, 170, 170);
    lv_obj_set_opa_scale(lmeter, LV_OPA_TRANSP);
    lv_obj_align(lmeter, LV_ALIGN_CENTER, 0, 0);

    lv_meter_scale_t *scale = lv_meter_add_scale(lmeter);
    lv_meter_set_scale_ticks(lmeter, scale, 0, 0, 45, lv_color_black());
    lv_meter_set_scale_range(lmeter, scale, 0, 100, 360, 180);

    indic = lv_meter_add_arc(lmeter, scale, 45, lv_color_white(), 0);

    /*Create an animation to set the value*/
    lv_amin_start(indic, 0, 100,
                  1, 400, 0, (lv_anim_exec_xcb_t)set_value, lv_anim_path_bounce);
    return lmeter;
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

#if LV_USE_GIF
    Music_view_create(appWindow);

    Gif_create(appWindow);

    timer_float = lv_timer_create(onTimer, 4500, NULL);
    lv_timer_set_repeat_count(timer_float, 1);

    timer_display = lv_timer_create(onTimer_display, 4800, NULL);
    lv_timer_set_repeat_count(timer_display, 1);
#else
    Music_view_create(appWindow);
    spectrum_anim_create(appWindow);
#endif
    spect_timer = lv_timer_create(onTimer, 15 * 1000, NULL);
}

/**
 * @brief  页面退出事件
 * @param  无
 * @retval 无
 */
static void Exit()
{

    lv_music_pause();

    lv_amin_start(indic,
                  100, 0,
                  1,
                  400,
                  0,
                  (lv_anim_exec_xcb_t)set_value,
                  lv_anim_path_bounce);

    lv_amin_start(exit_btn,
                  lv_obj_get_x(exit_btn), 120,
                  0,
                  200,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(pause_btn,
                  lv_obj_get_y(pause_btn), -40,
                  0,
                  300,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(play_btn,
                  lv_obj_get_y(play_btn), 40,
                  0,
                  400,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_y,
                  lv_anim_path_bounce);

    lv_amin_start(prev_btn,
                  lv_obj_get_x(prev_btn), -40,
                  0,
                  500,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_x,
                  lv_anim_path_bounce);

    lv_amin_start(next_btn,
                  lv_obj_get_x(next_btn), 40,
                  0,
                  600,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_x,
                  lv_anim_path_bounce);

    if (spect_timer)
        lv_timer_del(spect_timer);

    PageDelay(600);
    lv_obj_clean(appWindow);
}

/**
 * @brief  页面事件
 * @param  obj:发生事件的对象
 * @param  event:事件编号
 * @retval 无
 */
static void Event(void *obj, uint8_t event) {}