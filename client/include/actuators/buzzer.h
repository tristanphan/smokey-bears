#ifndef CS147_BUZZER_H
#define CS147_BUZZER_H


class Buzzer {
public:
    void initialize();

    void set_alarm(bool status);

private:
    TaskHandle_t* task = nullptr;
};


#endif //CS147_BUZZER_H
