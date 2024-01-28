// ----------------------------------------------------------------
// Divers
const int serialDebug = 1;
const int LED = 2;  // onboard led gpio pin
static uint32_t ledTimer = millis();

// ----------------------------------------------------------------
// Encodeur
//Arguments are: (PinA, PinB, Lower Limit, Upper Limit, Initial Value, Full Pulse/HalfPulse)
//Use FULL_PULSE for encodeurs that produce one complete quadrature pulse per detent
//Use HALF_PULSE for endoders that produce one complete quadrature pulse every two detents

#define encodeur_PinA  4            // Rotary encodeur gpio pin
#define encodeur_PinB  5            // Rotary encodeur gpio pin
#define encodeur_Press 15           // Rotary encodeur button gpio pin

SuperEncodeur encodeur(encodeur_PinB, encodeur_PinA, encodeur_Press, 0, 40, 0, HALF_PULSE);

// ----------------------------------------------------------------
// Ecran OLED SSD1306
#define OLED_ADDR 0x3C            // SSD1306 i2c address
#define OLEDC 22                  // OLED SCL pin gpio pin - 22
#define OLEDD 21                  // OLED SDA pin gpio pin - 21
#define SCREEN_WIDTH 128          // OLED display width, in pixels (usually 128)
#define SCREEN_HEIGHT 64          // OLED display height, in pixels (64 for larger oLEDs)

Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

//const int topLine = 10;                   // Ligne de départ d'écriture des items si écran Noir et Blanc
const int topLine = 18;                   // Ligne de départ d'écriture des items si écran bicolore
const byte lineSpace1 = 9;                // line spacing for textsize 1 (small text)
const byte lineSpace2 = 17;               // line spacing for textsize 2 (large text)
const int displayMaxLines = 5;            // max lines that can be displayed in lower section of display in textsize1 (5 on larger oLeds)

// ----------------------------------------------------------------
// Menus
const int menuTimeout = 10;               // menu inactivity timeout (seconds)
const bool menuLargeText = 0;             // show larger text when possible (if struggling to read the small text)
const int maxmenuItems = 12;              // max number of items used in any of the menus (keep as low as possible to save memory)
const int MaxmenuTitleLength = 10;        // max characters per line when using text size 2 (usually 10)

// modes that the menu system can be in
enum menuModes {
  off,                                  // display is off
  menu,                                 // a menu is active
  value,                                // 'enter a value' none blocking is active
  message,                              // displaying a message
  blocking                              // a blocking procedure is in progress (see enter value)
};
menuModes menuMode = off;                 // default mode at startup is off

struct oledMenus {
  // menu
  String menuTitle = "";                    // the title of active mode
  int noOfmenuItems = 0;                    // number if menu items in the active menu
  int selectedMenuItem = 0;                 // when a menu item is selected it is flagged here until actioned and cleared
  int highlightedMenuItem = 0;              // which item is curently highlighted in the menu
  String menuItems[maxmenuItems + 1];       // store for the menu item titles
  uint32_t lastMenuActivity = 0;            // time the menu last saw any activity (used for timeout)
  // 'enter a value'
  int mValueEntered = 0;                    // store for number entered by value entry menu
  int mValueLow = 0;                        // lowest allowed value
  int mValueHigh = 0;                       // highest allowed value
  int mValueStep = 0;                       // step size when encodeur is turned
};
oledMenus oledMenu;
// ----------------------------------------------------------------
