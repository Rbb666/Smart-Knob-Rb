#include "Basic/FileGroup.h"
#include "GUI/DisplayPrivate.h"

PAGE_EXPORT(StartUp);

static lv_obj_t *ui_cont = NULL;
static lv_obj_t *ui_labelLogo = NULL;
static lv_anim_timeline_t *anim_timeline = NULL;

static lv_timer_t *timer1 = NULL;
static lv_timer_t *timer2 = NULL;

#define COLOR_RED lv_color_hex(0xff0000)

void StartupView_Create(lv_obj_t *root) {
    lv_obj_remove_style_all(root);
    lv_obj_set_size(root, LV_HOR_RES, LV_VER_RES);

    lv_obj_t *cont = lv_obj_create(root);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 110, 40);
    lv_obj_set_style_border_color(cont, COLOR_RED, 0);
    lv_obj_set_style_border_side(cont, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(cont, 3, 0);
    lv_obj_set_style_border_post(cont, true, 0);
    lv_obj_center(cont);
    ui_cont = cont;

    lv_obj_t *label = lv_label_create(cont);
    LV_FONT_DECLARE(HandGotn_20);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_obj_set_style_text_font(label, &HandGotn_20, 0);
    lv_label_set_text(label, "Knob-Rb");
    lv_obj_center(label);
    ui_labelLogo = label;

    anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    lv_anim_timeline_wrapper_t wrapper[] =
            {
                    ANIM_DEF(0, ui_cont, width, 0, lv_obj_get_style_width(ui_cont, 0)),
                    ANIM_DEF(500, ui_labelLogo, y, lv_obj_get_style_width(ui_cont, 0), lv_obj_get_y(ui_labelLogo) + 5),
                    LV_ANIM_TIMELINE_WRAPPER_END};

    lv_anim_timeline_add_wrapper(anim_timeline, wrapper);

    lv_anim_timeline_start(anim_timeline);
}

void onTimer(lv_timer_t *timer) {
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 300, lv_anim_path_ease_out, true}

    if (timer == timer1) {

        lv_anim_timeline_wrapper_t wrapper[] =
                {
                        ANIM_DEF(0, ui_cont, width, lv_obj_get_style_width(ui_cont, 0), 0),
                        LV_ANIM_TIMELINE_WRAPPER_END};
        lv_anim_timeline_add_wrapper(anim_timeline, wrapper);

        lv_anim_timeline_start(anim_timeline);
    } else if (timer == timer2) {
        Page->Push(PAGE_MainMenu);
    }
}

static void Setup() {
    /*将此页面移到前台*/
    lv_obj_move_foreground(appWindow);
    StartupView_Create(appWindow);
    timer1 = lv_timer_create(onTimer, 1500, NULL);
    timer2 = lv_timer_create(onTimer, 2000, NULL);
    lv_timer_set_repeat_count(timer1, 1);
    lv_timer_set_repeat_count(timer2, 1);
}

static void Exit() {
    if (anim_timeline) {
        lv_anim_timeline_del(anim_timeline);
        anim_timeline = nullptr;
    }
    PageDelay(LV_ANIM_TIME_DEFAULT);
    lv_obj_clean(appWindow);
}

static void Event(void *obj, uint8_t event) {}
