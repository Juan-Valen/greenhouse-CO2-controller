#ifndef MENUITEM_H
#define MENUITEM_H
#include "ssd1306os.h"
#include <string>

class MenuItem {
public:
  enum menuEvent { up, down, ok, back };
  MenuItem(std::string menu_title, ssd1306os display);
  virtual const std::string &title() const;
  virtual bool event(menuEvent e) = 0;
  virtual void show() = 0;
  virtual ~MenuItem() = default;

protected:
  std::string menu_title;
  ssd1306os display;
};
#endif
