#include <Arduino.h>
#include "globals.h"

// FileSystem headers
#include <FS.h>
#include <LITTLEFS.h>

#include "WorkingStation.h"

CWorkingStation workStation;

CRGB channel1[110] = { CRGB::Black };
CRGB channel2[110] = { CRGB::Black };

void setup()
{
    // Initialize Serial output
    SERIAL_CONFIGURE;

    delay(2000);

    Println("");
    Println("Starting...");

    // bool littleFsEn = LittleFS.begin();
    // Print("LittleFS Status: ");
    // Println(littleFsEn);

    // workStation.Init();

    Println("Setup finished.");

    pinMode(LED_PIN0, OUTPUT);
    FastLED.addLeds<WS2812B, LED_PIN0, COLOR_ORDER>(channel1, 110);

    pinMode(LED_PIN1, OUTPUT);
    FastLED.addLeds<WS2812B, LED_PIN1, COLOR_ORDER>(channel2, 110);

    FastLED.setBrightness(255);
}

void loop()
{
    // workStation.Work();

    for (int i = 0; i < 110; i++)
    {
        channel1[i] = CRGB::BlueViolet;
        channel2[i] = CRGB::Green;

        FastLED.show();
        delay(500);
    }


    for (int i = 0; i < 110; i++)
    {
        channel1[i] = CRGB::Black;
        channel2[i] = CRGB::Black;

        FastLED.show();
        delay(500);
    }
}