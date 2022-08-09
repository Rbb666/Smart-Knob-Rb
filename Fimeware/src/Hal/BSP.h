#ifndef __BSP_H
#define __BSP_H

#include "../Basic/FileGroup.h"
#include <stdint.h>

/*Backlight*/
void Backlight_Init(void);
uint16_t Backlight_GetValue(void);
void Backlight_SetValue(int16_t val);
void Backlight_SetGradual(uint16_t target, uint16_t time = 500);
void Backlight_UpdateBKP(void);
uint16_t Backlight_GetBKP(void);

#endif
