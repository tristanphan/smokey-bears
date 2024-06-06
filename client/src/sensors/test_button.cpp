#include "sensors/test_button.h"

bool pressed_pending = false;

void interrupt_handler() {
    pressed_pending = true;
}

void TestButton::initialize() {
    pinMode(TEST_BUTTON_PIN, INPUT_PULLUP);
    if (digitalPinToInterrupt(TEST_BUTTON_PIN) == -1) {
        Serial.printf("[Test Button] Pin cannot be used as interrupt");
        while (true);
    } else {
        Serial.printf("[Test Button] Pin successfully set as interrupt");
        attachInterrupt(digitalPinToInterrupt(TEST_BUTTON_PIN), interrupt_handler, RISING);
    }
}

bool TestButton::is_pressed() {
    if (pressed_pending) {
        pressed_pending = false;
        return true;
    }
    return false;
}
