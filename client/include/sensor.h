#ifndef LAB4_SENSOR_H
#define LAB4_SENSOR_H

#include <DHT20.h>

class Sensor {
public:
    Sensor() : dht{} {}

    void initialize();

    bool read();

    float get_temperature();

    float get_humidity();

private:
    DHT20 dht;
};


#endif //LAB4_SENSOR_H
