// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Piezo Melody "The Imperial March" Example
// Original code you can find here:
// http://www.arduino.cc/en/Tutorial/Tone
// 
// This example shows how to use piezo with the tone() command to generate notes.
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

#include "pitches.h"          // Need to create the pitches.h library: https://arduino.cc/en/Tutorial/ToneMelody

#define PIEZO      11        // The number of the Piezo pin. Datasheet: http://bit.ly/PIEZO-KLJ1230

// Notes in the melody:
const int melody[] PROGMEM = 
{
    NOTE_A4,  NOTE_A4,  NOTE_A4,  NOTE_F4,  NOTE_C5,  NOTE_A4,  NOTE_F4,  NOTE_C5,
    NOTE_A4,  NOTE_E5,  NOTE_E5,  NOTE_E5,  NOTE_F5,  NOTE_C5,  NOTE_GS4, NOTE_F4,
    NOTE_C5,  NOTE_A4,  NOTE_A5,  NOTE_A4,  NOTE_A4,  NOTE_A5,  NOTE_GS5, NOTE_G5,
    NOTE_FS5, NOTE_F5,  NOTE_FS5, 0,        NOTE_AS4, NOTE_DS5, NOTE_D5,  NOTE_CS5,
    NOTE_C5,  NOTE_B4,  NOTE_C5,  0,        NOTE_F4,  NOTE_GS4, NOTE_F4,  NOTE_A4,
    NOTE_C5,  NOTE_A4,  NOTE_C5,  NOTE_E5,  NOTE_A5,  NOTE_A4,  NOTE_A4,  NOTE_A5,
    NOTE_GS5, NOTE_G5,  NOTE_FS5, NOTE_F5,  NOTE_FS5, 0,        NOTE_AS4, NOTE_DS5,
    NOTE_D5,  NOTE_CS5, NOTE_C5,  NOTE_B4,  NOTE_C5,  0,        NOTE_F4,  NOTE_GS4,
    NOTE_F4,  NOTE_C5,  NOTE_A4,  NOTE_F4,  NOTE_C5,  NOTE_A4,                            
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.:
// Some notes durations have been changed (1, 3, 6) to make them sound better
const byte noteDurations[] PROGMEM = 
{
    2, 2, 2, 3, 6, 2, 3, 6, 
    1, 2, 2, 2, 3, 6, 2, 3, 
    6, 1, 2, 3, 6, 2, 4, 4, 
    8, 8, 4, 3, 4, 2, 4, 4,
    8, 8, 4, 3, 6, 2, 3, 6,
    2, 3, 6, 1, 2, 3, 8, 2,
    4, 4, 8, 8, 4, 4, 4, 2,
    4, 4, 8, 8, 4, 4, 4, 2, 
    3, 8, 2, 3, 8, 1,                                       
};

void setup() 
{
    // Nothing to setup this time
}

void loop() 
{
    for (int thisNote = 0; thisNote < 70; thisNote++) 
    {   
        // To calculate the note duration, take one second divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / pgm_read_byte(&noteDurations[thisNote]);
        tone(PIEZO, pgm_read_word(&melody[thisNote]), noteDuration);

        // To distinguish the notes, set a minimum time between them.
        // The note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // Stop the tone playing:
        noTone(PIEZO);
    }
    delay(2000); 
}
