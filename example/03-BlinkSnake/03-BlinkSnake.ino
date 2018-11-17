// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Blink & Snake Example
// Original code you can find here:
// https://github.com/FastLED/FastLED/tree/master/examples/Blink
// https://github.com/FastLED/FastLED/tree/master/examples/FirstLight
// 
// This example shows how to turn on/off a single LED and move a dot along the strip of LEDs.
//
// Hardware:
// Arduino Uno, Binary Clock Shield for Arduino
// INT/SQW connected to Arduino pin 3 / INT1
// PIEZO connected to Arduino pin 11 PWM
// S3 button connected to Arduino pin A0
// S2 button connected to Arduino pin A1
// S1 button connected to Arduino pin A2
// LEDs      connected to Arduino pin A3
// RTC SDA   connected to Arduino pin A4
// RTC SCL   connected to Arduino pin A5

#include <FastLED.h>            // https://github.com/FastLED/FastLED

#define LED_PIN     A3          // Data pin that LEDs data will be written out over
#define NUM_LEDS    17          // All LEDs on shield 
#define BRIGHTNESS  60          // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B     // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB         // For color ordering use this sketch: http://bit.ly/RGBCalibrate 

CRGB leds[NUM_LEDS];            // Define the array of LEDs

void setup() 
{ 
    // Important power-up safety delay
    delay(3000); 

    // Limit draw to 450mA at 5V of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5,450);
    
    // Initialize LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() 
{
    // Turn on/off single LED
    for(int i = 0; i < 3; i++) 
    {    
        // Turn the LED on, then pause
        leds[0] = CRGB::Red;
        FastLED.show();
        delay(500);
    
        // Now turn the LED off, then pause
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(500);    
    }
  
    // Move a dot along the strip of LEDs
    for(int i = 0; i < NUM_LEDS; i++) 
    {    
        // Turn our current LED on, then show the LEDs
        leds[i] = CRGB::Red;

        // Show the LEDs (only one of which is set to red)
        FastLED.show();

        // Wait a little bit
        delay(100);

        // Turn our current led back to black for the next loop around
        leds[i] = CRGB::Black;     
    }
}
