#include "MD302_ASI_ALT.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <altitude_indicator_box_2.h>
#include <speed_indicator_box_2.h>
#include <baro_box.h>
#include <heading_box.h>
#include <B612Font18.h>
#include <B612Font20.h>
#include <B612Font28.h>
#include <logo.h>

#define digitsS B612Font18
#define digitsM B612Font20
#define digitsL B612Font28


TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

TFT_eSprite mainSpr = TFT_eSprite(&tft);           // Sprite to hold the speed indicator
TFT_eSprite SpeedIndicatorSpr = TFT_eSprite(&tft); // Sprite to hold the speed indicator
TFT_eSprite AltitudeIndSpr = TFT_eSprite(&tft);    // Sprite to hold the altitude indicator
TFT_eSprite headingBoxSpr = TFT_eSprite(&tft);     // Sprite to hold heading Box
TFT_eSprite speedIndBoxSpr = TFT_eSprite(&tft);    // Sprite to hold speed indicator box
TFT_eSprite altIndBoxSpr = TFT_eSprite(&tft);      // Sprite to hold altitude indicator box
TFT_eSprite baroBoxSpr = TFT_eSprite(&tft);        // Sprite to holdt the Baro Box

#define BROWN 0x80C3    // 0x5960
#define SKY_BLUE 0x255C // 0x0318 //0x039B //0x34BF
#define PANEL_COLOR 0x7BEE

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

MD302_ASI_ALT::MD302_ASI_ALT(uint8_t Pin1, uint8_t Pin2)
{
    _pin1 = Pin1;
    _pin2 = Pin2;
}

void MD302_ASI_ALT::begin()
{

      // put your setup code here, to run once:
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(screenRotation);
  tft.fillScreen(PANEL_COLOR);
  tft.setPivot(320, 160);
  tft.setSwapBytes(true);
  tft.pushImage(40, 80, 160, 160, logo);
  delay(3000);
  tft.fillScreen(TFT_BLACK);


  mainSpr.createSprite(240, 320);
  mainSpr.setSwapBytes(false);
  mainSpr.fillSprite(TFT_BLACK);

  SpeedIndicatorSpr.createSprite(120, 320);
  SpeedIndicatorSpr.setSwapBytes(false);
  SpeedIndicatorSpr.fillSprite(TFT_BLACK);

  AltitudeIndSpr.createSprite(120, 320);
  AltitudeIndSpr.setSwapBytes(false);
  AltitudeIndSpr.fillSprite(TFT_BLACK);

  altIndBoxSpr.createSprite(altitude_indicator_box_2_width, altitude_indicator_box_2_height);
  altIndBoxSpr.setSwapBytes(false);
  altIndBoxSpr.fillScreen(TFT_BLACK); // set blue background and use this for transparency later

  speedIndBoxSpr.createSprite(speed_indicator_box_2_width, speed_indicator_box_2_height);
  speedIndBoxSpr.setSwapBytes(false);
  speedIndBoxSpr.fillScreen(TFT_BLACK); // set blue background and use this for transparency later

  headingBoxSpr.createSprite(heading_box_width, heading_box_height);
  headingBoxSpr.setSwapBytes(false);
  headingBoxSpr.fillScreen(TFT_BLACK); // set blue background and use this for transparency later

  baroBoxSpr.createSprite(baro_box_width, baro_box_height);
  baroBoxSpr.setSwapBytes(false);
  baroBoxSpr.fillScreen(TFT_BLACK); // set blue background and use this for transparency later

}

void MD302_ASI_ALT::attach(uint16_t Pin3, char *init)
{
    _pin3 = Pin3;
}

