#pragma once

#include <Arduino.h>
#include <BleKeyboard.h>

#include "task.h"

class Ble_Interface
{
public:
    Ble_Interface();

    ~Ble_Interface();

    void begin(void);

    bool check_keyboard_connected(void);

    int keyboard_enable_player(void);

    int keyboard_player_next(void);
    
    int keyboard_turn_up_volume(void);

    int keyboard_turn_down_volume(void);

protected:
    void run();

private:
};
