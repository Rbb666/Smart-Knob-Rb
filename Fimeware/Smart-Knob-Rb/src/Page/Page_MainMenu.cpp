#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(MainMenu);
// 全部APP界面
static lv_obj_t *contApps;

extern "C"
{
    LV_IMG_DECLARE(IMG_Fun);
    LV_IMG_DECLARE(IMG_Switch);
    LV_IMG_DECLARE(IMG_Computer);
}

typedef struct
{
    const void *src_img;
    const char *name;
    const uint8_t pageID;
    const lv_color_t bg_color;
    lv_obj_t *obj;
} AppICON_TypeDef;

#define APP_DEF(name, color)                         \
    {                                                \
        &IMG_##name, #name, PAGE_##name, color, NULL \
    }

#define APP_ICON_SIZE_START 10
#define APP_ICON_SIZE_END 100
#define APP_ICON_SIZE 100

static AppICON_TypeDef AppICON_Grp[] = {
    APP_DEF(Switch, LV_COLOR_MAKE(0, 40, 255)),
    APP_DEF(Fun, LV_COLOR_MAKE(75, 216, 99)),
    APP_DEF(Computer, LV_COLOR_MAKE(75, 216, 99)),
};

static void lamp_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        // LV_LOG_USER("Toggled");
    }
}

static void fan_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        // LV_LOG_USER("Toggled");
    }
}

static void sw_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *sw = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
    }
}

void lv_set_scroll_box(lv_obj_t *background_btn, void *image_src)
{
    /* style */
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    // lv_style_set_bg_opa(&style_btn, 0); //透明
    lv_style_set_radius(&style_btn, 20);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_bg_color(&style_btn, lv_color_white());

    lv_obj_set_size(background_btn, APP_ICON_SIZE, APP_ICON_SIZE);
    lv_obj_add_style(background_btn, &style_btn, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(background_btn);
    lv_img_set_src(img, image_src);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

static void anim_x_cb(lv_obj_t *var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_y_cb(lv_obj_t *var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void anim_size_cb(lv_obj_t *var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}
static lv_anim_timeline_t *anim_timeline = NULL;

void setup_main_page_menu(lv_obj_t *page)
{
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    uint32_t i, n = 1;
    uint32_t pos;

    for (i = 0; i < __Sizeof(AppICON_Grp); i++)
    {
        lv_obj_t *btn = lv_btn_create(page);
        lv_set_scroll_box(btn, (void *)AppICON_Grp[i].src_img);
        // lv_obj_align(btn, LV_ALIGN_CENTER, i * 10, 15);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, btn);
        lv_anim_set_values(&a, APP_ICON_SIZE_START, APP_ICON_SIZE_END);
        lv_anim_set_time(&a, 1000);

        lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_size_cb);
        lv_anim_start(&a);

        lv_anim_t a2;
        lv_anim_init(&a2);
        lv_anim_set_var(&a2, btn);
        lv_anim_set_values(&a2, 0, i * APP_ICON_SIZE + i * 10); // 40 80+40 160+40
        lv_anim_set_time(&a2, 1000);

        lv_anim_set_path_cb(&a2, lv_anim_path_overshoot);
        lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)anim_y_cb);
        lv_anim_start(&a2);

        lv_anim_t a3;
        lv_anim_init(&a3);
        lv_anim_set_var(&a3, btn);

        pos = (120 - APP_ICON_SIZE / 2) - (i * APP_ICON_SIZE / 2);

        if (i <= __Sizeof(AppICON_Grp) / 2)
            lv_anim_set_values(&a3, 0, pos); //前一半的情况
        else
        {
            for (int i = __Sizeof(AppICON_Grp) / 2 - 1; i > 0; i--)
            {
                pos += (i * APP_ICON_SIZE / 2);
                lv_anim_set_values(&a3, 0, pos); //后一半的情况
            }
        }

        lv_anim_set_time(&a3, 1000);

        lv_anim_set_path_cb(&a3, lv_anim_path_overshoot);
        lv_anim_set_exec_cb(&a3, (lv_anim_exec_xcb_t)anim_x_cb);
        lv_anim_start(&a3);
    }

    lv_obj_update_snap(page, LV_ANIM_ON);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup()
{
    //获取芯片可用内存
    Serial.printf(" page_MainMenu_start    esp_get_free_heap_size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    Serial.printf(" page_MainMenu_start    esp_get_minmusicm_free_heap_size : %d  \n", esp_get_minimum_free_heap_size());

    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    setup_main_page_menu(appWindow);
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
    // if (obj == lv_scr_act())
    // {
    //     if (event == LV_GESTURE_DIR_LEFT || event == LV_GESTURE_DIR_RIGHT)
    //     {
    //         Page->Pop();
    //     }
    // }

    if (event == LV_EVENT_LONG_PRESSED)
    {
        Serial.printf("page pop!!!\n");
        Page->Pop();
    }
}
