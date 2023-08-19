#pragma once


#define FLASH_VERSION          25   // Update ONLY this to increment the version number

#define XSTR(x) STR(x)              // The defs will generate the stringized version of it
#define STR(x) "v0"#x               // Remove the zero when we exceed 100, or make this dynamic
#define FLASH_VERSION_NAME_X(x) "v"#x 
#define FLASH_VERSION_NAME XSTR(FLASH_VERSION)

#define FASTLED_INTERNAL        1   // Silence FastLED build banners
#define NTP_DELAY_COUNT         20  // delay count for ntp update
#define NTP_PACKET_LENGTH       48  // ntp packet length
#define TIME_ZONE             (-8)  // My offset from London (UTC-8)

// C Helpers and Macros

#define ARRAYSIZE(a)		(sizeof(a)/sizeof(a[0]))		// Returns the number of elements in an array
#define PERIOD_FROM_FREQ(f) (round(1000000 * (1.0 / f)))	// Calculate period in microseconds (us) from frequency in Hz
#define FREQ_FROM_PERIOD(p) (1.0 / p * 1000000)				// Calculate frequency in Hz given the period in microseconds (us)

#define EFFECT_CROSS_FADE_TIME 600.0    // How long for an effect to ramp brightness fader down and back during effect change


#define FASTLED_INTERNAL        1   // Suppresses the compilation banner from FastLED

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <Arduino.h>

#include <iostream>
#include <memory>
#include <string>

#include <FastLED.h>                // FastLED for the LED panels
#include <pixeltypes.h>             // Handy color and hue stuff

#include <ESP8266WiFi.h>

#include <sys/time.h>
#include <exception>

#include<sstream>

// I don't know why to_string is missing, but it seems to be a compiler/cygwin
// issue. If this turns into a redefinition at some point because the real one
// comes online in the future, this to_string can be removed.

template <typename T>
std::string to_string(T value)
{
    //create an output string stream
    std::ostringstream os ;

    //throw the value into the string stream
    os << value ;

      //convert the string stream into a string and return
    return os.str();
}

#define STRING(num) STR(num)

// This is a simple demo configuration.  To build, simply connect the data lead from a WS2812B
// strip to pin 5.  This does not use the OLED, LCD, or anything fancy, it simply drives the
// LEDs with a simple rainbow effect as specified in effects.cpp for DEMO.
//
// Please ensure you supply sufficent power to your strip, as even the DEMO of 144 LEDs, if set
// to white, would overload a USB port.

#define LED_PIN0                14
#define LED_PIN1                12
#define COLOR_ORDER             RGB

#define MATRIX_WIDTH            110
#define MATRIX_HEIGHT           1
#define NUM_LEDS                (MATRIX_WIDTH*MATRIX_HEIGHT)
#define NUM_CHANNELS            2

#define POWER_LIMIT_MW       3 * 12 * 1000   // 3 amp supply at 12 volts assumed

// How long should the error be shown in milliseconds
#define ERROR_SHOW_TIME 30 * 1000

//
// CConfigurationManager Definitions
#define ESP8266_LED 2
#define BUILD_IN_LED_ON LOW
#define BUILD_IN_LED_OFF HIGH

const char WiFiAPPID[] = "ESP8266 ConfigWifi";
const char WiFiAPPSK[] = "123456789";

// CConfigurationFile Definitions
#define CONFIG_FILE_NAME "/config.txt"
#define CONFIG_FILE_SEPAREATOR '&'
#define CONFIG_FILE_EQUALS '='
#define CONFIG_FILE_END '#'

const char stationID[] = "ESP8266_OUTSIDE_LED_CONTROLLER_TWO";
const char reportErrorTopic[] = "outside_led_controller_two/get/error";
const char reportCurrEffectTopic[] = "outside_led_controller_two/get/effect";
const char reportCurrBrightnessTopic[] = "outside_led_controller_two/get/brightness";
const char setEffectTopic[] = "outside_led_controller_two/set/effect";
const char setBrightnessTopic[] = "outside_led_controller_two/set/brightness";
const char subscribeTopic[] = "outside_led_controller_two/set/#";

