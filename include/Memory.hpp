#pragma once
#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 512
#define SELECTED_SPEAKER_LENGTH 1
#define SSID_LENGTH 32
#define PASSWORD_LENGTH 32
#define STATIC_IP_LENGTH 32
#define GATEWAY_LENGTH 32

#define POSITION_SELECTED_SPEAKER 0
#define POSITION_SSID POSITION_SELECTED_SPEAKER + SELECTED_SPEAKER_LENGTH
#define POSITION_PASSWORD POSITION_SSID + SSID_LENGTH
#define POSITION_STATIC_IP POSITION_PASSWORD + PASSWORD_LENGTH
#define POSITION_GATEWAY POSITION_STATIC_IP + STATIC_IP_LENGTH

class Memory {
public:
    static void Init();
    static uint8_t GetSelectedSpeaker();
    static void SaveSelectedSpeaker(uint8_t speaker);
    static String GetSSID();
    static void SetSSID(const String& ssid);
    static String GetPassword();
    static void SetPassword(const String& password);
    static String GetStaticIP();
    static void SetStaticIP(const String& staticIP);
    static String GetGateway();
    static void SetGateway(const String& gateway);

private:
    static String ReadStringFromEEPROM(int address, int length);
    static void WriteStringToEEPROM(int address, const String& str, int length);
};
