#include <display.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <main.h>
#include <Hal/motor.h>

#include "demos/lv_demos.h"
#include "Page/gui_super_knob.h"
#include "GUI/DisplayPrivate.h"

static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;

#define COLOR_BUFFER (screenWidth * screenHeight / 2)

static lv_disp_draw_buf_t draw_buf;
void *lv_disp_buf1_p = nullptr;
void *lv_disp_buf2_p = nullptr;

lv_ui super_knob_ui;

/* TFT instance */
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setSwapBytes(true);
    tft.pushImageDMA(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);
    tft.dmaWait();
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = get_motor_position();

    data->enc_diff = 0;
    if (now_num > old_num)
    {
        data->enc_diff++;
        old_num = get_motor_position();
        // update_ws2812_status(WS2812_ROLL, 10);
    }
    else if (now_num < old_num)
    {
        data->enc_diff--;
        old_num = get_motor_position();
        // update_ws2812_status(WS2812_ROLL, 10);
    }

    if (xSemaphoreTake(xBinarySemaphore, (TickType_t)0) == pdTRUE)
    {
        data->state = LV_INDEV_STATE_PR;
        update_page_status(BUTTON_CLICK);
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    // page_status_check();
}

void update_page_status(int page_status)
{
    struct _knob_message *send_message;
    send_message = &LVGL_MSG;
    send_message->ucMessageID = page_status;
    xQueueSend(motor_rcv_Queue, &send_message, (TickType_t)0u);
    Serial.printf("send_message to Queue --->(%d)\n", send_message->ucMessageID);
}

void Task_lvgl(void *pvParameters)
{
    (void)pvParameters;

    lv_disp_buf1_p = heap_caps_malloc(COLOR_BUFFER * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(lv_disp_buf1_p != NULL);
    lv_disp_buf2_p = heap_caps_malloc(COLOR_BUFFER * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(lv_disp_buf2_p != NULL);

    Serial.printf("--------------------------------\n");
    Serial.printf("heap get free size(%d)\n", heap_caps_get_free_size(MALLOC_CAP_DMA));
    Serial.printf("--------------------------------\n");

    lv_init();

    tft.begin();
    tft.initDMA();
    tft.setRotation(0);

    ledcSetup(0, 1000, 16);
    ledcAttachPin(PIN_LCD_BACKLIGHT, 0);
    ledcWrite(0, UINT16_MAX);

    lv_disp_draw_buf_init(&draw_buf, lv_disp_buf1_p, lv_disp_buf2_p, COLOR_BUFFER);
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*加载输入设备*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    super_knob_ui.indev_encoder = lv_indev_drv_register(&indev_drv);

    super_knob_ui.defult_group = lv_group_create();
    lv_group_set_default(super_knob_ui.defult_group);
    lv_indev_set_group(super_knob_ui.indev_encoder, super_knob_ui.defult_group);

    update_page_status(0);

    // lv_demo_music();

    /*APP窗口初始化*/
    AppWindow_Create(lv_scr_act());

    DisplayPage_Init();

    Serial.printf("Screen init success!!\n");

    for (;;)
    {
        Display_Update();
        vTaskDelay(LV_DISP_DEF_REFR_PERIOD);
    }
}