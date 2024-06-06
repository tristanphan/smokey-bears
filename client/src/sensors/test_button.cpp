#include "sensors/test_button.h"

void TestButton::initialize() {
    pinMode(TEST_BUTTON_PIN, INPUT_PULLUP);
}

bool TestButton::is_pressed() {
    bool previously_pressed = pressed;
    pressed = digitalRead(TEST_BUTTON_PIN) == LOW;

    if (pressed && !previously_pressed) {
        Serial.println("[TEST BUTTON] Button pressed");
        return true;
    }
    return false;
}
