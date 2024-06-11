#ifndef SMOKEY_TEST_BUTTON_H
#define SMOKEY_TEST_BUTTON_H

#include "config.h"
#include <Arduino.h>


// This button works as both a button to start the alarm and reset it.
class TestButton {
public:
    void initialize();

    bool is_pressed();
};


#endif //SMOKEY_TEST_BUTTON_H
