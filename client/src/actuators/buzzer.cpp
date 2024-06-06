#include <Arduino.h>
#include "actuators/buzzer.h"
#include "config.h"

void Buzzer::initialize() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void Buzzer::set_alarm(bool status) {
    alarm_status = status;
    if (alarm_status) {
        last_change_time = millis();
        sound_currently_on = true;
        tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
    } else {
        noTone(BUZZER_PIN);
    }
}

void Buzzer::tick() {
    // Fix sound timing being inconsistent (TODO)
    if (!alarm_status || millis() - last_change_time < BUZZER_INTERVAL_MS) return;
    last_change_time = millis();

    // Flip sound status
    sound_currently_on = !sound_currently_on;
    if (sound_currently_on) tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
    else noTone(BUZZER_PIN);
}
