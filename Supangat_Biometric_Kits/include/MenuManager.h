#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <Arduino.h>

class MenuManager {
public:
    static void showMainMenu();
    static int getCurrentOption();
    static void nextOption();
private:
    static uint8_t currentOption;
};

#endif
