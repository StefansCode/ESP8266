#include <Arduino.h>
#include <cmath>

#include "functionVisualizer.h"
#include "hardware.h"
#include "screen.h"
#include "bitmap.h"

// Intervalbounderys
double x_1=-2;
double x_2= 2;
double y_1= -1;
double y_2= 1;

int grid_size=4;

// update the screen with the updated parameters
void update(void (*scaling)());

// different scalings
void do_nothing();
void zoom_out();
void zoom_in();
void move_up();
void move_down();
void move_right();
void move_left();

// point to the function that should be displayed
double (*function)(double);
char* function_name;

int cursor_position=0;
int list_position=0;
const int list_length = 6;menuEntry EntryList[list_length]= {
                                        {"sin",sin},
                                        {"cos",cos},
                                        {"tan",tan},
                                        {"asin",asin},
                                        {"acos",acos},
                                        {"atan",atan},
                                      };
int funktionSelector(){

    draw_menu(EntryList, list_length, cursor_position, list_position);

    if(!digitalRead(BTN2)){
        function =      EntryList[list_position + cursor_position].function;
        function_name = EntryList[list_position + cursor_position].name;
        update(do_nothing); // this is needed since funktionVisualizer can only update on user input
        return 1;
    }
    else{
        // turn rotary encoder right
        if(encoderPosition > lastPosition) {
            lastPosition = encoderPosition;
            if(list_position + cursor_position < list_length - 1){
                if(cursor_position == 3){
                    list_position++;
                }
                else{
                    cursor_position++;
                }
            }
        }
        // turn rotary encoder left
        else if (encoderPosition < lastPosition) {
            lastPosition = encoderPosition;
            if(list_position + cursor_position > 0){
                if(cursor_position == 0){
                    list_position--;
                }
                else{
                    cursor_position--;
                }
            }
        }
    }
    return 0;
}

#define INOUT 0
#define LEFTRIGHT 1
#define UPDOWN 2
#define GRID 3

int zoom_mode=0;
const uint8_t* logo = logo_inout;
void funktionVisualizer(){
    
    // if button 2 is pressed change the zoom mode
    if(!digitalRead(BTN2) and !digitalRead(BTN2) != button_2_lastState){
        zoom_mode = (((zoom_mode+1) % 4) + 4) % 4;
    if(zoom_mode == INOUT){logo = logo_inout;}
    if(zoom_mode == LEFTRIGHT){logo = logo_leftright;}
    if(zoom_mode == UPDOWN){logo = logo_updown;}
    if(zoom_mode == GRID){logo = logo_grid;}
        draw_topBar_fktVis(function_name, logo);
        screen.display();
    }
    button_2_lastState =  !digitalRead(BTN2);

    // if the rotary encoder is turned update accordingly
    if(zoom_mode == INOUT){
        logo = logo_inout;
        if(encoderPosition < lastPosition) {
            lastPosition = encoderPosition;
            update(zoom_out);
        }
        else if (encoderPosition > lastPosition) {
            lastPosition = encoderPosition;
            update(zoom_in);
        }
    }
    else if(zoom_mode == LEFTRIGHT){
        logo = logo_leftright;
        if (encoderPosition < lastPosition) {
            lastPosition = encoderPosition;
            update(move_right);
        }
        else if (encoderPosition > lastPosition) {
            lastPosition = encoderPosition;
            update(move_left);
        }
    }
    else if(zoom_mode == UPDOWN){
        logo = logo_updown;
        if (encoderPosition < lastPosition) {
            lastPosition = encoderPosition;
            update(move_down);
        }
        else if (encoderPosition > lastPosition) {
            lastPosition = encoderPosition;
            update(move_up);
        }
    }
    else if(zoom_mode == GRID){
        logo = logo_grid;
        if (encoderPosition > lastPosition) {
            lastPosition = encoderPosition;
            if(grid_size > 1){
                grid_size = grid_size/2;
                update(do_nothing);
            }
        }
        else if (encoderPosition < lastPosition) {
            lastPosition = encoderPosition;
            if(grid_size < SCREEN_WIDTH){
                grid_size = grid_size*2;
                update(do_nothing);
            }
        }
    }
    
}

void update(void (*scaling)()){
    scaling();
    screen.clearDisplay();
    draw_topBar_fktVis(function_name, logo);
    draw_coordinateSystem(x_1 ,x_2 ,y_1, y_2);
    draw_function(x_1 ,x_2 ,y_1, y_2, grid_size, function);
    draw_topBar_fktVis(function_name, logo);
    if(zoom_mode == GRID){
        screen.setTextSize(1);
        screen.setCursor(90, 4);
        screen.setTextColor(WHITE);
        screen.println(grid_size);
    }
    screen.display();
}

void do_nothing(){

}

void zoom_out(){
    int oldx_1=x_1;int oldy_1=y_1;
    x_1=x_1-(x_2-x_1)*0.1;
    x_2=x_2+(x_2-oldx_1)*0.1;
    y_1=y_1-(y_2-y_1)*0.1;
    y_2=y_2+(y_2-oldy_1)*0.1;
}
void zoom_in(){
    int oldx_1=x_1;int oldy_1=y_1;
    x_1=x_1+(x_2-x_1)*0.1;
    x_2=x_2-(x_2-oldx_1)*0.1;
    y_1=y_1+(y_2-y_1)*0.1;
    y_2=y_2-(y_2-oldy_1)*0.1;
}
void move_up(){
    int oldy_1=y_1;
    y_1=y_1+(y_2-y_1)*0.1;
    y_2=y_2+(y_2-oldy_1)*0.1;
}
void move_down(){
    int oldy_1=y_1;
    y_1=y_1-(y_2-y_1)*0.1;
    y_2=y_2-(y_2-oldy_1)*0.1;
}
void move_right(){
    int oldx_1=x_1;
    x_1=x_1+(x_2-x_1)*0.1;
    x_2=x_2+(x_2-oldx_1)*0.1;
}
void move_left(){
    int oldx_1=x_1;
    x_1=x_1-(x_2-x_1)*0.1;
    x_2=x_2-(x_2-oldx_1)*0.1;
}