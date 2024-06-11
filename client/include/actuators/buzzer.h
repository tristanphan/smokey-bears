#ifndef SMOKEY_BUZZER_H
#define SMOKEY_BUZZER_H


class Buzzer {
public:
    void initialize();

    void set_alarm(bool status);

private:
    TaskHandle_t* task = nullptr;
};


#endif //SMOKEY_BUZZER_H
