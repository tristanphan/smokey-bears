#include <Arduino.h>
#include <WiFi.h>
#include <cstdio>

#include "wifi_manager.h"
#include "config.h"
#include "sensors/temperature.h"
#include "sensors/gas.h"

WiFiManager wifi{};

// Sensors
TemperatureSensor temperature_sensor{};
GasSensor gas_sensor{};


void setup() {
    Serial.begin(9600);
    delay(1000);

    String ssid;
    String password;
    WiFiManager::retrieve_credentials(&ssid, &password);
    WiFiManager::connect(ssid, password);

    // Initialize sensors
    temperature_sensor.initialize();
    gas_sensor.initialize();
}

void loop() {
    float temperature = temperature_sensor.get_temperature();
    float humidity = gas_sensor.get_gas_level();
    char path[50];
    sprintf(path, PATH, temperature, humidity);
    String body = wifi.get(HOSTNAME, PORT, path);
    Serial.printf("Response: %s\n", body.c_str());
    wifi.stop();
    delay(1000);
}