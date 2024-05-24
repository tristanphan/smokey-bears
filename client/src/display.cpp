#include "display.h"
#include "TFT_eSPI.h"
#include "config.h"

void Display::initialize() {
    // Initialize the screen
    tft.init();
    tft.setRotation(1); // 0-3 CCW

    // Initialize text
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM); // Middle Center Datum
}

void Display::update(String *lines, int num_lines, bool alarm_status) {
    if (is_lines_equal(lines, num_lines) && alarm_status == current_alarm_status) return;
    set_current_text(lines, num_lines);

    if (alarm_status) tft.fillScreen(TFT_RED);
    else tft.fillScreen(TFT_BLACK);

    int line_height = tft.fontHeight();
    int space_between_midpoints = line_height + DISPLAY_TEXT_PADDING;
    int space_between_first_last_midpoints = space_between_midpoints * (num_lines - 1);
    int height = tft.height() / 2 - space_between_first_last_midpoints / 2;
    for (int i = 0; i < num_lines; i++) {
        tft.drawString(lines[i], tft.width() / 2, height);
        height += space_between_midpoints;
    }
}

void Display::set_current_text(String *lines, int num_lines) {
    if (current_lines != NULL) delete[] current_lines;

    current_lines = new String[num_lines];
    current_num_lines = num_lines;

    for (int i = 0; i < num_lines; i++) {
        current_lines[i] = lines[i];
    }
}

bool Display::is_lines_equal(String *lines, int num_lines) {
    if (num_lines != current_num_lines) return false;

    for (int i = 0; i < num_lines; i++) {
        if (lines[i] != current_lines[i]) return false;
    }

    return true;
}
