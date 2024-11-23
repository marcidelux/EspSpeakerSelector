#include "NetworkSettings.hpp"
#include <Arduino.h>

#include "Memory.hpp"
#include "page.hpp"

NetworkSettings::NetworkSettings() : server(80) {
    pinMode(NETWORK_BUTTON, INPUT_PULLUP);
}

void NetworkSettings::begin() {
    if (digitalRead(NETWORK_BUTTON) == LOW) {
        delay(3000);
        if (digitalRead(NETWORK_BUTTON) == LOW) {
            WiFi.softAP("ADAM_AUDIO_SETTINGS");

            server.on("/", std::bind(&NetworkSettings::handleRoot, this));
            server.on("/submit", std::bind(&NetworkSettings::handleSubmit, this));
            server.begin();
            Serial.println("HTTP server started in AP mode");

            apMode = true;
        }
    }
    
    readNetworkSettings();
}

void NetworkSettings::handleClient() {
    server.handleClient();
}

void NetworkSettings::handleRoot() {
    server.send(200, "text/html", networkSettingsPage);
}

void NetworkSettings::handleSubmit() {
    if (server.method() == HTTP_POST) {
        ssid = server.arg("ssid");
        password = server.arg("password");
        static_ip = server.arg("static_ip");
        gateway = server.arg("gateway");

        saveNetworkSettings();

        server.send(200, "text/html", "Settings received");
    } else {
        server.send(405, "text/html", "Method Not Allowed");
    }
}

void NetworkSettings::saveNetworkSettings() {
    Memory::SetSSID(ssid);
    Memory::SetPassword(password);
    Memory::SetStaticIP(static_ip);
    Memory::SetGateway(gateway);
    Serial.println("Save Network Settings:");
    Serial.println("SSID: " + ssid);
    Serial.println("Password: " + password);
    Serial.println("Static IP: " + static_ip);
    Serial.println("Gateway: " + gateway);
}

void NetworkSettings::readNetworkSettings() {
    ssid = Memory::GetSSID();
    password = Memory::GetPassword();
    static_ip = Memory::GetStaticIP();
    gateway = Memory::GetGateway();

    Serial.println("Read Network Settings:");
    Serial.println("SSID: " + ssid);
    Serial.println("Password: " + password);
    Serial.println("Static IP: " + static_ip);
    Serial.println("Gateway: " + gateway);
}