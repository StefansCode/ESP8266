#include <Arduino.h>
#include "hardware.h"

// global variables for debouncing the button
#define DEBOUNCETIME 50
unsigned long lastInterruptTime = 0;

// extern Variables for the encoder
volatile bool encoderButtonPressed = false;
volatile int encoderPosition = 0;
int lastPosition = 0;

//extern Variables for the buttons to detect change
volatile bool button_1_lastState = false;
volatile bool button_2_lastState = false;

// Implementation of the constructor
void setupHardware(){

    // setup LED
    pinMode(LED, OUTPUT);

    // setup buttons
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);

    // Pin setup for the Rotary encoder
    pinMode(SW, INPUT_PULLUP);
    pinMode(CLK, INPUT_PULLUP);
    pinMode(DT , INPUT_PULLUP);

    // attach the IRS-functions to the Pins
    attachInterrupt(digitalPinToInterrupt(CLK), handleEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW), handleButton, FALLING);
}


// ISR-function for the button
void IRAM_ATTR handleButton() {
  if (millis() - lastInterruptTime > DEBOUNCETIME) {
        encoderButtonPressed = true;
        lastInterruptTime = millis();
    }
}


// ISR-function for the encoder.
void IRAM_ATTR handleEncoder() {
    static int lastStateCLK = HIGH;
    int currentStateCLK = digitalRead(CLK); // Beispiel: CLK-Pin

    if (currentStateCLK != lastStateCLK && currentStateCLK == LOW) {
        if (digitalRead(DT) != currentStateCLK) { // Beispiel: DT-Pin
            encoderPosition++; // Drehung im Uhrzeigersinn
        } else {
            encoderPosition--; // Drehung gegen den Uhrzeigersinn
        }
    }
    lastStateCLK = currentStateCLK;
}