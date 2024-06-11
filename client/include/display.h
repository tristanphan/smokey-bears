#ifndef SMOKEY_DISPLAY_H
#define SMOKEY_DISPLAY_H

#include "TFT_eSPI.h"
#include <Arduino.h>

class Display {
public:
    Display() : current_lines{nullptr}, current_num_lines{0}, current_background_color(TFT_BLACK) {};

    void initialize();

    void update(String *lines, int num_lines, uint16_t background_color);

    void update_line(const String &line, int line_num, uint16_t background_color);

private:
    TFT_eSPI tft;
    String *current_lines;
    int current_num_lines;
    uint16_t current_background_color;

    void set_current_text(String *lines, int num_lines);

    bool is_lines_equal(String *lines, int num_lines);

    void draw_lines();

};


#endif //SMOKEY_DISPLAY_H
