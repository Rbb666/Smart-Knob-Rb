#include "GUI/DisplayPrivate.h"

/*实例化页面调度器*/
PageManager page(PAGE_MAX);

/*页面导入*/
#define PAGE_IMPORT(name)                                                     \
    do                                                                        \
    {                                                                         \
        extern void PageRegister_##name(PageManager *page_p, uint8_t pageID); \
        PageRegister_##name(&page, PAGE_##name);                              \
    } while (0)

static void page_gestute_event_cb(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_GESTURE)
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        page.EventTransmit(event->target, dir);
    }
}

/**
 * @brief  页面初始化
 * @param  无
 * @retval 无
 */
void DisplayPage_Init(void)
{
    PAGE_IMPORT(StartUp);    //开始菜单
    PAGE_IMPORT(MainMenu);   //主菜单
    PAGE_IMPORT(Switch);     //菜单
    PAGE_IMPORT(Smart);      //菜单
    PAGE_IMPORT(Window);     //菜单
    page.Push(PAGE_StartUp); //进入第一个页面
    lv_obj_add_event_cb(lv_scr_act(), page_gestute_event_cb, LV_EVENT_ALL, NULL);
}

/**
 * @brief  显示更新
 * @param  无
 * @retval 无
 */
void Display_Update(void)
{
    page.Running();
    lv_task_handler();
}

/**
 * @brief  页面阻塞延时，保持lvgl更新
 * @param  无
 * @retval 无
 */
void PageDelay(uint32_t ms)
{
    uint32_t lastTime = lv_tick_get();

    while (lv_tick_elaps(lastTime) <= ms)
    {
        lv_task_handler();
    }
}
