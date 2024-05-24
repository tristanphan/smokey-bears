#ifndef CS147_CONFIG_H
#define CS147_CONFIG_H

static const char *HOSTNAME = "MY_HOSTNAME";
static const int PORT = 5000;
static const char *PATH = "/update?temperature=%.4f&gas_level=%.4f";

static const int NETWORK_TIMEOUT = 30 * 1000;
static const int NETWORK_DELAY = 1000;

// Sensors
static const int MQ2_PIN = 32;
// The DHT20 temperature sensor uses I2C

#endif //CS147_CONFIG_H
