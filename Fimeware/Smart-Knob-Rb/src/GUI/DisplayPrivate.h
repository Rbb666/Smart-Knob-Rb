#ifndef __DISPLAYPRIVATE_H
#define __DISPLAYPRIVATE_H

/*Basic*/

// extern SCREEN_CLASS screen;
void lv_port_disp_init();
void lv_port_indev_init();
void lv_port_log_init();

void Display_Init();
void Display_Update();

/*Page*/
#include "PageManager.h"
extern PageManager page;
typedef enum
{
    /*保留*/
    PAGE_NONE,
    /*用户页面*/
    PAGE_StartUp,
    PAGE_MainMenu,
    PAGE_Switch,
    PAGE_Smart,
    PAGE_Win,
    PAGE_Music,

    /*保留*/
    PAGE_MAX
} Page_Type;

void PageDelay(uint32_t ms);
void DisplayPage_Init();

#define PAGE_EXPORT(name)                                                         \
    static PageManager *Page;                                                     \
    static lv_obj_t *appWindow;                                                   \
    static void Setup();                                                          \
    static void Exit();                                                           \
    static void Event(void *obj, uint8_t event);                                  \
    static void Page_EventHandler(void *obj, uint8_t event)                       \
    {                                                                             \
        if (obj == Page)                                                          \
        {                                                                         \
            switch (event)                                                        \
            {                                                                     \
            case PageManager::MSG_Setup:                                          \
                Setup();                                                          \
                break;                                                            \
            case PageManager::MSG_Exit:                                           \
                Exit();                                                           \
                break;                                                            \
            case PageManager::MSG_Loop: /*Loop();*/                               \
                break;                                                            \
            default:                                                              \
                break;                                                            \
            }                                                                     \
        }                                                                         \
        else                                                                      \
        {                                                                         \
            Event(obj, event);                                                    \
        }                                                                         \
    }                                                                             \
    void PageRegister_##name(PageManager *page, uint8_t pageID)                   \
    {                                                                             \
        appWindow = AppWindow_GetObj(pageID);                                     \
        lv_obj_add_event_cb(appWindow, (lv_event_cb_t)Event, LV_EVENT_ALL, NULL); \
        page->Register(pageID, Page_EventHandler, #name);                         \
        Page = page;                                                              \
    }

/*LVGL*/
#include "lvgl.h"
#include "Utils/lv_ext/lv_obj_ext_func.h"
#include "Utils/lv_ext/lv_anim_timeline_wrapper.h"

extern "C"
{
    LV_FONT_DECLARE(FONT_MONTSERRAT_14);
}

/*AppWindow*/
void AppWindow_Create(lv_obj_t *par);
lv_obj_t *AppWindow_GetObj(uint8_t pageID);
#define APP_WIN_HEIGHT lv_obj_get_height(appWindow)
#define APP_WIN_WIDTH lv_obj_get_width(appWindow)

#endif
