#include "../GUI/DisplayPrivate.h"
#include "../Basic/FileGroup.h"
#include "BSP.h"

/**
 * @brief  背光初始化
 * @param  无
 * @retval 无
 */
void Backlight_Init(void)
{
  /*设置频率为20kHz，分辨率为10位 占空比最大1023 */
  ledcSetup(1, 1000, 10);
  ledcAttachPin(PIN_LCD_BACKLIGHT, 1);
}

/**
 * @brief  背光亮度渐变，受lv_anim控制
 * @param  obj:无用
 * @param  brightness:亮度值
 * @retval None
 */
static void Backlight_AnimCallback(void *obj, int16_t brightness)
{
  Backlight_SetValue(brightness);
}

/**
 * @brief  背光设置，渐变效果
 * @param  target:目标亮度(0~1000 -> 0~100%)
 * @retval 无
 */
void Backlight_SetGradual(uint16_t target, uint16_t time)
{
  static lv_anim_t a;
  lv_obj_add_anim(
      NULL, &a,
      (lv_anim_exec_xcb_t)Backlight_AnimCallback,
      Backlight_GetValue(), target,
      time);
}

/**
 * @brief  获取背光亮度
 * @param  无
 * @retval 当前亮度(0~1000 -> 0~100%)
 */
uint16_t Backlight_GetValue()
{
  return ledcRead(1);
}

/**
 * @brief  设置背光亮度
 * @param  val: 亮度(0~1000 -> 0~100%)
 * @retval 无
 */
void Backlight_SetValue(int16_t val)
{
  __LimitValue(val, 0, 1000);
  ledcWrite(1, val); //亮度，0-1000
}