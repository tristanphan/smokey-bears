#include "sensors/gas.h"

void GasSensor::initialize() {
    // This is empty, but kept for consistency
}

int GasSensor::get_gas_level() {
    int analog_value = analogRead(MQ2_PIN);
    return map(analog_value, 0, 4095, 0, 100);
}
