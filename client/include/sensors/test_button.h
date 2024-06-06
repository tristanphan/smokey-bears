#ifndef CS147_TEST_BUTTON_H
#define CS147_TEST_BUTTON_H

#include "config.h"
#include <Arduino.h>


// This button works as both a button to start the alarm and reset it.
class TestButton {
public:
    void initialize();

    bool is_pressed();

private:
    bool pressed;
};


#endif //CS147_TEST_BUTTON_H
