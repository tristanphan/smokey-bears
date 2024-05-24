#ifndef CS147_DISPLAY_H
#define CS147_DISPLAY_H

#include "TFT_eSPI.h"
#include <Arduino.h>

class Display {
public:
    Display() : current_lines{NULL}, current_num_lines{0}, current_alarm_status(false) {};

    void initialize();

    void update(String *lines, int num_lines, bool alarm_status);

private:
    TFT_eSPI tft;
    String *current_lines;
    int current_num_lines;
    bool current_alarm_status;

    void set_current_text(String *lines, int num_lines);

    bool is_lines_equal(String *lines, int num_lines);
};


#endif //CS147_DISPLAY_H
