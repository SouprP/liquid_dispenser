#ifndef MENU_H_
#define MENU_H_

#include <menu/button.h>
#include <menu/lcd.h>
#include <pump_driver.h>
#include <vector>
#include <functional>

enum MenuType {
    MENU_TYPE_DEFAULT,
    MENU_TYPE_VALUE,
    MENU_TYPE_VALUE_READ,
    MENU_TYPE_NUMBER_INPUT,
    MENU_TYPE_CALLBACK,
};

class Menu;

class MenuItem {
public:
    const char* display_name;
    MenuType type;
    std::function<void()> callback;
    int* value;
    std::function<int()> value_read_function;
    Menu* submenu;

    MenuItem(const char* name, MenuType t, std::function<void()> cb = nullptr, int* val = nullptr, std::function<int()> val_func = nullptr, Menu* sub = nullptr)
        : display_name(name), type(t), callback(cb), value(val), value_read_function(val_func), submenu(sub) {}
};

class Menu {
private:
    LCD* lcd;
    std::vector<MenuItem> items;
    uint8_t current_index;

    Button* up;
    Button* down;
    Button* right;
    Button* left;
    Button* middle;

    void display();
    void handle_input();

public:
    Menu(LCD* lcd_display);

    void add_item(const MenuItem& item);
    void init_button(Button* left_btn, Button* up_btn, Button* middle_btn, Button* down_btn, Button* right_btn);
    void run();
};

#endif // MENU_H_
