#pragma once
#include <AceButton.h>
#include <Arduino.h>

#include "task.h"

class InterfaceTask : public Task<InterfaceTask>, public ace_button::IEventHandler
{
    friend class Task<InterfaceTask>;
public:
    InterfaceTask(const uint8_t task_core);

    ~InterfaceTask();

    void handleEvent(ace_button::AceButton *button, uint8_t event_type, uint8_t button_state) override;

protected:
    void run();

private:
    int current_config_ = 0;

    // void changeConfig(bool next);
};
