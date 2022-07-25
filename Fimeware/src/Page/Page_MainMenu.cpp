#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"
#include "Page_Anim.h"

#include <Hal/motor.h>

PAGE_EXPORT(MainMenu);

extern "C"
{
    LV_IMG_DECLARE(IMG_Switch);
    LV_IMG_DECLARE(IMG_Smart);
    LV_IMG_DECLARE(IMG_Window);
    LV_IMG_DECLARE(IMG_Music);
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

#define APP_ICON_SIZE 85
#define APP_ICON_SIZE_START 0
#define APP_ICON_SIZE_END APP_ICON_SIZE

static AppICON_TypeDef AppICON_Grp[] = {
    APP_DEF(Switch, LV_COLOR_MAKE(0, 40, 255)),
    APP_DEF(Smart, LV_COLOR_MAKE(75, 216, 99)),
    APP_DEF(Window, LV_COLOR_MAKE(75, 216, 99)),
    APP_DEF(Music, LV_COLOR_MAKE(75, 216, 99)),
};

static lv_style_t style_test;
static lv_style_t style_btn;
static lv_style_t style_pr;
static lv_style_t style_def;
static lv_style_transition_dsc_t transition_dsc_def;
static lv_style_transition_dsc_t transition_dsc_pr;

static lv_obj_t *page_cont = NULL;
static lv_obj_t *scroll_cont[__Sizeof(AppICON_Grp)];
static lv_obj_t *cont_btn[__Sizeof(AppICON_Grp)];
static lv_obj_t *btn_text[__Sizeof(AppICON_Grp)];
static lv_obj_t *btn_img[__Sizeof(AppICON_Grp)];

static void scroll_event_cb(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for (i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if (diff_y >= r)
        {
            x = r;
        }
        else
        {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000); /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

static void button_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    AppICON_TypeDef *appIcon = (AppICON_TypeDef *)lv_obj_get_user_data(btn);

    if (code == LV_EVENT_SHORT_CLICKED)
    {
        uint8_t pageID = appIcon->pageID;
        Serial.printf("page id is:(%d)\n", pageID);

        switch (pageID)
        {
        case PAGE_Switch:
            Page->Push(PAGE_Switch);
            break;
        case PAGE_Smart:
            Page->Push(PAGE_Smart);
            update_motor_config(1);
            break;
        case PAGE_Window:
            Page->Push(PAGE_Window);
            update_motor_config(2);
            break;
        case PAGE_Music:
            Page->Push(PAGE_Music);
            break;
        default:
            break;
        }
        update_page_status(CHECKOUT_PAGE);
    }
}

static void anim_size_cb(lv_obj_t *var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}

void Item_Create(
    uint8_t page_num,
    lv_obj_t *par,
    void *user_data,
    const void *image,
    const char *infos)
{
    static lv_style_prop_t props[] = {
        LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, LV_STYLE_PROP_INV};

    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);

    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, -10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 20);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_bg_color(&style_btn, lv_color_white());

    /*Create txt style*/
    lv_style_init(&style_test);
    lv_style_set_text_color(&style_test, lv_color_white());

    /*Create cont*/
    scroll_cont[page_num] = lv_obj_create(par);
    lv_obj_set_scrollbar_mode(scroll_cont[page_num], LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(scroll_cont[page_num], lv_pct(100), APP_ICON_SIZE + 5);
    lv_obj_set_style_bg_opa(scroll_cont[page_num], 0, 0);
    lv_obj_set_style_border_width(scroll_cont[page_num], 0, 0);
    lv_obj_align(scroll_cont[page_num], LV_ALIGN_CENTER, 0, 0);

    /*Create button*/
    cont_btn[page_num] = lv_btn_create(scroll_cont[page_num]);
    lv_obj_set_size(cont_btn[page_num], APP_ICON_SIZE, APP_ICON_SIZE);
    lv_obj_add_style(cont_btn[page_num], &style_btn, LV_PART_MAIN);
    lv_obj_add_style(cont_btn[page_num], &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(cont_btn[page_num], &style_def, 0);
    lv_obj_add_event_cb(cont_btn[page_num], button_event_cb, LV_EVENT_ALL, 0);
    lv_obj_set_user_data(cont_btn[page_num], user_data);
    lv_obj_align(cont_btn[page_num], LV_ALIGN_LEFT_MID, 0, 0);

    /*Create lable*/
    btn_text[page_num] = lv_label_create(scroll_cont[page_num]);
    lv_obj_set_style_text_font(btn_text[page_num], &lv_font_montserrat_18, 0);
    lv_label_set_text(btn_text[page_num], infos);
    lv_obj_add_style(btn_text[page_num], &style_test, LV_PART_MAIN);
    lv_obj_align_to(btn_text[page_num], cont_btn[page_num], LV_ALIGN_OUT_RIGHT_MID, 30, 0);

    /*Create image*/
    btn_img[page_num] = lv_img_create(cont_btn[page_num]);
    lv_img_set_src(btn_img[page_num], image);
    lv_obj_align(btn_img[page_num], LV_ALIGN_CENTER, 0, 0);

    static lv_anim_t anim_zoom;
    lv_anim_init(&anim_zoom);
    lv_anim_set_var(&anim_zoom, cont_btn[page_num]);
    lv_anim_set_values(&anim_zoom, APP_ICON_SIZE_START, APP_ICON_SIZE_END);
    lv_anim_set_time(&anim_zoom, 800);

    lv_anim_set_path_cb(&anim_zoom, lv_anim_path_bounce);
    lv_anim_set_exec_cb(&anim_zoom, (lv_anim_exec_xcb_t)anim_size_cb);
    lv_anim_start(&anim_zoom);
}

void setup_main_page_menu(lv_obj_t *page)
{
    lv_obj_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    page_cont = lv_obj_create(page);
    lv_obj_set_size(page_cont, 240, 240);
    lv_obj_align(page_cont, LV_ALIGN_CENTER, 10, 0);
    lv_obj_set_flex_flow(page_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(page_cont, scroll_event_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_set_style_radius(page_cont, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(page_cont, true, 0);
    lv_obj_set_style_bg_opa(page_cont, 0, 0);
    lv_obj_set_style_border_width(page_cont, 0, 0);
    lv_obj_set_scroll_dir(page_cont, LV_DIR_VER); // 滚动方向
    lv_obj_set_scroll_snap_y(page_cont, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(page_cont, LV_SCROLLBAR_MODE_OFF);

    for (int i = 0; i < __Sizeof(AppICON_Grp); i++)
    {
        if (i != 0)
        {
            /*Create line style*/
            static lv_style_t style_line;
            lv_style_init(&style_line);
            lv_style_set_line_width(&style_line, 4);
            lv_style_set_line_color(&style_line, lv_color_hex(0xff0000));

            lv_obj_t *line = lv_line_create(page_cont);
            static lv_point_t line_points[] = {{90, 0}, {90, 80}};
            lv_line_set_points(line, line_points, 2);
            lv_obj_add_style(line, &style_line, LV_PART_MAIN);
        }

        Item_Create(
            i,
            page_cont,
            &AppICON_Grp[i],
            (void *)AppICON_Grp[i].src_img,
            AppICON_Grp[i].name);
    }

    /*Update the buttons position manually for first*/
    lv_event_send(page_cont, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(page_cont, 0), LV_ANIM_OFF);

    /*动画*/
    lv_amin_start(page, -120, 0, 1, 240, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}

/**
 * @brief  页面初始化事件
 * @param  无
 * @retval 无
 */
static void Setup()
{
    Serial.printf("Page MainMenu -------->\n");
    //获取芯片可用内存
    Serial.printf("esp free heap size : %d  \n", esp_get_free_heap_size());
    //获取从未使用过的最小内存
    Serial.printf("esp minm free heap size : %d  \n", esp_get_minimum_free_heap_size());

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
    lv_amin_start(appWindow,
                  lv_obj_get_x(appWindow),
                  -240,
                  1,
                  240,
                  0,
                  (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);

    PageDelay(LV_ANIM_TIME_DEFAULT);
    lv_obj_clean(page_cont);
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
    if (event == LV_EVENT_LONG_PRESSED)
    {
        // Serial.printf("page pop!!!\n");
        // Page->Pop();
    }
}
