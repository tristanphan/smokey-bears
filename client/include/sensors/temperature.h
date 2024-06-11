#ifndef SMOKEY_TEMPERATURE_SENSOR_H
#define SMOKEY_TEMPERATURE_SENSOR_H

#include "DHT20.h"

class TemperatureSensor {
public:
    TemperatureSensor() : dht{} {}

    void initialize();

    float get_temperature();

private:
    DHT20 dht;

    bool read();
};


#endif //SMOKEY_TEMPERATURE_SENSOR_H
