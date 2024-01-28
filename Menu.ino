//==============================================//
// Start the default menu
// Menu de démonstration
void defaultMenu()
//
{
  resetMenu();                          // clear any previous menu
  menuMode = menu;                      // enable menu mode
  oledMenu.noOfmenuItems = 8;           // set the number of items in this menu
  oledMenu.menuTitle = "demo_menu";     // menus title (used to identify it)
  oledMenu.menuItems[1] = "item1";      // set the menu items
  oledMenu.menuItems[2] = "item2";
  oledMenu.menuItems[3] = "Quick menu";
  oledMenu.menuItems[4] = "On ou Off";
  oledMenu.menuItems[5] = "Entrer valeur";
  oledMenu.menuItems[6] = "Entrer valeur-bloquee";
  oledMenu.menuItems[7] = "Message";
  oledMenu.menuItems[8] = "Menus Off";
}  // demoMenu
/*
  {
  resetMenu(); // clear any previous menu
  String tList[] = {"main menu",
                    "item1",
                    "item2",
                    "Quick menu",
                    "On ou Off",
                    "Entrer valeur",
                    "Entrer valeur-bloquee",
                    "Message",
                    "Ecran Off"
                   };
  createList("demo_menu", 8, &tList[0]);
  }
*/
//==============================================//
// actions for menu selections are put in here
void menuActions()
{
  // actions when an item is selected in "demo_menu"
  if (oledMenu.menuTitle == "demo_menu")
  {
    // demonstrate quickly create a menu from a list
    if (oledMenu.selectedMenuItem == 3) {
      if (serialDebug) Serial.println("demo_menu : créer un menu à partir d'une liste");
      String tList[] = {"main menu", "2", "3", "4", "5", "6"};
      createList("demo_list", 6, &tList[0]);
    }

    // demonstrate selecting between 2 options only
    if (oledMenu.selectedMenuItem == 4)
    {
      resetMenu();
      menuMode = value;
      // set parameters
      oledMenu.menuTitle = "on or off";
      oledMenu.mValueLow = 0; oledMenu.mValueHigh = 1;
      oledMenu.mValueStep = 1; oledMenu.mValueEntered = 0;
    }

    // demonstrate usage of 'enter a value' (none blocking)
    if (oledMenu.selectedMenuItem == 5)
    {
      if (serialDebug) Serial.println("menu démo : saisie d'une valeur non bloquante");
      resetMenu();
      value1();       // enter a value
    }

    // demonstrate usage of 'enter a value' (blocking) which is quick
    // and easy but stops all other tasks until the value is entered
    if (oledMenu.selectedMenuItem == 6)
    {
      if (serialDebug) Serial.println("demo_menu: blocking enter a value");
      // set perameters
      resetMenu();
      menuMode = value;
      oledMenu.menuTitle = "blocking";
      oledMenu.mValueLow = 0;
      oledMenu.mValueHigh = 50;
      oledMenu.mValueStep = 1;
      oledMenu.mValueEntered = 5;
      int tEntered = serviceValue(1);      // request value
      Serial.println("Valeur = " + String(tEntered));
      defaultMenu();
    }

    // demonstrate usage of message
    if (oledMenu.selectedMenuItem == 7)
    { // 21 chars per line, "\n" = next line
      if (serialDebug) Serial.println("demo_menu: message");
      displayMessage("Message", "Hello\nThis is a demo\nmessage.");
    }

    // turn menu/oLED off
    else if (oledMenu.selectedMenuItem == 8)
    {
      if (serialDebug) Serial.println("demo_menu: menu off");
      resetMenu();  // turn menus off
    }
    oledMenu.selectedMenuItem = 0; // clear menu item selected flag
  }

  // Actions quand un item est selectionné dans le menu
  if (oledMenu.menuTitle == "demo_list")
  {
    // back to main menu
    if (oledMenu.selectedMenuItem == 1)
    {
      if (serialDebug) Serial.println("demo_list: back to main menu");
      defaultMenu();
    }
    oledMenu.selectedMenuItem = 0; // clear menu item selected flag
  }
}  // menuActions

