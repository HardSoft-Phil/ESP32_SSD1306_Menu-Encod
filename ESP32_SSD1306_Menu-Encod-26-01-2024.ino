/*        
================================================================================
    Menu non bloquant sur ESP32, encodeur rotatif et écran I2c OLED SSD1306
================================================================================
 D'après:  https://github.com/alanesq/BasicOLEDMenu
                      et
 https://forum.arduino.cc/t/unresponsiveness-in-nested-menus-esp32-arduino-oled-rotary-encoder/638172

================================================================================
  The sketch displays a menu on the oled and when an item is selected it sets a
  flag and waits until the event is acted upon.  
  Max menu items on a 128x64 oled is five.

================================================================================
*/
#include <Wire.h>
#include <SuperEncodeur.h> // 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Params.h"

//==============================================//
void ledFlash()
{
  // flash onboard led
  if ( (unsigned long)(millis() - ledTimer) > 1000 ) 
  {
    digitalWrite(LED, !digitalRead(LED));
    ledTimer = millis();
  }
}
    
//==============================================//
void setup() 
{
  Serial.begin(115200); while (!Serial); delay(50);
  Serial.println("ESP32_SSD1306_Menu-Encod-PROTO");
  Serial.println("\n\n\nDémarre le menu de démo\n");

  pinMode(LED, OUTPUT);

  // Initialise l'écran OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  { 
    Serial.println(F("SSD1306 non trouvé"));
    for (;;); // Boucle sans fin
  }

  display.clearDisplay();
  display.setTextColor(WHITE);     // Couleur du texte
  display.setRotation(0);          // Orientation de l'écran. Peut etre 0, 1, 2 ou 3
  display.setTextWrap(false);      // Par défaut, les longues lignes sont automatiquement
                                   // coupées et passées à la ligne suivante.

  if (!encodeur.begin())
  {
    Serial.println(F("encodeur non trouvé. Vérifier les branchements."));
    while (1); // On ne va pas plus loin...
  } else {
    Serial.print(F("encodeur Successfully Started at value = "));
    Serial.println(encodeur);
  }

  // Affiche le message de départ - presser le bouton passe au menu principal
  displayMessage("DEPART", "Test Menus");
  while(encodeur.buttonPressed());
}

//==============================================//
void loop() 
{
  menuUpdate();
  ledFlash();
}

//==============================================//
