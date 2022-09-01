#pragma once
#include <AceButton.h>
#include <Arduino.h>

#include "task.h"

class InterfaceTask : public Task<InterfaceTask>, public ace_button::IEventHandler
{
    friend class Task<InterfaceTask>;

    typedef enum
    {
        LED_NORMAL,
        LED_RAINBOWCYCLE,
        LED_RAINBOW,
        LED_METEOR_OVERTURN,
        LED_ROLL,
    } LED_RUNNING_MODE_E;

public:
    InterfaceTask(const uint8_t task_core);

    ~InterfaceTask();

    void handleEvent(ace_button::AceButton *button, uint8_t event_type, uint8_t button_state) override;

    void Firt_Light(void);

    int led_status;

protected:
    void run();

private:
    int current_config_ = 0;

    QueueHandle_t led_rcv_Queue;

    // void changeConfig(bool next);
};
