#include "Memory.hpp"

void Memory::Init() {
    EEPROM.begin(EEPROM_SIZE);
}

uint8_t Memory::GetSelectedSpeaker() {
    return EEPROM.read(POSITION_SELECTED_SPEAKER);
}

void Memory::SaveSelectedSpeaker(uint8_t speaker) {
    EEPROM.write(POSITION_SELECTED_SPEAKER, speaker);
    EEPROM.commit();
}

String Memory::GetSSID() {
    return ReadStringFromEEPROM(POSITION_SSID, SSID_LENGTH);
}

void Memory::SetSSID(const String& ssid) {
    WriteStringToEEPROM(POSITION_SSID, ssid, SSID_LENGTH);
}

String Memory::GetPassword() {
    return ReadStringFromEEPROM(POSITION_PASSWORD, PASSWORD_LENGTH);
}

void Memory::SetPassword(const String& password) {
    WriteStringToEEPROM(POSITION_PASSWORD, password, PASSWORD_LENGTH);
}

String Memory::GetStaticIP() {
    return ReadStringFromEEPROM(POSITION_STATIC_IP, STATIC_IP_LENGTH);
}

void Memory::SetStaticIP(const String& staticIP) {
    WriteStringToEEPROM(POSITION_STATIC_IP, staticIP, STATIC_IP_LENGTH);
}

String Memory::GetGateway() {
    return ReadStringFromEEPROM(POSITION_GATEWAY, GATEWAY_LENGTH);
}

void Memory::SetGateway(const String& gateway) {
    WriteStringToEEPROM(POSITION_GATEWAY, gateway, GATEWAY_LENGTH);
}

String Memory::ReadStringFromEEPROM(int address, int length) {
    char data[length + 1];
    int i;
    for (i = 0; i < length; i++) {
        data[i] = EEPROM.read(address + i);
        if (data[i] == '\0') {
            break;
        }
    }
    data[i] = '\0';
    return String(data);
}

void Memory::WriteStringToEEPROM(int address, const String& str, int length) {
    int strLength = str.length();
    for (int i = 0; i < length; i++) {
        if (i < strLength) {
            EEPROM.write(address + i, str[i]);
        } else {
            EEPROM.write(address + i, 0);
        }
    }
    EEPROM.commit();
}