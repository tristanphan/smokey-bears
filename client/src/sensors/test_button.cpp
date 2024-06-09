#include "sensors/test_button.h"

bool pressed_pending = false;

void interrupt_handler() {
    pressed_pending = true;
}

void TestButton::initialize() {
    pinMode(TEST_BUTTON_PIN, INPUT_PULLUP);
    int pin = digitalPinToInterrupt(TEST_BUTTON_PIN);
    Serial.printf("[TEST BUTTON] Pin %d set as interrupt\n", pin);
    attachInterrupt(pin, interrupt_handler, RISING);
}

bool TestButton::is_pressed() {
    if (pressed_pending) {
        Serial.printf("[TEST BUTTON] Button pressed\n");
        pressed_pending = false;
        return true;
    }
    return false;
}
