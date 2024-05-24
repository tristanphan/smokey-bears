#include "sensors/gas.h"

void GasSensor::initialize() {
    // This is empty, but kept for consistency
}

float GasSensor::get_gas_level() {
    int analog_value = analogRead(MQ2_PIN);
    float percentage = ((float) analog_value / 4096) * 100;
    Serial.printf("[GAS SENSOR] Gas Level: %d / 4096 ≈ %.2f / 100\n", analog_value, percentage);
    return percentage;
}
