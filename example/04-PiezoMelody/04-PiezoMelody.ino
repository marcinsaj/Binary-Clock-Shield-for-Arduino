// Binary Clock Shield for Arduino by Marcin Saj https://nixietester.com
// https://github.com/marcinsaj/Binary-Clock-Shield-for-Arduino
//
// Piezo Melody Example
// Original code you can find here:
// http://www.arduino.cc/en/Tutorial/Tone
// 
// This example shows how to use piezo with the tone() command to generate notes.
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

#include "pitches.h"         // Need to create the pitches.h library: https://arduino.cc/en/Tutorial/ToneMelody 

#define PIEZO       11       // The number of the Piezo pin. Datasheet: http://bit.ly/PIEZO-KLJ1230

// Notes in the melody
int melody[] = 
{
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, 
  NOTE_G3, 0,       NOTE_B3, NOTE_C4,
};

// Note durations: 4 = quarter note, 8 = eighth note, etc.:
byte noteDurations[] = 
{
  4, 8, 8, 4,
  4, 4, 4, 4,
};

void setup() 
{
    // Nothing to setup this time
}

void loop() 
{
    for (int thisNote = 0; thisNote < 8; thisNote++) 
    {   
        // To calculate the note duration, take one second divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(PIEZO, melody[thisNote], noteDuration);

        // To distinguish the notes, set a minimum time between them.
        // The note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // Stop the tone playing:
        noTone(PIEZO);
    }
    delay(2000); 
}