//==============================================//
// Entrer une valeur
void value1()
{
  resetMenu();                           // clear any previous menu
  menuMode = value;                      // enable value entry
  oledMenu.menuTitle = "demo_value";     // title (used to identify which number was entered)
  oledMenu.mValueLow = 0;                // minimum value allowed
  oledMenu.mValueHigh = 100;             // maximum value allowed
  oledMenu.mValueStep = 1;               // step size
  oledMenu.mValueEntered = 50;           // starting value
}

//==============================================//
// actions for value entered put in here
void menuValues()
{
  // action for "demo_value"
  if (oledMenu.menuTitle == "demo_value") {
    String tString = String(oledMenu.mValueEntered);
    if (serialDebug) Serial.println("Valeur entree = " + tString);
    displayMessage("ENTREE", "\nValeur entree = " + tString);
    //    display.print("ENTREE = "); display.print(tString);
    // alternatively use 'resetMenu()' here to turn menus off after
    //value entered - or use 'defaultMenu()' to re-start the default menu
  }
  // action for "on or off"
  if (oledMenu.menuTitle == "on or off") {
    if (serialDebug) Serial.println("demo_menu: on off selection was " + String(oledMenu.mValueEntered));
    defaultMenu();
  }
}

//==============================================//
void menuUpdate()
{
  if (menuMode == off)
    if (encodeur.buttonPressed()) defaultMenu();
    else return;    // if menu system is turned off do nothing more

  // if no recent activity then turn oled off
  if ( (unsigned long)(millis() - oledMenu.lastMenuActivity) > (menuTimeout * 1000) )
  {
    resetMenu();
    return;
  }

  switch (menuMode)
  {
    // Si c'est un menu actif
    case menu:
      serviceMenu();
      menuActions();
      break;
    // if there is an active none blocking 'enter value'
    case value:
      serviceValue(0);
      if (encodeur.buttonPressed())
      { // if the button has been pressed
        menuValues(); // a value has been entered so action it
        break;
      }
    // Si un message doit etre affiché
    case message:
      if (encodeur.buttonPressed()) defaultMenu();// if button has been pressed return to default menu
      break;
  }
}

//==============================================//
void serviceMenu()
{
  // rotary encodeur
  if (encodeur.downClick())
  {
    oledMenu.highlightedMenuItem++;
    oledMenu.lastMenuActivity = millis();   // log time
  }
  if (encodeur.upClick())
  {
    oledMenu.highlightedMenuItem--;
    oledMenu.lastMenuActivity = millis();   // log time
  }
  if (encodeur.buttonPressed())
  {
    oledMenu.selectedMenuItem = oledMenu.highlightedMenuItem; // flag that the item has been selected
    oledMenu.lastMenuActivity = millis();   // log time
    if (serialDebug) Serial.println("menu '" + oledMenu.menuTitle + "' item '" + oledMenu.menuItems[oledMenu.highlightedMenuItem] + "' selected");
  }
  const int _centreLine = displayMaxLines / 2 + 1;    // mid list point
  display.clearDisplay();
  display.setTextColor(WHITE);

  // verify valid highlighted item
  if (oledMenu.highlightedMenuItem > oledMenu.noOfmenuItems) oledMenu.highlightedMenuItem = oledMenu.noOfmenuItems;
  if (oledMenu.highlightedMenuItem < 1) oledMenu.highlightedMenuItem = 1;

  // title
  display.setCursor(0, 0);
  if (menuLargeText) {
    display.setTextSize(2);
    display.println(oledMenu.menuItems[oledMenu.highlightedMenuItem].substring(0, MaxmenuTitleLength));
  } else {
    display.setTextSize(2);
    display.println(oledMenu.menuTitle);
  }
  display.drawLine(0, topLine - 1, display.width(), topLine - 1, WHITE);   // draw horizontal line under title

  // menu
  display.setTextSize(1);
  display.setCursor(0, topLine);
  for (int i = 1; i <= displayMaxLines; i++)
  {
    int item = oledMenu.highlightedMenuItem - _centreLine + i;
    if (item == oledMenu.highlightedMenuItem) display.setTextColor(BLACK, WHITE);
    else display.setTextColor(WHITE);
    if (item > 0 && item <= oledMenu.noOfmenuItems) display.println(oledMenu.menuItems[item]);
    else display.println(" ");
  }
  display.display();
}

