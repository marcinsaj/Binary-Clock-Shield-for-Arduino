// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Binary Thermometer
// This example demonstrates how to use RTC integrated temperature compensated
// crystal oscillator a simple thermometer. 
// The DS3231 does a temperature conversion once every 64 seconds, temperature is 
// represented with a resolution of 0.25°C. To simplify the temperature display, 
// it is rounded to one decimal place. 
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
#define NUM_LEDS    17        // Only middle and bottom LEDs row 
#define BRIGHTNESS  40        // The best tested LEDs brightness 20-60
#define LED_TYPE    WS2812B   // Datasheet: http://bit.ly/LED-WS2812B
#define COLOR_ORDER GRB       // For color ordering use this sketch: http://bit.ly/RGBCalibrate  

CRGB leds[NUM_LEDS];          // Define the array of LEDs

byte BinaryTemperature[17];   // Binary temperature array 

// The accuracy of the thermometer is +/- 3.00 ° C, so it is better to compare the results 
// with another thermometer and apply the correction value   
float Correction = 3.00;      // +/- 3.00°C
float Celsius;


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
    // Get temperature from RTC
    Celsius = (RTC.temperature() / 4.0) - Correction;
    
    // Show it
    convertToBinaryAndDisplay ();

    // RTC does a temperature conversion once every 64 seconds
    int counter = 0;
    while (counter < 64) 
    {
        float time_now = millis();
        while(millis() < time_now + 1000){}
        counter++;    
    }
}

// Convert temperature to binary format and display on LEDs
void convertToBinaryAndDisplay ()
{  
    // Integer and decimal part extraction
    uint16_t integerPart = Celsius;
    uint16_t decimalPart = (((round(10 * Celsius))*10 - integerPart*100)/10);
    
    Serial << ("Temperature: ");
    Serial << ((integerPart < 10) ? "0" : "");
    Serial << integerPart << "." << decimalPart << "°C";
     
/*  Celsius = 32 bits, integerPart = 16 bits  
    integerPart will only fit 16 bits (without decimals) of Celsius 32 bits 
    Celsius 23.75, integerPart = 23  
      
    e.g. Celsius = 23.75    
    10 * Celsius = 237.5
    round (237.5) = 238
    238 * 10 = 2380
    23 (integer part) * 100 = 2300
    2380 - 2300 = 80
    80/10 = 8 = decimal part  */  

    bool BitTemperature;
   
    for(int i = 12; i < 17; i++)                              // Upper row not used
    {
        leds[i] = CRGB::Black;                                // All LEDs turn off
    }
    
    for(int i = 6; i < 12; i++)                               // Middle row - integer part
    {
        BitTemperature = integerPart & B00000001;             // Extraction of individual bits 0/1
        BinaryTemperature[i] = BitTemperature;                // Save bit in binary array
        integerPart = integerPart >> 1;                       // Bit shift

        if((BinaryTemperature[i])) leds[i] = CRGB::Red;       // Show on LEDs
        else leds[i] = CRGB::Black;       
    }

    for(int i = 0; i < 6; i++)                                // Bottom row - decimal part
    {
        BitTemperature = decimalPart & B00000001;             // Extraction of individual bits 0/1
        BinaryTemperature[i] = BitTemperature;                // Save bit in binary array
        decimalPart = decimalPart >> 1;                       // Bit shift

        if((BinaryTemperature[i])) leds[i] = CRGB::Green;     // Show on LEDs
        else leds[i] = CRGB::Black;       
    }
    
    FastLED.show();

    Serial << ("  BIN: "); 
    for(int i = 11; i >= 0; i--) 
    {                 
        if(i == 5) Serial << (" ");       
        Serial << (BinaryTemperature[i]); 
    }
    Serial << endl;
}
