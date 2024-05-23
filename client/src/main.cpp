#include <Arduino.h>
#include <WiFi.h>
#include <cstdio>

#include "wifi_manager.h"
#include "config.h"
#include "sensors/temperature_sensor.h"

WiFiManager wifi{};
TemperatureSensor temperature_sensor{};


void setup() {
    Serial.begin(9600);
    delay(1000);

    String ssid;
    String password;
    WiFiManager::retrieve_credentials(&ssid, &password);
    WiFiManager::connect(ssid, password);
    temperature_sensor.initialize();
}

void loop() {
    bool temperature_success = temperature_sensor.read();
    if (!temperature_success) {
        Serial.printf("Failed to read temperature_sensor data\n");
        delay(1000);
        return;
    }

    float temperature = temperature_sensor.get_temperature();
    float humidity = 0; // TODO Change to gas temperature_sensor
    char path[50];
    sprintf(path, PATH, temperature, humidity);
    String body = wifi.get(HOSTNAME, PORT, path);
    Serial.printf("Response: %s\n", body.c_str());
    wifi.stop();
    delay(1000);
}