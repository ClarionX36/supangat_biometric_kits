#include "MenuManager.h"
#include "OLED_Display.h"

uint8_t MenuManager::currentOption = 0;

void MenuManager::showMainMenu() {
  OLED_Display::showMainMenu();
}

int MenuManager::getCurrentOption() {
  return currentOption;
}

void MenuManager::nextOption() {
  currentOption = (currentOption + 1) % 2;
}