void MD302_ASI_ALT::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void MD302_ASI_ALT::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    // int32_t  data = atoi(setPoint);
    // uint16_t output;
   switch (messageID) {
    // do something according your messageID
    case -1:
        // tbd., get's called when Mobiflight shuts down
        setPowerSaveMode(true);
        break;
    case -2:
        // tbd., get's called when PowerSavingMode is entered
        if(atoi(setPoint) == 1)
          setPowerSaveMode(true);
        else if (atoi(setPoint) == 0)
          setPowerSaveMode(false);
        break;
    case 0:
        // output = (uint16_t)data;
        // data   = output;
        setAirSpeed(atof(setPoint));
        break;
    case 1:
        setAltitude(atof(setPoint));
        /* code */
        break;
    case 2:
        /* code */
        setBaro(atof(setPoint));
        break;
      case 3:
        /* code */
        setHeading(atof(setPoint));
        break;
    case 4:
        /* code */
        setInstrumentBrightnessRatio(atof(setPoint));
        break;
    case 100:
        /* code */
        setScreenRotation(atoi(setPoint));
        break;
    default:
        break;
    }
}

void MD302_ASI_ALT::update()
{
    // Do something which is required regulary
  // if(!powerSaveFlag)
  // {
  //   analogWrite(TFT_BL, instrumentBrightness);
  //   if(prevScreenRotation != screenRotation)
  //   {
  //       tft.setRotation(screenRotation);
  //       prevScreenRotation = screenRotation;
  //   }
  //   drawSpeedIndicator();
  //   drawAltitudeIndicator();
  //   drawAll();
  //  }
  //  else digitalWrite(TFT_BL, LOW);
   
    analogWrite(TFT_BL, instrumentBrightness);
    drawSpeedIndicator();
    drawAltitudeIndicator();
    drawAll();
   
}

void MD302_ASI_ALT::drawSpeedIndicator()
{

  drawSpeedIndicatorLines();
  speedIndBoxSpr.pushImage(0, 0, speed_indicator_box_2_width, speed_indicator_box_2_height, speed_indicator_box_2);
  speedIndBoxSpr.setSwapBytes(true);
  drawSpeedIndicatorValues();
  speedIndBoxSpr.pushToSprite(&mainSpr, 21, 137, TFT_BLACK);

  headingBoxSpr.pushImage(0, 0, heading_box_width, heading_box_height, heading_box);
  headingBoxSpr.setSwapBytes(true);
  headingBoxSpr.setTextColor(TFT_GREEN);
  headingBoxSpr.setTextDatum(MC_DATUM);
  // headingBoxSpr.setFreeFont(FSSB12);
  headingBoxSpr.loadFont(digitsM);
  headingBoxSpr.drawString(String((int)round(heading)), heading_box_width / 2, heading_box_height / 2 + 4);
  headingBoxSpr.pushToSprite(&mainSpr, 87, 267, TFT_BLACK);
  // tft.setViewport(0, 0, 120, 320);

  // SpeedIndicatorSpr.pushSprite(0, 0);
  // mainSpr.pushToSprite(&mainSpr, 0, 0, TFT_BLACK);
  headingBoxSpr.fillSprite(TFT_BLACK);
  // mainSpr.fillSprite(TFT_BLACK);
}

