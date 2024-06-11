#ifndef SMOKEY_GAS_SENSOR_H
#define SMOKEY_GAS_SENSOR_H

#include "config.h"
#include <Arduino.h>

class GasSensor {
public:
    void initialize();

    float get_gas_level(); // percentage of gas level

};


#endif //SMOKEY_GAS_SENSOR_H
