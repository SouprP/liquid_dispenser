#include <menu/menu.h>
#include <stdio.h>

// Initialize the static member
Menu* Menu::current_menu = nullptr;

Menu::Menu(const char *name, Menu *parent, MenuType type) : name(name), parent(parent), type(type), num_children(0), selected_item(0), number_value(0) {
    if (parent) {
        parent->add_child(this);
    }
}

void Menu::add_child(Menu *child) {
    if (num_children < 4) {
        children[num_children++] = child;
    }
}

void Menu::display(LCD &lcd) {
    lcd.lcd_clear();
    lcd.lcd_set_cursor(0, 0);
    lcd.lcd_print_str(name);

    if (type == MENU_TYPE_NUMBER_INPUT) {
        lcd.lcd_set_cursor(1, 0);
        char buffer[16];
        sprintf(buffer, "Value: %d", number_value);
        lcd.lcd_print_str(buffer);
    } else {
        for (uint8_t i = 0; i < num_children; i++) {
            lcd.lcd_set_cursor(i + 1, 0);
            if (i == selected_item) {
                lcd.lcd_print_str("> ");
            } else {
                lcd.lcd_print_str("  ");
            }
            lcd.lcd_print_str(children[i]->name);
        }
    }
}

void Menu::handle_input(Button &up, Button &down, Button &right, Button &left, Button &middle) {
    while (!up.is_pressed() && !down.is_pressed() && !right.is_pressed() && !left.is_pressed() && !middle.is_pressed()) {
        // just wait and do nothing
    }

    if (type == MENU_TYPE_NUMBER_INPUT) {
        if (right.is_pressed()) {
            number_value++;
            printf("right\n");
        } else if (left.is_pressed()) {
            number_value--;
            printf("Left\n");
        } else if (middle.is_pressed()) {
            // Exit number input mode
            printf("exit\n");
            set_current_menu(parent);
        }
    } else {
        if (up.is_pressed()) {
            if (selected_item > 0) {
                selected_item--;
            }
        } else if (down.is_pressed()) {
            if (selected_item < num_children - 1) {
                selected_item++;
            }
        // } else if (right.is_pressed() && children[selected_item] != nullptr) {
        //     set_current_menu(children[selected_item]);
        // } else if (left.is_pressed() && parent != nullptr) {
        //     set_current_menu(parent);
        } else if (middle.is_pressed()) {
            // Enter number input mode if the selected item is of that type
            if (children[selected_item]->type == MENU_TYPE_NUMBER_INPUT) {
                set_current_menu(children[selected_item]);
            } else {
                // Implement action for middle button if needed
                printf("Action on %s\n", children[selected_item]->name);
            }
        }
    }

    // Wait for all buttons to be released before continuing
    while (up.is_pressed() || down.is_pressed() || right.is_pressed() || left.is_pressed() || middle.is_pressed()) {
        // just wait and do nothing
    }
}

Menu* Menu::get_current_menu() {
    return current_menu;
}

void Menu::set_current_menu(Menu *menu) {
    current_menu = menu;
}