void MD302_ASI_ALT::drawSpeedIndicatorLines()
{
  int yPosShortLines[8];
  int yPosLongLines[8];
  int speedValues[8];
  int i;

  int minSpeed; // minimum spped in range
  int maxSpeed; // maximum spped in range

  // minSpeed = round(airSpeed - 40.0);
  minSpeed = airSpeed - 40.0;
  // maxSpeed = round(airSpeed + 40.0);
  maxSpeed = airSpeed + 40.0;

  mainSpr.loadFont(digitsM);
  // SpeedIndicatorSpr.setFreeFont(d);
  mainSpr.setTextColor(TFT_WHITE);
  mainSpr.setTextDatum(ML_DATUM);

  // find the first airspeed value that has as "10" to draw long lines
  for (i = round(minSpeed); i <= round(maxSpeed); i++)
  {
    if ((i % 10) == 0) // found our first long line
    {
      // yPosLongLines[0] = round(scaleValue(i, minSpeed, maxSpeed, 320, 0));
      yPosLongLines[0] = scaleValue(i, minSpeed, maxSpeed, 320, 0);
      speedValues[0] = i;
      mainSpr.drawWideLine(15, yPosLongLines[0] + 2, 28, yPosLongLines[0] + 2, 3, TFT_WHITE, TFT_BLACK);
      if (speedValues[0] >= 0)
        mainSpr.drawString(String(speedValues[0]), 30, yPosLongLines[0] + 2);
      break;
    }
  }

  // Now populate the positions of the other long lines
  for (i = 1; i < 8; i++)
  {
    // yPosLongLines[i] = round(scaleValue(speedValues[0] + (i * 10), minSpeed, maxSpeed, 320, 0));
    yPosLongLines[i] = scaleValue(speedValues[0] + (i * 10), minSpeed, maxSpeed, 320, 0);
    mainSpr.drawWideLine(15, yPosLongLines[i] + 2, 28, yPosLongLines[i] + 2, 3, TFT_WHITE, TFT_BLACK);
    speedValues[i] = speedValues[0] + (i * 10);
    if (speedValues[i] >= 0)
      mainSpr.drawString(String(speedValues[i]), 30, yPosLongLines[i] + 2);
  }

  // find the first airspeed value that has as "5" to draw short lines
  for (i = minSpeed; i <= maxSpeed; i++)
  {
    if ((i % 5) == 0 && (i % 10) != 0) // found our first short line
    {
      // yPosShortLines[0] = round(scaleValue(i, minSpeed, maxSpeed, 320, 0));
      yPosShortLines[0] = scaleValue(i, minSpeed, maxSpeed, 320, 0);
      speedValues[0] = i;
      mainSpr.drawWideLine(15, yPosShortLines[0] + 2, 23, yPosShortLines[0] + 2, 3, TFT_WHITE, TFT_BLACK);
      // tft.setTextColor(TFT_GREEN);
      // tft.drawString(String(i), 0, 20, 4);
      break;
    }
  }
  // Now populate the positions of the other short lines

  for (i = 1; i < 8; i++)
  {
    // yPosShortLines[i] = round(scaleValue(speedValues[0] + (i * 10), minSpeed, maxSpeed, 320, 0));
    yPosShortLines[i] = scaleValue(speedValues[0] + (i * 10), minSpeed, maxSpeed, 320, 0);
    speedValues[i] = speedValues[0] + (i * 10);
    mainSpr.drawWideLine(15, yPosShortLines[i] + 2, 23, yPosShortLines[i] + 2, 3, TFT_WHITE, TFT_BLACK);
  }
  // Draw the short lines
}

void MD302_ASI_ALT::drawSpeedIndicatorValues()
{
  speedIndBoxSpr.setTextColor(TFT_WHITE);
  speedIndBoxSpr.setTextDatum(ML_DATUM);
  // speedIndBoxSpr.setFreeFont(FSSB12);
  speedIndBoxSpr.loadFont(digitsL);
  speedIndBoxSpr.drawString(String((int)round(airSpeed)), 13, speed_indicator_box_2_height / 2 - 6);
}

void MD302_ASI_ALT::drawAltitudeIndicator()
{
  drawAltitudeIndicatorLines();
  // altIndBoxSpr.setSwapBytes(true);
  altIndBoxSpr.pushImage(0, 0, altitude_indicator_box_2_width, altitude_indicator_box_2_height, altitude_indicator_box_2);
  altIndBoxSpr.setSwapBytes(true);
  drawAltitudeIndicatorValues();
  altIndBoxSpr.pushToSprite(&mainSpr, 122, 140, TFT_BLACK);

  baroBoxSpr.setSwapBytes(true);
  baroBoxSpr.pushImage(0, 0, baro_box_width, baro_box_height, baro_box);

  baroBoxSpr.setTextColor(TFT_GREEN);
  baroBoxSpr.setTextDatum(MC_DATUM);
  // baroBoxSpr.setFreeFont(FSSB12);
  baroBoxSpr.loadFont(digitsS);
  baroBoxSpr.drawString(String(baro), baro_box_width / 2, baro_box_height / 2 + 10);
  baroBoxSpr.pushToSprite(&mainSpr, 88, 10, TFT_BLACK);

  // tft.setViewport(120, 0, 240, 320);
  // AltitudeIndSpr.pushSprite(0, 0);
  // mainSpr.pushToSprite(&mainSpr, 120, 0, TFT_BLACK);
  // mainSpr.fillSprite(TFT_BLACK);
  baroBoxSpr.fillSprite(TFT_BLACK);
}

