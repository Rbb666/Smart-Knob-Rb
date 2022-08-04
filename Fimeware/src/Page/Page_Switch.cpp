#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

PAGE_EXPORT(Switch);

static lv_obj_t *meter;
static lv_meter_indicator_t *indic1 = NULL;
static lv_meter_indicator_t *indic2 = NULL;

static lv_timer_t *timer2 = NULL;

static void onTimer(lv_timer_t *timer)
{
    Page->Pop();
}

static void set_value(lv_meter_indicator_t *indic, int32_t v)
{
    lv_meter_set_indicator_end_value(meter, indic, v);
}

// 播放器表盘
static void sw_meter_create(lv_obj_t *win)
{
    meter = lv_meter_create(win);
    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_ITEMS);
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(meter, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(meter, 180, 180);
    lv_obj_set_opa_scale(meter, LV_OPA_TRANSP);
    lv_obj_align(meter, LV_ALIGN_CENTER, 0, 0);

    lv_meter_scale_t *scale1 = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale1, 2, 2, 10, lv_color_white());
    lv_meter_set_scale_range(meter, scale1, -50, 50, 360, 0);

    // lv_meter_scale_t *scale2 = lv_meter_add_scale(meter);
    // lv_meter_set_scale_ticks(meter, scale2, 2, 2, 10, lv_color_white());
    // lv_meter_set_scale_range(meter, scale2, -50, 50, 360, 100);

    indic1 = lv_meter_add_arc(meter, scale1, 10, lv_color_white(), 10);
    indic2 = lv_meter_add_arc(meter, scale1, 10, lv_palette_main(LV_PALETTE_GREEN), -10);

    /*Create an animation to set the value*/
    // lv_amin_start(indic1, -50, 50, 1, 500, 0,
    //               (lv_anim_exec_xcb_t)set_value, lv_anim_path_bounce);
    lv_amin_start(indic2, 0, 50, 1, 500, 50,
                  (lv_anim_exec_xcb_t)set_value, lv_anim_path_bounce);
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