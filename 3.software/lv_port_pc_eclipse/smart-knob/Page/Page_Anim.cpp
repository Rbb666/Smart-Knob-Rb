#include "lvgl.h"
#include "Page_Anim.h"

#define LV_DEMO_PRINTER_ANIM_TIME (150)

void lv_amin_start(void *obj,
                   int32_t start_value,
                   int32_t end_value,
                   uint32_t repeat_count,
                   uint32_t duration,
                   uint32_t delay,
                   lv_anim_exec_xcb_t exec_cb,
                   lv_anim_path_cb_t path_cb)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_values(&anim, start_value, end_value);
    lv_anim_set_repeat_count(&anim, repeat_count);
    lv_anim_set_exec_cb(&anim, exec_cb);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, path_cb);
    lv_anim_start(&anim);
}

//从上至下动态加载控件
void lv_obj_add_anim_down(lv_obj_t *obj, uint32_t delay)
{
    if (obj != lv_scr_act())
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_DEMO_PRINTER_ANIM_TIME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(obj) - 60, lv_obj_get_y(obj));
        lv_anim_start(&a);
        lv_obj_fade_in(obj, LV_DEMO_PRINTER_ANIM_TIME - 50, delay); //动画淡入
    }
}

//从下至上动态加载控件
void lv_obj_add_anim_up(lv_obj_t *obj, uint32_t delay)
{
    if (obj != lv_scr_act())
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_DEMO_PRINTER_ANIM_TIME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_values(&a, lv_obj_get_y(obj) + 60, lv_obj_get_y(obj));
        lv_anim_start(&a);
        lv_obj_fade_in(obj, LV_DEMO_PRINTER_ANIM_TIME - 50, delay); //动画淡入
    }
}

//从左至右动态加载控件
void lv_obj_add_anim_right(lv_obj_t *obj, uint32_t delay)
{
    if (obj != lv_scr_act())
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_DEMO_PRINTER_ANIM_TIME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(obj) - 60, lv_obj_get_x(obj));
        lv_anim_start(&a);
        lv_obj_fade_in(obj, LV_DEMO_PRINTER_ANIM_TIME - 50, delay); //动画淡入
    }
}

//从右至左动态加载控件
void lv_obj_add_anim_left(lv_obj_t *obj, uint32_t delay)
{
    if (obj != lv_scr_act())
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, obj);
        lv_anim_set_time(&a, LV_DEMO_PRINTER_ANIM_TIME);
        lv_anim_set_delay(&a, delay);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, lv_obj_get_x(obj) + 60, lv_obj_get_x(obj));
        lv_anim_start(&a);
        lv_obj_fade_in(obj, LV_DEMO_PRINTER_ANIM_TIME - 50, delay); //动画淡入
    }
}

//移除全部控件
static void lv_demo_printer_anim_out_all(lv_obj_t *ignore_obj, lv_obj_t *obj, uint32_t delay)
{
    uint32_t i = 0;
    for (i = 0; i < lv_obj_get_child_cnt(obj); i++)
    {
        lv_obj_t *child = lv_obj_get_child(obj, i);
        /*Do something with child*/
        if (child != ignore_obj && child != lv_scr_act())
        {
            lv_anim_t a;
            lv_anim_init(&a);                                          //初始化动画变量
            lv_anim_set_var(&a, child);                                // 设置变量进行动画处理
            lv_anim_set_time(&a, LV_DEMO_PRINTER_ANIM_TIME);           // 设置动画的持续时间
            lv_anim_set_delay(&a, delay);                              // 设置开始动画之前的延迟
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y); // 设置动画功能  可以以横纵坐标为入口参数
            if (lv_obj_get_y(child) < 80)
            {
                lv_anim_set_values(&a, lv_obj_get_y(child), lv_obj_get_y(child) - 60);
            }
            else
            {
                lv_anim_set_values(&a, lv_obj_get_y(child), lv_obj_get_y(child) + 60);

                delay += 60;
            }
            lv_anim_set_ready_cb(&a, lv_obj_del_anim_ready_cb);
            lv_anim_start(&a);
        }
    }
}