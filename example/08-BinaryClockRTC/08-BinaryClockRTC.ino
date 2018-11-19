// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Binary Clock RTC Example
// 
// This example demonstrates how to set the RTC time, read the time from RTC,
// then convert to binary format and show on LEDs.
// DS3231 RTC Datasheet: https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
//
// Hardware:
// Arduino Uno, Binary Clock Shield for Arduino
// Battery CR1216/CR1220 
// INT/SQW   connected to Arduino pin 3  INT1
// PIEZO     connected to Arduino pin 11 PWM
// S3 button connected to Arduino pin A0
// S2 button connected to Arduino pin A1
// S1 button connected to Arduino pin A2
// LEDs      connected to Arduino pin A3
// RTC SDA   connected to Arduino pin A4
// RTC SCL   connected to Arduino pin A5

#include <FastLED.h>          // https://github.com/FastLED/FastLED
#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/                            
          
#define LED_PIN     A3        // Data pin that LEDs data will be written out over
#define NUM_LEDS    17        // All LEDs on shield 
#define BRIGHTNESS  40        // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B   // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB       // For color ordering use this sketch: http://bit.ly/RGBCalibrate   

CRGB leds[NUM_LEDS];          // Define the array of LEDs

byte BinaryTime[17];          // Binary time array

time_t t; // t variable for Arduino Time Library
// See the Arduino Time Library for details on the tmElements_t structure:    
// http://playground.arduino.cc/Code/Time
// https://github.com/PaulStoffregen/Time                   

void setup() 
{  
    Serial.begin(115200);
    
    // Important power-up safety delay
    delay(3000);     
  
    // Limit draw to 450mA at 5V of power draw
    FastLED.setMaxPowerInVoltsAndMilliamps(5,450);   

    // Initialize LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    // Set RTC time using tmElements_t structure. 
    tmElements_t tm;
    tm.Hour = 11;               // Time 11:45:22
    tm.Minute = 45;
    tm.Second = 22;
    RTC.write(tm);              // Set the RTC from the tm structure  
}

void loop()
{
    static time_t tLast;
    t = RTC.get();              // Read time from RTC
    
    if (t != tLast)             // Check if the time has changed
    {
        tLast = t;              // Save current time
        
        // Convert time to BIN format
        DecToBinary(second(t), minute(t), hour(t)); 
        
        // Show on the serial monitor
        printTime();      
    }
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
