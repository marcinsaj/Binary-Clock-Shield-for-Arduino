// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// RGB Calibration Example
//
// Use this sketch to determine what the RGB ordering for your chipset should be.
// You should see six LEDs on. If the RGB ordering is correct, you should see 1 red LED, 2 green 
// LEDs, and 3 blue LEDs.  If you see different colors, the count of each color tells you what 
// the position for that color in the rgb ordering should be.  So, for example, if you see
// 1 Blue, 2 Red, and 3 Green LEDs then the rgb ordering should be BRG (Blue, Red, Green).  
//
// You can then test this ordering by setting the RGB ordering in the COLOR_ORDER line below
// to the new ordering and it should come out correctly, 1 red, 2 green, and 3 blue.
//
// Original code you can find here: https://github.com/FastLED/FastLED/tree/master/examples/RGBCalibrate

#include <FastLED.h>      // https://github.com/FastLED/FastLED

#define NUM_LEDS    6     // LEDs using for RGB Calibration
#define BRIGHTNESS  40    // The best tested LEDs brightness 20-60
#define DATA_PIN    A3    // Data pin that LEDs data will be written out over
#define COLOR_ORDER GRB   // RGB, GRB, GBR etc.

CRGB leds[NUM_LEDS];      // Define the array of LEDs

void setup() 
{  
    // Important power-up safety delay
    delay(3000); 

    // Limit my draw to 450mA at 5V of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5,450);
    
    // LEDs configurations
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

    // Brightness settings
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() 
{
    leds[0] = CRGB(255,0,0); // RGB Red Color
    leds[1] = CRGB(0,255,0); // RGB Green Color
    leds[2] = CRGB(0,255,0); // RGB Green Color
    leds[3] = CRGB(0,0,255); // RGB Blue Color
    leds[4] = CRGB(0,0,255); // RGB Blue Color
    leds[5] = CRGB(0,0,255); // RGB Blue Color
    FastLED.show();
    delay(1000);
}
