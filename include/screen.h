#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SSD1306 Display Objekt erstellen
extern Adafruit_SSD1306 screen;

struct menuEntry{
    char name[8];
    double (*function)(double);
};

void draw_text(int, int, char*);

void draw_menu(menuEntry* EntryList, int length, int curser_position, int list_position);

void draw_A0(int, int, int);

void draw_topBar_fktVis(char* name, uint8_t* logo);
void draw_coordinateSystem(double x1, double x2, double y1, double y2);
void draw_function(double x1, double x2, double y1, double y2, int grid_step, double (*fkt)(double));

#endif //SCREEN_H