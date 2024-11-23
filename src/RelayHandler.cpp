#include "RelayHandler.hpp"

#include "Memory.hpp"

void RelayHandler::init()
{
    for (int i = 0; i < ButtonCount; i++) {
        pinMode(buttons_[i], INPUT_PULLUP);
    }

    for (int i = 0; i < SpeakerCount; i++) {
        pinMode(speakers_[i], OUTPUT);
        digitalWrite(speakers_[i], LOW);
    }

    // Load the last state of SelectedSpeaker from EEPROM
    selectedSpeaker_ = Memory::GetSelectedSpeaker();
    if (selectedSpeaker_ >= SpeakerCount) {
        selectedSpeaker_ = 0; // Default to 0 if the value is invalid`
        Memory::SaveSelectedSpeaker(selectedSpeaker_);
    }
    Serial.printf("Initial selected speaker: %d\n", selectedSpeaker_);

    setSpeaker();
}

void RelayHandler::setSpeaker(uint8_t speaker)
{
    if (speaker < SpeakerCount && speaker != selectedSpeaker_) {
        selectedSpeaker_ = speaker;
        Memory::SaveSelectedSpeaker(selectedSpeaker_);
    }
    setSpeaker();
}

uint8_t RelayHandler::getSelectedSpeaker()
{
    return selectedSpeaker_;
}

void RelayHandler::measureAndSetSpeaker()
{
    uint8_t pressedButton = 0;
    uint8_t pressedCount = 0;

    for (int i = 0; i < ButtonCount; i++) {
        if (digitalRead(buttons_[i]) == LOW) {
            pressedButton = i;
            pressedCount++;
        }
    }                                                                                                                                                                                                                                                                                                                

    if (pressedCount == 1 &&
        selectedSpeaker_ != pressedButton &&
        !speakerChanged)
    {
        speakerChanged = true;
        selectedSpeaker_ = pressedButton;
        Memory::SaveSelectedSpeaker(selectedSpeaker_);
        setSpeaker();
    }
}

// Private funtions 
void RelayHandler::setSpeaker()
{
    for (int i = 0; i < SpeakerCount; i++) {
        if (i == selectedSpeaker_) {
            digitalWrite(speakers_[i], HIGH);
            Serial.printf("Speaker %d is set HIGH\n", i);
        } else {
            digitalWrite(speakers_[i], LOW);
        }
    }
}