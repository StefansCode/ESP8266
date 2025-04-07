#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include <Arduino.h>

// Global variables for the encoder
extern volatile bool encoderButtonPressed; // Flag for button press
extern volatile int encoderPosition; // Encoder position
extern int lastPosition;

// Global variables for the buttons
extern volatile bool button_1_lastState;
extern volatile bool button_2_lastState;

// Function prototypes
void setupHardware();
void IRAM_ATTR handleButton();
void IRAM_ATTR handleEncoder();

#endif // ROTARYENCODER_H