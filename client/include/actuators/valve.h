#ifndef SMOKEY_VALVE_H
#define SMOKEY_VALVE_H

#include <Servo.h>


class Valve {
public:
    void initialize();

    void set_alarm(bool alarm_status);

private:
    Servo servo{};
};


#endif //SMOKEY_VALVE_H
