#include "MenuItem.h"
#include "ssd1306os.h"

MenuItem::MenuItem(std::string menu_title, ssd1306os display)
        : menu_title(menu_title), display(display) {}

const std::string &MenuItem::title() const { return menu_title; }