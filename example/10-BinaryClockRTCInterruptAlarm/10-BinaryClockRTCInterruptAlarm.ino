// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Binary Clock RTC with Interrupt and Alarm Example
// This example demonstrates how to use RTC Alarm option with piezo melody.
// 
// Use this sketch to set/read RTC time. Set alarm and play alarm melody every minute.
// How to use piezo with the tone() command to generate notes you can find here:
// http://www.arduino.cc/en/Tutorial/Tone
// PIEZO Datasheet: http://bit.ly/PIEZO-KLJ1230
//
// A falling edge at the RTC INT/SQW output causes an interrupt, 
// which is uses for regular - 1 per second - reading time from RTC and 
// checking alarm status flag 'A2F'. Since we use RTC SQW output for
// regular reading current time, the alarm interrupt flag 'A2IE' should be clear/disable.
// Check RTC datasheet page 11-13 http://bit.ly/DS3231-RTC
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

#include <FastLED.h>          // https://github.com/FastLED/FastLED
#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/
#include "pitches.h"          // Need to create the pitches.h library: https://arduino.cc/en/Tutorial/ToneMelody

#define INT         3         // Interrupt. Arduino pin no.3 <-> Shield RTC INT/SQW pin           
#define PIEZO       11        // The number of the Piezo pin.
#define LED_PIN     A3        // Data pin that LEDs data will be written out over
#define NUM_LEDS    17        // All LEDs on shield 
#define BRIGHTNESS  30        // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B   // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB       // For color ordering use this sketch: http://bit.ly/RGBCalibrate 

CRGB leds[NUM_LEDS];          // Define the array of LEDs

time_t t; // t variable for Arduino Time Library
// See the Arduino Time Library for details on the tmElements_t structure:    
// http://playground.arduino.cc/Code/Time
// https://github.com/PaulStoffregen/Time     

// Binary time array
byte BinaryTime[17];
byte hours = 0, minutes = 0, seconds = 0; 

// Interrupt flag true/false
volatile bool RTCinterruptWasCalled = false;

// Notes in the melody:
int melodyAlarm[] = 
{
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, 
  NOTE_G3, 0,       NOTE_B3, NOTE_C4
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = 
{
  4, 8, 8, 4,
  4, 4, 4, 4
};


void setup()
{
    Serial.begin(115200);
  
    // Important power-up safety delay
    delay( 3000 );        
  
    // Initialize LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    
    // Set RTC time using tmElements_t structure
    tmElements_t tm;
    tm.Hour = 11;             // Time 11:45:52
    tm.Minute = 45;
    tm.Second = 52;
    RTC.write(tm);            // Set the RTC from the tm structure  

    // Configure an interrupt on the falling edge from the RTC INT/SQW output
    pinMode(INT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT), RTCinterrupt, FALLING);

    // Initialize the alarm every minute - ALM2_EVERY_MINUTE
    RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0);
  
    // Clear the alarm status flag 'A2F'
    RTC.alarm(ALARM_2);

    // Since we use SQW_PIN for regular reading current time, 
    // the alarm interrupt flag 'A2IE' should be clear/disable
    RTC.alarmInterrupt(ALARM_2, false);
  
    // Enable 1 Hz square wave SQW output
    RTC.squareWave(SQWAVE_1_HZ);
}

void RTCinterrupt()
{
    // Set interrupt flag
    RTCinterruptWasCalled = true;
}

void loop()
{
    if (RTCinterruptWasCalled) 
    {
        // Clear interrupt flag
        RTCinterruptWasCalled = false;

        // Get time from RTC and convert to BIN format
        getTime();
        
        // Show on the serial monitor
        printTime();

        // Clear the alarm status flag 'A2F'and clear if the alarm has been activated
        if (RTC.alarm(ALARM_2)) 
        {
            Serial << " *** ALARM!\n";
            playAlarm();
        }             
    }        
}

// Play melody alarm
void playAlarm () 
{
  for (int thisNote = 0; thisNote < 8; thisNote++) {    
    // To calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(PIEZO, melodyAlarm[thisNote], noteDuration);
    
    // To distinguish the notes, set a minimum time between them.
    // The note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    // Stop the tone playing:
    noTone(PIEZO);
  }  
}

// Get time from RTC and convert to BIN format
void getTime() 
{
    // Read time from RTC
    t = RTC.get();
    
    // Convert time to BIN format
    DecToBinary(second(t), minute(t), hour(t));   
}

// Print Time in DEC & BIN format
void printTime() 
{
    Serial << ("DEC:");
    Serial << ((hour(t)<10) ? "0" : "") << _DEC(hour(t)) << ':';
    Serial << ((minute(t)<10) ? "0" : "") << _DEC(minute(t)) << ':';
    Serial << ((second(t)<10) ? "0" : "") << _DEC(second(t)) << "  ";

    Serial << ("BIN:"); 
    for(int i=16; i >= 0; i--) 
    {                 
        if(i == 11 || i == 5) Serial << (" ");       
        Serial << (BinaryTime[i]); 
    }
    Serial << endl;
}

// Convert time from DEC to BIN format and display on LEDs
void DecToBinary(int secTemp, int minTemp, int hourTemp) 
{  
    bool BitTime = 0;
    
    for(int i = 12; i < 17; i++)                        // Hours
    {
        BitTime = hourTemp & B00000001;                 // Extraction of individual bits 0/1
        BinaryTime[i] = BitTime;                        // Save bit in Binary time array
        hourTemp = hourTemp >> 1;                       // Bit shift

        if((BinaryTime[i])) leds[i] = CRGB::Blue;       // Show on LEDs
        else leds[i] = CRGB::Black;     
    }

    for(int i = 6; i < 12; i++)                         // Minutes
    {
        BitTime = minTemp & B00000001;
        BinaryTime[i] = BitTime;
        minTemp = minTemp >> 1;

        if((BinaryTime[i])) leds[i] = CRGB::Green;      // Show on LEDs
        else leds[i] = CRGB::Black;       
    }

    for(int i = 0; i < 6; i++)                          // Seconds
    {
        BitTime = secTemp & B00000001;            
        BinaryTime[i] = BitTime;                  
        secTemp = secTemp >> 1;                     
    
        if((BinaryTime[i])) leds[i] = CRGB::Red;        // Show on LEDs
        else leds[i] = CRGB::Black; 
    }  
    FastLED.show();
}
