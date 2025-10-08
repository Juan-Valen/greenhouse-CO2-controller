#include "Menu.h"
#include "ssd1306os.h"

#include <iostream>

Menu::Menu(std::string menu_title, ssd1306os display)
        : MenuItem(menu_title, display), position(0), selection(-1) {}

bool Menu::event(MenuItem::menuEvent e) {
    bool handled = true;
    if (items.empty())
        return false;

    if (selection < 0) {
        switch (e) {
            case MenuItem::up:
                ++position;
                if (position > items.size() - 1)
                    position = items.size() - 1;
                ;
                break;
            case MenuItem::down:
                --position;
                if (position < 0)
                    position = 0;
                break;
            case MenuItem::ok:
                selection = position;
                break;
            case MenuItem::back:
                handled = false;
                break;
        }
    } else if (!items[selection]->event(e)) {
        selection = -1;
    }

    return handled;
}

void Menu::show() {
    if (selection != -1) {
        items[position]->show();
    } else {

        int index = 0;
        display.fill(0);
        display.text("=" + menu_title + "=", 0, 0);
        display.text(">", 0, 10 * (position + 1));
        for (auto &item : items) {
            display.text(item->title(), 8, 10 * (index + 1));
            ++index;
        }
        display.show();
    }
}

void Menu::add_item(std::shared_ptr<MenuItem> item) {
    items.push_back(std::move(item));
}
