#ifndef PAGE_ANIM_H
#define PAGE_ANIM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
    typedef struct
    {
        lv_obj_t *screen_main;

        lv_indev_t *indev_encoder; //编码器输入
        lv_group_t *defult_group;  //默认组
    } lv_ui;

    typedef enum
    {
        WELCOME_PAGE,
        IOT_MAIN_PAGE,
        IOT_POINTER_PAGE,
        IOT_SENSOR_PAGE,
        IOT_COMPUTER_PAGE,
        IOT_LIGHT_BELT_PAGE,
        IOT_SMART_FAN_PAGE,

        SUPER_PAGE_BUSY,
        SUPER_PAGE_MAX,
    } SUPER_KNOB_PAGE_NUM;

    void lv_amin_start(void *obj,
                       int32_t start_value,
                       int32_t end_value,
                       uint32_t repeat_count,
                       uint32_t duration,
                       uint32_t delay,
                       lv_anim_exec_xcb_t exec_cb,
                       lv_anim_path_cb_t path_cb);

    void lv_obj_add_anim_down(lv_obj_t *obj, uint32_t delay);

    void lv_fun_delayed(void (*fun)(), uint32_t SysTick_Delay_ms);

    void lv_obj_add_anim_up(lv_obj_t *obj, uint32_t delay);

    void lv_obj_add_anim_right(lv_obj_t *obj, uint32_t delay);

    void lv_obj_add_anim_left(lv_obj_t *obj, uint32_t delay);
#ifdef __cplusplus
}
#endif
#endif