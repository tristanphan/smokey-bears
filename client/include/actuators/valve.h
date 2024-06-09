#ifndef CS147_VALVE_H
#define CS147_VALVE_H

#include <Servo.h>


class Valve {
public:
    void initialize();

    void set_alarm(bool alarm_status);

private:
    Servo servo{};
};


#endif //CS147_VALVE_H
