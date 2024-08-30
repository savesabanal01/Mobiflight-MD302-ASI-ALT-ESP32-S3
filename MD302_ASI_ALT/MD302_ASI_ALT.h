#pragma once

#include "Arduino.h"

class MD302_ASI_ALT
{
public:
    MD302_ASI_ALT(uint8_t Pin1, uint8_t Pin2);
    void begin();
    void attach(uint16_t Pin3, char *init);
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();

private:
    bool    _initialised;
    uint8_t _pin1, _pin2, _pin3;
    // Variables
    float airSpeed = 0; // Air speed value from the sim
    float altitude = 0; // Altitude Value from the simulator
    float baro = 0;
    float heading = 0;
    float instrumentBrightnessRatio = 1;
    int instrumentBrightness = 255;
    int screenRotation = 0;
    int prevScreenRotation = 0;
    bool powerSaveFlag = false;

    // Variables to store the 1000s, 100s, 10s and 1s
    int speedHundreds = 0; // Speed hundreds value
    int speedTens = 0;     // Speed Tens value
    int speedOnes = 0;     // Speed Ones value

    int altTenThousands = 0; // Altitude ten thousands value
    int altThousands = 0;    // Altitude thousands value
    int altHundreds = 0;     // Altitude hundreds value
    int altTens = 0;         // Altitude tens value
    int altOnes = 0;         // Altitude ones value

    float startTime;
    float endTime;

    // Function declarations
    float scaleValue(float x, float in_min, float in_max, float out_min, float out_max);
    void setAirSpeed(float value);
    void setAltitude(float value);
    void setBaro(float value);
    void setHeading(float value);
    void setInstrumentBrightnessRatio(float value);
    void setScreenRotation(int value);
    void drawSpeedIndicator();
    void drawAltitudeIndicator();
    void drawSpeedIndicatorLines();
    void drawAltitudeIndicatorLines();
    void drawSpeedIndicatorValues();
    void drawAltitudeIndicatorValues();
    void drawAll();
    void setPowerSaveMode(bool mode);
};