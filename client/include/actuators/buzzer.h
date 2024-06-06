#ifndef CS147_BUZZER_H
#define CS147_BUZZER_H


class Buzzer {
public:
    void initialize();

    void set_alarm(bool status);

    void tick();

private:
    bool alarm_status;

    // Only used when [alarm_status] is true
    unsigned long last_change_time;
    bool sound_currently_on;
};


#endif //CS147_BUZZER_H
