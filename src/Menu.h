#ifndef MENU_H
#define MENU_H
#include "MenuItem.h"
#include "ssd1306os.h"
#include <memory>
#include <string>
#include <vector>

class Menu : public MenuItem {
public:
  explicit Menu(std::string menu_title, ssd1306os display);
  bool event(menuEvent e) override;
  void show() override;
  void add_item(std::shared_ptr<MenuItem> item);

private:
  std::vector<std::shared_ptr<MenuItem>> items;
  int position;
  int selection;
};

#endif
