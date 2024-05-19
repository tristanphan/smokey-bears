#include <Arduino.h>
#include <WiFi.h>
#include <cstdio>

#include "wifi_manager.h"
#include "sensor.h"
#include "config.h"

WiFiManager wifi{};
Sensor sensor{};


void setup() {
    Serial.begin(9600);
    delay(1000);

    String ssid;
    String password;
    WiFiManager::retrieve_credentials(&ssid, &password);
    WiFiManager::connect(ssid, password);
    sensor.initialize();
}

void loop() {
    if (!sensor.read()) {
        Serial.printf("Failed to read sensor data\n");
        delay(1000);
        return;
    }
    float temperature = sensor.get_temperature();
    float humidity = sensor.get_humidity();
    char path[50];
    sprintf(path, PATH, temperature, humidity);
    String body = wifi.get(HOSTNAME, PORT, path);
    wifi.stop();
    Serial.printf("Response: %s\n", body.c_str());
    delay(1000);
}