void MD302_ASI_ALT::drawAltitudeIndicatorLines()
{

  int yPosShortLines[10];
  int yPosMediumLines[5];
  int yPosLongLines[5];
  int altitudeValues[10];
  int i;
  int tmpMinAlt;
  int tmpMaxAlt;

  int minAltitude; // minimum altitude in range
  int maxAltitude; // maximum altitude in range

  // minAltitude = round(altitude - 250.0);
  minAltitude = altitude - 250.0;
  // maxAltitude = round(altitude + 250.0);
  maxAltitude = altitude + 250.0;

  mainSpr.loadFont(digitsS);
  // mainSpr.setFreeFont(digitsM);
  mainSpr.setTextColor(TFT_WHITE);
  mainSpr.setTextDatum(MR_DATUM);

  // find the first altitude value that has a "100" to draw long lines
  for (i = round(minAltitude); i <= round(maxAltitude); i++)
  {
    if ((i % 100) == 0) // found our first long line
    {
      // yPosLongLines[0] = round(scaleValue(i, minAltitude, maxAltitude, 320, 0));
      yPosLongLines[0] = scaleValue(i, minAltitude, maxAltitude, 320, 0);
      altitudeValues[0] = i;
      mainSpr.drawWideLine(120 + 91, yPosLongLines[0] + 2, 120 + 109, yPosLongLines[0] + 2, 3, TFT_WHITE, TFT_BLACK);
      mainSpr.drawString(String(altitudeValues[0]), 120 + 89, yPosLongLines[0] + 2);
      break;
      // tft.setTextColor(TFT_GREEN);
      // tft.drawString(String(i), 0, 210, 4);
    }
  }

  // Now populate the positions of the other long lines
  for (i = 1; i < 5; i++)
  {
    // yPosLongLines[i] = round(scaleValue(altitudeValues[0] + (i * 100), minAltitude, maxAltitude, 320, 0));
    yPosLongLines[i] = scaleValue(altitudeValues[0] + (i * 100), minAltitude, maxAltitude, 320, 0);
    mainSpr.drawWideLine(120 + 91, yPosLongLines[i] + 2, 120 + 109, yPosLongLines[i] + 2, 3, TFT_WHITE, TFT_BLACK);
    altitudeValues[i] = altitudeValues[0] + (i * 100);
    mainSpr.drawString(String(altitudeValues[i]), 120 + 89, yPosLongLines[i] + 2);
  }

  // find the first altitude value that has a "50" to draw medium lines
  for (i = minAltitude; i <= maxAltitude; i++)
  {
    if ((i % 100) != 0 && (i % 50) == 0) // found our first medium ine
    {
      // yPosMediumLines[0] = round(scaleValue(i, minAltitude, maxAltitude, 320, 0));
      yPosMediumLines[0] = scaleValue(i, minAltitude, maxAltitude, 320, 0);
      altitudeValues[0] = i;
      mainSpr.drawWideLine(120 + 97, yPosMediumLines[0] + 2, 120 + 109, yPosMediumLines[0] + 2, 3, TFT_WHITE, TFT_BLACK);
      // AltitudeIndSpr.drawString(String(altitudeValues[0]), 50, yPosMediumLines[0] + 2);
      break;
    }
  }

  // Now populate the positions of the other medium lines

  for (i = 1; i < 5; i++)
  {
    // yPosMediumLines[i] = round(scaleValue(altitudeValues[0] + (i * 100), minAltitude, maxAltitude, 320, 0));
    yPosMediumLines[i] = scaleValue(altitudeValues[0] + (i * 100), minAltitude, maxAltitude, 320, 0);
    altitudeValues[i] = altitudeValues[0] + (i * 100);
    mainSpr.drawWideLine(120 + 97, yPosMediumLines[i] + 2, 120 + 109, yPosMediumLines[i] + 2, 3, TFT_WHITE, TFT_BLACK);
  }

  // find the first altitude value that has a "25" to draw short lines
  for (i = minAltitude; i <= maxAltitude; i++)
  {
    if ((i % 100) != 0 && (i % 50) != 0 && (i % 25) == 0) // found our first short ine
    {
      // yPosShortLines[0] = round(scaleValue(i, minAltitude, maxAltitude, 320, 0));
      yPosShortLines[0] = scaleValue(i, minAltitude, maxAltitude, 320, 0);
      altitudeValues[0] = i;
      mainSpr.drawWideLine(120 + 102, yPosShortLines[0] + 2, 120 + 109, yPosShortLines[0] + 2, 3, TFT_WHITE, TFT_BLACK);
      break;
    }
  }

  // Now populate the positions of the other short lines

  for (i = 1; i < 10; i++)
  {
    // yPosShortLines[i] = round(scaleValue(altitudeValues[0] + (i * 50), minAltitude, maxAltitude, 320, 0));
    yPosShortLines[i] = scaleValue(altitudeValues[0] + (i * 50), minAltitude, maxAltitude, 320, 0);
    altitudeValues[i] = altitudeValues[0] + (i * 50);
    mainSpr.drawWideLine(120 + 102, yPosShortLines[i] + 2, 120 + 109, yPosShortLines[i] + 2, 3, TFT_WHITE, TFT_BLACK);
  }
}

