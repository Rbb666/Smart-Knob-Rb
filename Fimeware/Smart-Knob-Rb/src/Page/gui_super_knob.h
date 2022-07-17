/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-05-27 00:05:12
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 17:51:55
 */
/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_SUPER_KNOB_H
#define GUI_SUPER_KNOB_H
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

#ifdef __cplusplus
}
#endif
#endif