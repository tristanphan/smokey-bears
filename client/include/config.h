#ifndef SMOKEY_CONFIG_H
#define SMOKEY_CONFIG_H

static const char *HOSTNAME = "MY_HOSTNAME";
static const int PORT = 5000;
static const char *UPDATE_PATH = "/update?temperature=%.4f&gas_level=%.4f";
static const char *SET_ALARM_PATH = "/set_alarm?status=%s";

static const int REFRESH_INTERVAL = 500;

static const int NETWORK_TIMEOUT = 30 * 1000;
static const int NETWORK_DELAY = 1000;

static const int DISPLAY_TEXT_PADDING = 10;
static const int BUZZER_FREQUENCY_HZ = 554;  // C#4 (554 Hz)
static const int BUZZER_INTERVAL_MS = 100;

// Sensors
static const int MQ2_PIN = 32;
// The DHT20 temperature sensor uses I2C
static const int TEST_BUTTON_PIN = 33;
static const int BUZZER_PIN = 15;
static const int VALVE_PIN = 27;

#endif //SMOKEY_CONFIG_H
