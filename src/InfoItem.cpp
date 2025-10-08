#include "InfoItem.h"
#include "ssd1306os.h"

InfoItem::InfoItem(std::string menu_title, ssd1306os display)
    : MenuItem(menu_title, display) {};
bool InfoItem::event(MenuItem::menuEvent e) {
  bool handled = true;
  if (e == MenuItem::ok) {
    show();
    handled = false;
  }

  return handled;
}

void InfoItem::show() {
  int index = 0;
  display.fill(0);
  display.text("=" + menu_title + "=", 0, 0);
  for (const auto &pair : values) {
    display.text(pair.first + ": " + pair.second, 0, 10 * (index + 1));
    ++index;
  }
  display.show();
}

void InfoItem::updateValue(std::string key, std::string value) {
  values[key] = value;
};
