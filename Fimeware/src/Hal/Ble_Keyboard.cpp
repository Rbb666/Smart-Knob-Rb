#include "Ble_Keyboard.h"

BleKeyboard bleKeyboard;

Ble_Interface::Ble_Interface() {}

Ble_Interface::~Ble_Interface() {}

void Ble_Interface::begin()
{
    bleKeyboard.begin();
    Serial.println("Starting BLE work!");
}

bool Ble_Interface::check_keyboard_connected(void)
{
    return bleKeyboard.isConnected();
}

int Ble_Interface::keyboard_player_pause_play(void)
{
    if (!check_keyboard_connected())
    {
        Serial.println("check_keyboard_connected error...");
        return -1;
    }

    Serial.println("music pause or play...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    return 0;
}

int Ble_Interface::keyboard_player_prev(void)
{
    if (!check_keyboard_connected())
        return -1;

    Serial.println("music prev...");
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    return 0;
}

int Ble_Interface::keyboard_player_next(void)
{
    if (!check_keyboard_connected())
        return -1;

    Serial.println("music next...");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    return 0;
}

int Ble_Interface::keyboard_turn_up_volume(void)
{
    if (!check_keyboard_connected())
    {
        Serial.println("check_keyboard_connected error...");
        return -1;
    }

    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    return 0;
}

int Ble_Interface::keyboard_turn_down_volume(void)
{
    if (!check_keyboard_connected())
        return -1;

    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    return 0;
}

void Ble_Interface::run() {}