// !!! WARNING !!!!
// ESP.restart() will fail if done after programing
// (EPS8266 reenters boot loader mode, if not reset via reset button or power circle)
#define WAIT_BEFORE_RESTART_SEC 5 * 60

#define ELAPSED_SECONDS (millis() / 1000)

#define WIFI_WAIT_FOR_CONNECTION unsigned \
long ulRestartTime = ELAPSED_SECONDS + WAIT_BEFORE_RESTART_SEC; \
while (WiFi.status() != WL_CONNECTED) \
{\
    Print(".");\
    if (ulRestartTime < ELAPSED_SECONDS)\
        { Println("Failed to reconnect to WIFI for 5 mins. Calling ESP.restart()"); SERIAL_END; ESP.restart(); } \
    delay(1000); \
}

// Real Global Definitions
#define PRINT_LINES 1

#define SERIAL_SPEED 115200

#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

#if (PRINT_LINES == 1)
#define Print(x) Serial.print(x)
#define Println(x) Serial.println(x); Serial.flush()
#define SERIAL_CONFIGURE Serial.begin(SERIAL_SPEED)
#define SERIAL_END Serial.flush(); \
                   Serial.end(); \
                   delay(20)
#define PIN_MODE_SERIAL_PIN(x, y) ((void)0)
#define DIGITAL_WRITE_SERIAL_PIN(x, y) ((void)0)

#else
#define Print(x) ((void)0)
#define Println(x) ((void)0)
#define SERIAL_CONFIGURE ((void)0)
#define SERIAL_END ((void)0)
#define PIN_MODE_SERIAL_PIN(x, y) pinMode(x, y)
#define DIGITAL_WRITE_SERIAL_PIN(x, y) digitalWrite(x, y)
#endif // DEBUG


#if USE_LCD
    // These pins are based on the Espressif WROVER-KIT, which uses an ILI9314 chipset for its display
    // connected as follows:
    #define TFT_CS   22
    #define TFT_DC   21
    #define TFT_MOSI 23
    #define TFT_SCK  19
    #define TFT_RST  18
    #define TFT_MISO 25
#endif

#ifndef USE_OLED                            
#define USE_OLED 0
#endif

#ifndef USE_TFT                            
#define USE_TFT 0
#endif

#ifndef USE_LCD                            
#define USE_LCD 0
#endif

#ifndef USE_TFTSPI                            
#define USE_TFTSPI 0
#endif

// gRingSizeTable
//
// Items with rings must provide a table indicating how big each ring is.  If an insulator had 60 LEDs grouped
// into rings of 30, 20, and 10, you'd have (NUM_RINGS = 3) and this table would contain (30, 20, 10).

#define MICROS_PER_SECOND   1000000
#define MILLIS_PER_SECOND   1000 

#ifndef M5STICKC
#define M5STICKC 0
#endif

#ifndef M5STICKCPLUS
#define M5STICKCPLUS 0
#endif 

// Microphone
//
// The M5 mic is on Pin34, but when I wire up my own microphone module I usually put it on pin 36.

#if ENABLE_AUDIO
#if TTGO
#define INPUT_PIN (ADC1_CHANNEL_0_GPIO_NUM)   // Pin 27
#elif M5STICKC || M5STICKCPLUS
#define INPUT_PIN (34)	 
#define IO_PIN (0)
#else
#define INPUT_PIN (36)	  // Audio line input, ADC #1, input line 0 (GPIO pin 36)
#endif
#endif


// Final headers
// 
// Headers that are only included when certain features are enabled

