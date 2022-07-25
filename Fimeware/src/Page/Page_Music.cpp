#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(Music);

static lv_obj_t *scroll_cont;

static lv_timer_t *timer = NULL;

static void onTimer(lv_timer_t *timer)
{
    lv_amin_start(scroll_cont, lv_obj_get_y(scroll_cont),
                  140, 1, 300, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    lv_obj_del_delayed(scroll_cont, 500);
}

static void Music_view_create(lv_obj_t *win)
{
    lv_obj_set_scrollbar_mode(win, LV_SCROLLBAR_MODE_OFF);

    scroll_cont = lv_obj_create(win);
    lv_obj_set_size(scroll_cont, 140, 120);
    lv_obj_set_style_bg_color(scroll_cont, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(scroll_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_border_width(scroll_cont, 0, 0);
    lv_obj_set_style_radius(scroll_cont, 10, LV_STATE_DEFAULT);
    lv_obj_align(scroll_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    LV_IMG_DECLARE(music_gif);
    lv_obj_t *img;

    img = lv_gif_create(scroll_cont);
    lv_gif_set_src(img, &music_gif);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    lv_amin_start(scroll_cont, 140, -10, 1, 500, 100, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
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

    Music_view_create(appWindow);

    timer = lv_timer_create(onTimer, 5000, NULL);
    lv_timer_set_repeat_count(timer, 1);
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