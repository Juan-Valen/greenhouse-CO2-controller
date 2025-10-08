#ifndef INFOITEM_H
#define INFOITEM_H
#include "MenuItem.h"
#include "ssd1306os.h"
#include <map>
#include <string>

class InfoItem : public MenuItem {
public:
    explicit InfoItem(std::string menu_title, ssd1306os display);
    bool event(menuEvent e) override;
    void show() override;
    void updateValue(std::string key, std::string value);

private:
    std::map<std::string, std::string> values;
};
#endif