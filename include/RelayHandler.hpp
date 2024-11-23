#pragma once
#include <Arduino.h>

// Pin definitions for digital outputs
#define SpeakerA        D5
#define SpeakerB        D6
#define SpeakerC        D7
#define SpeakerD        D8

// Pin definitions for digital inputs
#define ButtonA         D1
#define ButtonB         D2
#define ButtonC         D3
#define ButtonD         D4

#define ButtonCount     4
#define SpeakerCount    4

class RelayHandler {
public:
    bool speakerChanged = false;
    void init();
    void measureAndSetSpeaker();
    void setSpeaker(uint8_t speaker);
    uint8_t getSelectedSpeaker();
private:
    void setSpeaker();
    uint8_t selectedSpeaker_ = 0;
    uint8_t speakers_[4] = {SpeakerA, SpeakerB, SpeakerC, SpeakerD};
    uint8_t buttons_[4] = {ButtonA, ButtonB, ButtonC, ButtonD};
};