#if USE_OLED
#include <U8g2lib.h>                // Library for monochrome displays
#include <gfxfont.h>                // Adafruit GFX font structs
#include <Fonts/FreeSans9pt7b.h>    // A nice font
#include <Adafruit_GFX.h>           // GFX wrapper so we can draw on screen
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_u8g2;
#endif

#if USE_TFTSPI
    #include <TFT_eSPI.h>
    #include <SPI.h>
    extern TFT_eSPI * g_pDisplay;
#endif


// FPS
// 
// Given a time value for when the last frame took place and the current timestamp returns the number of
// frames per second, as low as 0.  Never exceeds 999 so you can make some width assumptions.

inline int FPS(uint32_t start, uint32_t end, uint32_t perSecond = MILLIS_PER_SECOND)
{
	uint32_t duration = end - start;
    if (duration == 0)
    {
        return 999;
    }

	double fpsf = 1.0f / (duration / (double) perSecond);
	int FPS = (int)fpsf;
	if (FPS > 999)
		FPS = 999;
	return FPS;
}


// AppTime
//
// A class that keeps track of the clock, how long the last frame took, calculating FPS, etc.

class AppTime
{
  protected:

    double _lastFrame;
    double _deltaTime;
  
  public:

    // NewFrame
    //
    // Call this at the start of every frame or udpate, and it'll figure out and keep track of how
    // long between frames 

    void NewFrame()
    {
        timeval tv;
        gettimeofday(&tv, nullptr);
        double current = CurrentTime();
        _deltaTime = current - _lastFrame;

        // Cap the delta time at one full second

        if (_deltaTime > 1.0f)
            _deltaTime = 1.0f;

        _lastFrame = current;
    }

    AppTime() : _lastFrame(CurrentTime())
    {
        NewFrame();
    }

    double FrameStartTime() const
    {
        return _lastFrame;
    }

    static double CurrentTime()
    {
        return micros() / 1000000.0;
    }

    static double TimeFromTimeval(const timeval & tv)
    {
        return tv.tv_sec + (tv.tv_usec/(double)MICROS_PER_SECOND);
    }

    static timeval TimevalFromTime(double t)
    {
        timeval tv;
        tv.tv_sec = (long)t;
        tv.tv_usec = t - tv.tv_sec;
        return tv;
    }

    double DeltaTime() const
    {
        return _deltaTime;
    }
};

// C Helpers
//
// Simple inline utility functions like random numbers, mapping, conversion, etc

inline static double randomDouble(double lower, double upper)
{
    double result = (lower + ((upper - lower) * rand()) / RAND_MAX);
    return result;
}

inline double mapDouble(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline uint64_t ULONGFromMemory(uint8_t * payloadData)
{
    return  (uint64_t)payloadData[7] << 56  | 
            (uint64_t)payloadData[6] << 48  | 
            (uint64_t)payloadData[5] << 40  | 
            (uint64_t)payloadData[4] << 32  | 
            (uint64_t)payloadData[3] << 24  | 
            (uint64_t)payloadData[2] << 16  | 
            (uint64_t)payloadData[1] << 8   | 
            (uint64_t)payloadData[0];
}

inline uint32_t DWORDFromMemory(uint8_t * payloadData)
{
    return  (uint32_t)payloadData[3] << 24  | 
            (uint32_t)payloadData[2] << 16  | 
            (uint32_t)payloadData[1] << 8   | 
            (uint32_t)payloadData[0];
}

inline uint16_t WORDFromMemory(uint8_t * payloadData)
{
    return  (uint16_t)payloadData[1] << 8   | 
            (uint16_t)payloadData[0];
}

inline bool CheckBlueBuffer(CRGB * prgb, size_t count)
{
    bool bOK = true;
    for (size_t i = 0; i < count; i++)
    {
        if (bOK)
        {
            if (prgb[i].r > 0 || prgb[i].g > 0)
            {
                Serial.printf("Other color detected at offset %d\n", i);
                bOK = false;
            }
        }
    }
    return bOK;
}
