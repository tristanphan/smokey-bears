#ifndef CS147_WIFI_MANAGER_H
#define CS147_WIFI_MANAGER_H


#include "HttpClient.h"

class WiFiManager {
public:
    WiFiManager()
            : wifi_client{}, http_client{wifi_client} {}

    static void retrieve_credentials(String *ssid, String *password);

    static void connect(const String &ssid, const String &password);

    String get(const String &hostname, int port, const String &path);

    void stop();

private:
    WiFiClient wifi_client;
    HttpClient http_client;

};


#endif //CS147_WIFI_MANAGER_H
