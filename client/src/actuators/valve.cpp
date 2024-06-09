#include "actuators/valve.h"
#include "config.h"

void Valve::initialize() {
    pinMode(VALVE_PIN, OUTPUT);
    servo.attach(VALVE_PIN);
    servo.write(0);
}

void Valve::set_alarm(bool alarm_status) {
    int angle = alarm_status ? 179 : 0;
    Serial.printf("[VALVE] Setting servo angle to %d\n", angle);
//    servo.write(angle);
}
