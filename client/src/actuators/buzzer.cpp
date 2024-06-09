#include <Arduino.h>
#include "actuators/buzzer.h"
#include "config.h"

[[noreturn]] void buzzer_task(void *) {
    int frequency = (1000000 / BUZZER_FREQUENCY_HZ) / 2;
    while (true) {
        unsigned long last_change_time = millis();

        // On interval
        while (millis() - last_change_time < BUZZER_INTERVAL_MS) {
            digitalWrite(BUZZER_PIN, HIGH);
            delayMicroseconds(frequency);
            digitalWrite(BUZZER_PIN, LOW);
            delayMicroseconds(frequency);
        }

        // Off interval
        delay(BUZZER_INTERVAL_MS);
    }
}

void Buzzer::initialize() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void Buzzer::set_alarm(bool status) {
    if (status && task == nullptr) {
        task = new TaskHandle_t;
        // NOTE: [delay()] only works because there are <= 2 (pinned) tasks and 2 cores
        xTaskCreatePinnedToCore(buzzer_task, "buzzer_task", 2048, nullptr, tskIDLE_PRIORITY, task,
                                1 - xPortGetCoreID());
        Serial.printf("[BUZZER] Alarm is on\n");
    }
    if (!status && task != nullptr) {
        vTaskDelete(*task);
        delete task;
        task = nullptr;
        Serial.printf("[BUZZER] Alarm is off\n");
    }
}
