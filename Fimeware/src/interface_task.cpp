#include "interface_task.h"
#include <main.h>
#include <AceButton.h>
#include <FastLED.h>
#include <HX711.h>
#include <Adafruit_VEML7700.h>

#include "util.h"

using namespace ace_button;

CRGB leds[NUM_LEDS];

HX711 scale;

Adafruit_VEML7700 veml = Adafruit_VEML7700();

InterfaceTask::InterfaceTask(const uint8_t task_core) : Task("Motor", 2048, 3, task_core) {}

InterfaceTask::~InterfaceTask() {}

void InterfaceTask::run()
{
    pinMode(PIN_BUTTON_NEXT, INPUT_PULLUP);

    AceButton button_next((uint8_t)PIN_BUTTON_NEXT);
    button_next.getButtonConfig()->setIEventHandler(this);

    FastLED.addLeds<SK6812, PIN_LED_DATA, GRB>(leds, NUM_LEDS);

    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(400000);
#if SK_STRAIN
    scale.begin(38, 2);
#endif

    if (veml.begin())
    {
        veml.setGain(VEML7700_GAIN_2);
        veml.setIntegrationTime(VEML7700_IT_400MS);
    }
    else
    {
        Serial.println("ALS sensor not found!");
    }

    float press_value_unit = 0;

    // 开场灯效
    Firt_Light();

    led_status = LED_NORMAL;

    while (1)
    {
        button_next.check();

#if SK_ALS
        const float LUX_ALPHA = 0.005;
        static float lux_avg;
        float lux = veml.readLux();
        lux_avg = lux * LUX_ALPHA + lux_avg * (1 - LUX_ALPHA);
#endif

        const int32_t lower = 950000;
        const int32_t upper = 1800000;
        if (scale.wait_ready_timeout(100))
        {
            int32_t reading = scale.read();

            // Ignore readings that are way out of expected bounds
            if (reading >= lower - (upper - lower) && reading < upper + (upper - lower) * 2)
            {
                static uint32_t last_reading_display;
                if (millis() - last_reading_display > 1000)
                {
                    Serial.print("HX711 reading: ");
                    Serial.println(reading);
                    last_reading_display = millis();
                }
                long value = CLAMP(reading, lower, upper);
                press_value_unit = 1. * (value - lower) / (upper - lower);

                static bool pressed;
                // 确认按下
                if (!pressed && press_value_unit > 0.55)
                {
                    xSemaphoreGive(xBinarySemaphore);
                    pressed = true;
                }
                else if (pressed && press_value_unit < 0.25)
                {

                    pressed = false;
                }
            }
        }
        else
        {
            Serial.println("HX711 not found.");

            for (uint8_t i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Red;
            }
            FastLED.show();
        }

        uint16_t brightness = UINT16_MAX;
        brightness = (uint16_t)CLAMP(lux_avg * 13000, (float)1280, (float)UINT16_MAX);

        switch (led_status)
        {
        case LED_NORMAL:
        {
            for (uint8_t i = 0; i < NUM_LEDS; i++)
            {
                leds[i].setHSV(200 * press_value_unit, 255, brightness >> 8);
                // Gamma adjustment
                leds[i].r = dim8_video(leds[i].r);
                leds[i].g = dim8_video(leds[i].g);
                leds[i].b = dim8_video(leds[i].b);
            }
            break;
        }
        default:
            break;
        }

        FastLED.show();

        vTaskDelay(50);
    }
}

void InterfaceTask::handleEvent(AceButton *button, uint8_t event_type, uint8_t button_state)
{
    switch (event_type)
    {
    case AceButton::kEventPressed:
        if (button->getPin() == PIN_BUTTON_NEXT)
        {
        }
        break;
    case AceButton::kEventReleased:
        break;
    case AceButton::kEventLongPressed:
        break;
    }
}

void InterfaceTask::Firt_Light(void)
{
    for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++)
    {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::White;
        FastLED.show();
        delay(150);
        // Turn our current led back to black for the next loop around
        leds[whiteLed] = CRGB::Black;
        // FastLED.show();
    }
}

void rainbow(uint8_t wait)
{
    fill_rainbow(leds, NUM_LEDS, 0, 7);
    FastLED.show();
    delay(wait);
}
