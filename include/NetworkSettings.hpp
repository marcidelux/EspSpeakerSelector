#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string>

#define NETWORK_BUTTON D0

class NetworkSettings {
public:
    NetworkSettings();
    void begin();
    void handleClient();
    String getSSID() { return ssid; }
    String getPassword() { return password; }
    String getStaticIP() { return static_ip; }
    String getGateway() { return gateway; }
    bool GetAPMode() { return apMode; }

private:
    ESP8266WebServer server;
    String ssid;
    String password;
    String static_ip;
    String gateway;
    bool apMode = false;
    void handleRoot();
    void handleSubmit();
    void readNetworkSettings();
    void saveNetworkSettings();
};
