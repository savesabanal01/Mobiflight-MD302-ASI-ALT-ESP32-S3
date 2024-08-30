#pragma once

#include <Arduino.h>
#include "MD302_ASI_ALT.h"

// only one entry required if you have only one custom device
enum {
    MD302_ASI_ALT_DEVICE_1 = 1
};
class MFCustomDevice
{
public:
    MFCustomDevice();
    void attach(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig, bool configFromFlash = false);
    void detach();
    void update();
    void set(int16_t messageID, char *setPoint);

private:
    bool           getStringFromMem(uint16_t addreeprom, char *buffer, bool configFromFlash);
    bool           _initialized = false;
    MD302_ASI_ALT *_myASI_ALT_device;
    uint8_t        _pin1, _pin2, _pin3;
    uint8_t        _customType = 0;
};
