#ifndef CS147_GAS_SENSOR_H
#define CS147_GAS_SENSOR_H

#include "config.h"
#include <Arduino.h>

class GasSensor {
public:
    void initialize();

    int get_gas_level(); // percentage of gas level

};


#endif //CS147_GAS_SENSOR_H
