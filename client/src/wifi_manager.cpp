#include <nvs_flash.h>
#include <Arduino.h>
#include <WiFi.h>
#include <sstream>
#include "wifi_manager.h"
#include "config.h"

void WiFiManager::retrieve_credentials(String *ssid, String *password) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    Serial.printf("\nOpening Non-Volatile Storage (NVS) handle... ");
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    Serial.printf("Done\n");

    Serial.printf("Retrieving SSID/PASSWD... ");
    char ssid_buffer[50];
    char pass_buffer[50];
    size_t ssid_len = 50;
    size_t pass_len = 50;
    err = nvs_get_str(my_handle, "ssid", ssid_buffer, &ssid_len);
    err |= nvs_get_str(my_handle, "password", pass_buffer, &pass_len);
    *ssid = String(ssid_buffer);
    *password = String(pass_buffer);
    switch (err) {
        case ESP_OK:
            Serial.printf("Done\n");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            Serial.printf("The value is not initialized yet!\n");
            break;
        default:
            Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
    Serial.printf("SSID = %s\n", ssid->c_str());
    Serial.printf("PASSWD = %s\n", password->c_str());

    // Clos
    nvs_close(my_handle);
}

void WiFiManager::connect(const String &ssid, const String &password) {
    Serial.printf("\n\nConnecting to %s\n", ssid.c_str());
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nWiFi connected\n");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("MAC address: %s\n", WiFi.macAddress().c_str());
}

String WiFiManager::get(const String &hostname, int port, const String &path) {
    // Make the request to the server
    int err = http_client.get(hostname.c_str(), port, path.c_str(), nullptr);
    if (err != 0) {
        Serial.printf("[HTTP CLIENT ERROR] Connect failed: %d\n", err);
        return String{};
    }

    // Parse the response code
    Serial.printf("[WIFI MODULE] Making request to %s at %s\n", hostname.c_str(), path.c_str());
    int status_code = http_client.responseStatusCode();
    if (status_code < 0) {
        Serial.printf("[HTTP CLIENT ERROR] Getting response failed: %d\n", status_code);
        return String{};
    }
    Serial.printf("Got status code: %d\n", status_code);

    // Skip through the response header section
    err = http_client.skipResponseHeaders();
    if (err < 0) {
        Serial.printf("[HTTP CLIENT ERROR] Failed to skip response headers: %d\n", err);
        return String{};
    }

    // Get body
    unsigned long timeoutStart = millis();
    std::ostringstream oss;
    while ((http_client.connected() || http_client.available()) &&
           ((millis() - timeoutStart) < NETWORK_TIMEOUT)) {
        if (!http_client.available()) {
            delay(NETWORK_DELAY);
            continue;
        }
        oss << (char) http_client.read();
        timeoutStart = millis();
    }
    return {oss.str().c_str()};
}

void WiFiManager::stop() {
    http_client.stop();
}
