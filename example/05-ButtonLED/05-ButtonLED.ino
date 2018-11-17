// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Button & LED Example
// Original Button codes you can find here:
// https://www.arduino.cc/en/Tutorial/Button
// https://www.arduino.cc/en/Tutorial/Debounce
// 
// This example demonstrates how to debounce an input, which means checking twice 
// in a short period of time to make sure the pushbutton is definitely pressed. 
// Without debouncing, checking the button once may cause unpredictable results. 
// This sketch uses the millis() function to keep track of the time passed since 
// the button was pressed.

#include <FastLED.h>          // https://github.com/FastLED/FastLED

#define LED_PIN     A3        // Data pin that LEDs data will be written out over
#define NUM_LEDS    17        // All LEDs on shield 
#define BRIGHTNESS  60        // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B   // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB       // For color ordering use this sketch: http://bit.ly/RGBCalibrate   

#define S1  A2                // Buttons connected to the A0, A1, A2 Arduino pins
#define S2  A1    
#define S3  A0 

// Array of LEDs
CRGB leds[NUM_LEDS];

// Each time the input pin (button) goes from LOW to HIGH (e.g. because of a push-button
// press), the output pin is toggled from LOW to HIGH or HIGH to LOW. There's a
// minimum delay between toggles to debounce the circuit (i.e. to ignore noise).

// The current reading from the input pins
bool S1state = LOW;                  
bool S2state = LOW;    
bool S3state = LOW;  

// The previous reading from the input pins
bool lastreadS1 = LOW;                
bool lastreadS2 = LOW;
bool lastreadS3 = LOW;

// The current state of the LED
int led1state = HIGH;       
int led2state = HIGH;
int led3state = HIGH;

// The following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // The last time the output pin was toggled
unsigned long debounceDelay = 50;    // The debounce time; increase if the output flickers


void setup() {
  
    // Important power-up safety delay
    delay(3000);     
  
    // Limit draw to 450mA at 5V of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5,450);   

    // Initialize LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    // Initialize the buttons pins as an input
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    pinMode(S3, INPUT);
}

void loop() 
{
    checkS1(); // Check S1 button and if is pressed then toggle red LED
    checkS2(); // Check S2 button and if is pressed then toggle green LED
    checkS3(); // Check S3 button and if is pressed then toggle blue LED
}


////////////////////////////////////////////////////////////////////////////////////
// Check S1 button and if is pressed then toggle red LED
////////////////////////////////////////////////////////////////////////////////////
void checkS1() 
{
    // Read the state of the push button into a local variable:
    int currentreadS1 = digitalRead(S1);

    // Check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // Check which button changed, due to noise or pressing:
    if (currentreadS1 != lastreadS1) 
    {
        // Reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
        // Whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // If the button state has changed:
        if (currentreadS1 != S1state) 
        {
            S1state = currentreadS1;

            // only toggle the LED if the new button state is HIGH
            if (S1state == HIGH) 
            {
                led1state = !led1state;
            }
        }
    }

    if (!led1state) 
    {
        leds[12] = CRGB::Red;
        FastLED.show();       
    } else 
    {
        leds[12] = CRGB::Black;
        FastLED.show();  
    } 

    // Save S1 button state. Next time through the loop, it'll be the lastreadS1:
    lastreadS1 = currentreadS1;
}

////////////////////////////////////////////////////////////////////////////////////
// Check S2 button and if is pressed then toggle green LED
////////////////////////////////////////////////////////////////////////////////////
void checkS2() 
{

    // Read the state of the push button into a local variable:
    int currentreadS2 = digitalRead(S2);

    // Check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // Check which button changed, due to noise or pressing:
    if (currentreadS2 != lastreadS2) 
    {
        // Reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
        // Whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // If the button state has changed:
        if (currentreadS2 != S2state) 
        {
            S2state = currentreadS2;

            // only toggle the LED if the new button state is HIGH
            if (S2state == HIGH) 
            {
                led2state = !led2state;
            }
        }
    }

    if (!led2state) 
    {
        leds[14] = CRGB::Green;
        FastLED.show();       
    } else 
    {
        leds[14] = CRGB::Black;
        FastLED.show();  
    } 

    // Save S2 button state. Next time through the loop, it'll be the lastreadS2:
    lastreadS2 = currentreadS2; 
}

////////////////////////////////////////////////////////////////////////////////////
// Check S3 button and if is pressed then toggle blue LED
////////////////////////////////////////////////////////////////////////////////////
void checkS3() 
{
    // Read the state of the push button into a local variable:
    int currentreadS3 = digitalRead(S3);

    // Check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // Check which button changed, due to noise or pressing:
    if (currentreadS3 != lastreadS3) 
    {
        // Reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
        // Whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // If the button state has changed:
        if (currentreadS3 != S3state) 
        {
            S3state = currentreadS3;

            // only toggle the LED if the new button state is HIGH
            if (S3state == HIGH) 
            {
                led3state = !led3state;
            }
        }
    }

    if (!led3state) 
    {
        leds[16] = CRGB::Blue;
        FastLED.show();       
    } else 
    {
        leds[16] = CRGB::Black;
        FastLED.show();  
    } 

    // Save S3 button state. Next time through the loop, it'll be the lastreadS3:
    lastreadS3 = currentreadS3; 
}