//==============================================//
// if _blocking set to 1 then all other tasks are stopped until a value is entered
int serviceValue(bool _blocking)
{
  const int _valueSpacingX = 30;      // spacing for the displayed value y position
  const int _valueSpacingY = 5;       // spacing for the displayed value y position

  if (_blocking)
  {
    menuMode = blocking;
    oledMenu.lastMenuActivity = millis();   // log time of last activity (for timeout)
  }
  uint32_t tTime;
  do {
    // rotary encodeur
    if (encodeur.downClick())
    {
      oledMenu.mValueEntered++;
      oledMenu.lastMenuActivity = millis();   // log time
    }
    if (encodeur.upClick())
    {
      oledMenu.mValueEntered--;
      oledMenu.lastMenuActivity = millis();   // log time
    }
    if (oledMenu.mValueEntered < oledMenu.mValueLow)
    {
      oledMenu.mValueEntered = oledMenu.mValueLow;
      oledMenu.lastMenuActivity = millis();   // log time
    }
    if (oledMenu.mValueEntered > oledMenu.mValueHigh)
    {
      oledMenu.mValueEntered = oledMenu.mValueHigh;
      oledMenu.lastMenuActivity = millis();   // log time
    }

    display.clearDisplay();
    display.setTextColor(WHITE);

    // Titre
    display.setCursor(0, 0);
    if (oledMenu.menuTitle.length() > MaxmenuTitleLength) display.setTextSize(1);
    else display.setTextSize(2);
    display.println(oledMenu.menuTitle);
    display.drawLine(0, topLine - 1, display.width(), topLine - 1, WHITE);   // draw horizontal line under title

    // bar
    display.setTextSize(3);
    const int textPos = 27; // height of number on display
    display.fillRect(0, textPos, SCREEN_WIDTH, SCREEN_HEIGHT - textPos, BLACK);   // clear bottom half of display (128x64)
    display.setCursor(45, textPos);
    display.println(oledMenu.mValueEntered);
    // bar graph at bottom of display
    int tmag = map(oledMenu.mValueEntered, oledMenu.mValueLow, oledMenu.mValueHigh, 0 , SCREEN_WIDTH);
    display.fillRect(0, SCREEN_HEIGHT - 10, tmag, 10, WHITE);
    display.display(); // update display

    tTime = (unsigned long)(millis() - oledMenu.lastMenuActivity); // time since last activity
  } while (_blocking && encodeur.buttonPressed() == false && tTime < (menuTimeout * 1000)); // if in blocking mode repeat until button is pressed or timeout

  if (_blocking) menuMode = off;

  return oledMenu.mValueEntered;  // Utilisé en mode blocking
}

//==============================================//
// Création d'un menu depuis une liste
// ex :       String tList[]={"main menu", "2", "3", "4", "5", "6"};
//            createList("demo_list", 6, &tList[0]);

void createList(String _title, int _noOfElements, String *_list)
{
  resetMenu();                      // clear any previous menu
  menuMode = menu;                  // enable menu mode
  oledMenu.noOfmenuItems = _noOfElements;    // set the number of items in this menu
  oledMenu.menuTitle = _title;               // menus title (used to identify it)

  for (int i = 1; i <= _noOfElements; i++)
  {
    oledMenu.menuItems[i] = _list[i - 1];      // set the menu items
  }
}

//==============================================//
// 21 caracteres par ligne, "\n" pour passer à la ligne
void displayMessage(String _title, String _message)
{
  resetMenu();
  menuMode = message;

  display.clearDisplay();
  display.setTextColor(WHITE);

  // titre
  display.setCursor(0, 0);
  if (menuLargeText) {
    display.setTextSize(2);
    display.println(_title.substring(0, MaxmenuTitleLength));
  } else {
    if (_title.length() > MaxmenuTitleLength) display.setTextSize(1);
    else display.setTextSize(2);
    display.println(_title);
  }

  // message
  display.setCursor(0, topLine);
  display.setTextSize(1);
  display.println(_message);
  display.display();
}

//==============================================//
void resetMenu()
{
  // reset all menu variables / flags
  menuMode = off;
  oledMenu.selectedMenuItem = 0;
  oledMenu.noOfmenuItems = 0;
  oledMenu.menuTitle = "";
  oledMenu.highlightedMenuItem = 0;
  oledMenu.mValueEntered = 0;

  oledMenu.lastMenuActivity = millis();   // log time

  // clear oled display
  display.clearDisplay();
  display.display();
}

//==============================================//
