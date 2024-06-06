#include <Arduino.h>
#include <WiFi.h>
#include <cstdio>
#include <ArduinoJson.h>

#include "wifi_manager.h"
#include "display.h"
#include "config.h"
#include "sensors/temperature.h"
#include "sensors/gas.h"
#include "sensors/test_button.h"

bool alarm_status = false;

WiFiManager wifi{};
Display display{};

// Sensors
TemperatureSensor temperature_sensor{};
GasSensor gas_sensor{};
TestButton test_button{};


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
    test_button.initialize();
}

void loop() {
    // Get readings
    float temperature = temperature_sensor.get_temperature();
    float gas_level = gas_sensor.get_gas_level();

    // Send readings to server
    char update_path[50];
    sprintf(update_path, UPDATE_PATH, temperature, gas_level);
    String update_response_body = wifi.get(HOSTNAME, PORT, update_path);
    Serial.printf("Response: %s\n", update_response_body.c_str());
    wifi.stop();

    // Extract alarm status
    bool previous_alarm_status = alarm_status;
    JsonDocument document;
    deserializeJson(document, update_response_body);
    if (document.containsKey("alarm_status")) {
        alarm_status = document["alarm_status"].as<bool>();
    }

    // Change actuator status if alarm status changed
    if (alarm_status != previous_alarm_status) {
        // Change actuator status (TODO)
    }

    // Check test button
    if (test_button.is_pressed()) {
        char set_alarm_path[50];
        sprintf(set_alarm_path, SET_ALARM_PATH, alarm_status ? "off" : "on");
        String set_alarm_response_body = wifi.get(HOSTNAME, PORT, set_alarm_path);
        Serial.printf("Response: %s\n", set_alarm_response_body.c_str());
        wifi.stop();
    }

    String lines[] = {
            String("Temp: ") + temperature + " C",
            String("Gas Lvl: ") + gas_level + "%",
            String("Alarm: ") + "off",
    };
    display.update(lines, 3, false);

    delay(200);
}