#include "sensor.h"

void Sensor::initialize() {
    Wire.begin();
    this->dht.begin();
}

bool Sensor::read() {
    int status = this->dht.read();
    switch (status) {
        case DHT20_OK:
            Serial.println("Sensor successfully read");
            return true;
        case DHT20_ERROR_CHECKSUM:
            Serial.println("[SENSOR ERROR] Checksum error");
            return false;
        case DHT20_ERROR_CONNECT:
            Serial.println("[SENSOR ERROR] Connect error");
            return false;
        case DHT20_MISSING_BYTES:
            Serial.println("[SENSOR ERROR] Missing bytes");
            return false;
        case DHT20_ERROR_BYTES_ALL_ZERO:
            Serial.println("[SENSOR ERROR] All bytes read zero");
            return false;
        case DHT20_ERROR_READ_TIMEOUT:
            Serial.println("[SENSOR ERROR] Read time out");
            return false;
        case DHT20_ERROR_LASTREAD:
            Serial.println("[SENSOR ERROR] Error read too fast");
            return false;
        default:
            Serial.println("[SENSOR ERROR] Unknown error");
            return false;
    }
}

float Sensor::get_temperature() {
    float temperature = this->dht.getTemperature();
    Serial.printf("Temperature: %.2f\n", temperature);
    return temperature;
}

float Sensor::get_humidity() {
    float humidity = this->dht.getHumidity();
    Serial.printf("Humidity: %.2f\n", humidity);
    return humidity;
}