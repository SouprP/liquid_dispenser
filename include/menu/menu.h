#ifndef MENU_H_
#define MENU_H_

#include <menu/button.h>
#include <menu/lcd.h>

enum MenuType {
    MENU_TYPE_DEFAULT,
    MENU_TYPE_NUMBER_INPUT
};

class Menu {
public:
    Menu(const char *name, Menu *parent = nullptr, MenuType type = MENU_TYPE_DEFAULT);

    void add_child(Menu *child);
    void display(LCD &lcd);
    void handle_input(Button &up, Button &down, Button &right, Button &left, Button &middle);

    static Menu* get_current_menu();
    static void set_current_menu(Menu *menu);

private:
    const char *name;
    Menu *parent;
    Menu *children[4];
    uint8_t num_children;
    uint8_t selected_item;
    MenuType type;
    int number_value;

    static Menu *current_menu;
};

#endif // MENU_H
