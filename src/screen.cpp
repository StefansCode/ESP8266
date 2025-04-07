#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PID_v1.h>

#include "bitmap.h"
#include "screen.h"

// SSD1306 Display Objekt erstellen
Adafruit_SSD1306 screen(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void draw_text(int x, int y, char* text){

    screen.setTextSize(1);
    screen.setCursor(x, y);
    screen.setTextColor(WHITE);
    screen.println(text);
}

int scaled_hight[] = {20, 31, 42, 53};
void draw_menu(menuEntry* EntryList, int list_length, int cursor_position, int list_position){
    // cursor_position is where the curser is on the screen (1. Item or 2. Item or...)
    // list_position ist the 1.Item on the screen. (only 4 can be shown)

    screen.clearDisplay();

    // clear the top Bar
    screen.fillRect(0,0, 128, 16, BLACK);
    // draw Abra
    screen.drawBitmap(0, 0, Abra, 16, 16, WHITE);

    screen.setTextSize(1);
    screen.setTextColor(WHITE);

    // draw the Cursor
    screen.setCursor(0, scaled_hight[cursor_position]);
    screen.println("->");

    // draw the Entrys
    for(int i=0; i<4; i++){
        if(list_position + i < list_length){
            screen.setCursor(15, scaled_hight[i]);
            screen.println(EntryList[list_position+i].name);
        }
    }

    screen.display();
}

uint16_t A0_history[120];
int current_position = 0;
void draw_A0(int min, int max, int delay){

    // read A0
    A0_history[current_position] = analogRead(A0);


    screen.clearDisplay();

    // write value
    screen.setTextSize(1);
    screen.setCursor(0, 4);
    screen.setTextColor(WHITE);
    screen.print("[");
    screen.print(min);
    screen.print(",");
    screen.print(max);
    screen.print("] ");
    screen.print(A0_history[current_position]);

    // draw the Bar on the right side of the screen
    int scaled_value = map(A0_history[current_position] , min, max, 63, 16);
    screen.fillRect(120,scaled_value, 8,63-scaled_value, WHITE);

    // draw the value over time
    for(int i=1; i<120; i++){
        screen.drawPixel(120-i, A0_history[(((current_position-i)%120)+120)%120], WHITE);
        // positive_modulo (a % b) = ((a % b) + b) % b;
    }

    screen.display();

    // update array
    A0_history[current_position] = scaled_value;
    current_position = (current_position+1)%120;
}

void draw_function(double x1, double x2, double y1, double y2, int grid_step, double (*fkt)(double)){
    double x_step =(x2-x1)/(SCREEN_WIDTH/grid_step);
    double x = x1;
    double y;

    if(grid_step==1){
        for(int i=0; i<SCREEN_WIDTH; i++){
            y = fkt(x);
            x = x+x_step;
            if(y<y1 || y>y2){
                continue;
            }
            y = 16 + ((y - y1) / (y2 - y1)) * (63 - 16);
            screen.drawPixel(i, y, WHITE);
            screen.display();
        }
    }
    else{
        double last_y = fkt(x);
        int scaled_last_y = (int) 16 + ((last_y - y1) / (y2 - y1)) * (63 - 16);

        for(int i=1; i<SCREEN_WIDTH+grid_step; i=i+grid_step){
            x = x+x_step;
            y = fkt(x);
            if((y<y1 || y>y2) && (last_y<y1 ||last_y>y2)){
                last_y=y;
                continue;
            }
            if(isnan(y) || isnan(last_y)){
                last_y=y;
                continue;
            }
            int scaled_y = (int) 16 + ((y - y1) / (y2 - y1)) * (63 - 16);
            screen.drawLine(i-grid_step,scaled_last_y, i, scaled_y, WHITE);
            screen.display();
            last_y=y;
            scaled_last_y= scaled_y;
        }
    }

}

void draw_coordinateSystem(double x1, double x2, double y1, double y2){
    int scaled_zero;
    if(x1<0 && 0<x2){
        scaled_zero = (-x1) * (SCREEN_WIDTH) / (x2 - x1);
        screen.drawLine(scaled_zero, 17, scaled_zero, SCREEN_HEIGHT, WHITE);
    }
    if(y1<0 && 0<y2){
        scaled_zero = (-y1) * (64 - 17) / (y2 - y1) + 17;
        screen.drawLine(0, scaled_zero, SCREEN_WIDTH, scaled_zero, WHITE);
    }
}

void draw_topBar_fktVis(char* name, uint8_t* logo){
    // clear the top Bar
    screen.fillRect(0,0, 128, 16, BLACK);
    // draw Abra
    screen.drawBitmap(0, 0, Abra, 16, 16, WHITE);
    // write name of the function
    draw_text(20,4,name);
    // draw zoom mode logo
    screen.drawBitmap(0, 48, logo, 16, 16, WHITE);
}



