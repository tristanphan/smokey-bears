#ifndef CS147_TEMPERATURE_SENSOR_H
#define CS147_TEMPERATURE_SENSOR_H

#include "DHT20.h"

class TemperatureSensor {
public:
    TemperatureSensor() : dht{} {}

    void initialize();

    bool read();

    float get_temperature();

private:
    DHT20 dht;
};


#endif //CS147_TEMPERATURE_SENSOR_H