void MD302_ASI_ALT::drawAltitudeIndicatorValues()
{
  altIndBoxSpr.setTextColor(TFT_WHITE);
  altIndBoxSpr.setTextColor(TFT_WHITE);
  altIndBoxSpr.setTextDatum(MR_DATUM);
  // altIndBoxSpr.setFreeFont(FSSB12);
  altIndBoxSpr.loadFont(digitsL);
  altIndBoxSpr.drawString(String((int)round(altitude)), 88, altitude_indicator_box_2_height / 2 - 9);
}

void MD302_ASI_ALT::drawAll()
{
  tft.setViewport(0, 0, 240, 320);
  mainSpr.pushSprite(0, 0);
  mainSpr.fillSprite(TFT_BLACK);
}

void MD302_ASI_ALT::setAirSpeed(float value)
{
  airSpeed = value;
}

void MD302_ASI_ALT::setAltitude(float value)
{
  altitude = value;
}

void MD302_ASI_ALT::setBaro(float value)
{
  baro = value;
}

void MD302_ASI_ALT::setHeading(float value)
{
  heading = value;
}

void MD302_ASI_ALT::setScreenRotation(int value)
{
  screenRotation = value;
}
void MD302_ASI_ALT::setInstrumentBrightnessRatio(float value)
{
  instrumentBrightnessRatio = value;
  instrumentBrightness = (int)scaleValue(instrumentBrightnessRatio, 0, 1, 0, 255);
}

void MD302_ASI_ALT::setPowerSaveMode(bool enabled)
{
    if(enabled)
    {
        digitalWrite(TFT_BL, LOW);
        powerSaveFlag = true;
    }
    else
    {
        analogWrite(TFT_BL, instrumentBrightness);
        powerSaveFlag = false;
    }
}

// Scale function
float MD302_ASI_ALT::scaleValue(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
