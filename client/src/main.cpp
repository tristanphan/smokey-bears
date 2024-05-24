#include <Arduino.h>
#include <WiFi.h>
#include <cstdio>

#include "wifi_manager.h"
#include "display.h"
#include "config.h"
#include "sensors/temperature.h"
#include "sensors/gas.h"

WiFiManager wifi{};
Display display{};

// Sensors
TemperatureSensor temperature_sensor{};
GasSensor gas_sensor{};


void setup() {
    Serial.begin(9600);
    delay(1000);

    display.initialize();
    display.update(new String[2]{"Initializing...", "Please wait"}, 2, false);

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
    float gas_level = gas_sensor.get_gas_level();
    char path[50];
    sprintf(path, PATH, temperature, gas_level);
    String body = wifi.get(HOSTNAME, PORT, path);
    Serial.printf("Response: %s\n", body.c_str());
    wifi.stop();

    String lines[] = {
            String("Temp: ") + temperature + " C",
            String("Gas Lvl: ") + gas_level + "%",
            String("Alarm: ") + "off",
    };
    display.update(lines, 3, false);

    delay(1000);
}