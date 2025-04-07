#include <Arduino.h>

#include "functionVisualizer.h"
#include "hardware.h"
#include "bitmap.h"
#include "screen.h"


void setup() {

  setupHardware();

  // Initialisierung des Displays
    if (!screen.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Stoppen bei Fehler
    }

    screen.clearDisplay();
    // draw Abra
    screen.drawBitmap(0, 0, Abra, 16, 16, WHITE);
    // draw dude
    //draw_dude(0, 20, "O.O");
    //draw_function(x_1 ,x_2 ,y_1, y_2, 8, cos);
    screen.drawBitmap(0, 16, coolguys, 128, 48, WHITE);
    screen.display();

}

int mode=0;
void loop() {
  
  
  //delay(10);
  
  if(mode==0){
    mode = funktionSelector();
  }
  else if(mode==1){
    funktionVisualizer();
  }
  
}