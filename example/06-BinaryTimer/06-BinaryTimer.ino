// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Binary Timer Example
// 
// This example demonstrates simple timer. The time is converted from decimal to binary format.  
// LEDs are updated every 1 second.
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
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/

#define LED_PIN     A3        // Data pin that LEDs data will be written out over
#define NUM_LEDS    17        // All LEDs on shield 
#define BRIGHTNESS  60        // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B   // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB       // For color ordering use this sketch: http://bit.ly/RGBCalibrate  

CRGB leds[NUM_LEDS];          // Define the array of LEDs

byte BinaryTime[17];          // Binary time array 
byte hours = 0, minutes = 0, seconds = 0; 

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
}

void loop() 
{
    // Convert time from DEC to BIN format and show on LEDs
    DecToBinary(seconds, minutes, hours);

    // Show on the serial monitor
    printTime();

    // Change the delay value to see faster counting e.g. 100-10-1
    delay(1000); 
                                             
    seconds++;
  
    // Simple time format H:M:S management 
    if (seconds > 59)
    {        
        seconds = 0;
        minutes++;
        
        if (minutes > 59) 
        {
            minutes = 0;
            hours++;
            
            if (hours > 12) 
            {
                hours = 1;
            }
        }
    }
}

// Print Time in DEC & BIN format
void printTime() 
{
    Serial << ("DEC:");
    Serial << ((hours<10) ? "0" : "") << hours << ':';
    Serial << ((minutes<10) ? "0" : "")  << minutes << ':';
    Serial << ((seconds<10) ? "0" : "")  << seconds << "  ";

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
