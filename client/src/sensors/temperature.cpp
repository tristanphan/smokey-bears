#include "sensors/temperature.h"

void TemperatureSensor::initialize() {
    Wire.begin();
    this->dht.begin();
}

bool TemperatureSensor::read() {
    int status = this->dht.read();
    switch (status) {
        case DHT20_OK:
            Serial.println("[TEMPERATURE SENSOR] Successfully read");
            return true;
        case DHT20_ERROR_CHECKSUM:
            Serial.println("[TEMPERATURE SENSOR ERROR] Checksum error");
            return false;
        case DHT20_ERROR_CONNECT:
            Serial.println("[TEMPERATURE SENSOR ERROR] Connect error");
            return false;
        case DHT20_MISSING_BYTES:
            Serial.println("[TEMPERATURE SENSOR ERROR] Missing bytes");
            return false;
        case DHT20_ERROR_BYTES_ALL_ZERO:
            Serial.println("[TEMPERATURE SENSOR ERROR] All bytes read zero");
            return false;
        case DHT20_ERROR_READ_TIMEOUT:
            Serial.println("[TEMPERATURE SENSOR ERROR] Read time out");
            return false;
        case DHT20_ERROR_LASTREAD:
            Serial.println("[TEMPERATURE SENSOR ERROR] Error read too fast");
            return false;
        default:
            Serial.println("[TEMPERATURE SENSOR ERROR] Unknown error");
            return false;
    }
}

float TemperatureSensor::get_temperature() {
    while (!read()) {
        Serial.printf("Failed to read temperature_sensor data\n");
        delay(1000);
    }

    float temperature = this->dht.getTemperature();
    Serial.printf("[TEMPERATURE SENSOR] Temperature: %.2f\n", temperature);
    return temperature;
}
