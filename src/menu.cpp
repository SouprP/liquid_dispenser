#include <menu/menu.h>
#include <vector>
#include <functional>
#include <cstdio>

Menu::Menu(LCD* lcd_display)
    : lcd(lcd_display), current_index(0), up(nullptr), down(nullptr), right(nullptr), left(nullptr), middle(nullptr) {}

void Menu::add_item(const MenuItem& item) {
    items.push_back(item);
}

void Menu::init_button(Button* left_btn, Button* up_btn, Button* middle_btn, Button* down_btn, Button* right_btn) {
    left = left_btn;
    up = up_btn;
    middle = middle_btn;
    down = down_btn;
    right = right_btn;
}

void Menu::display() {
    lcd->lcd_clear();
    for (size_t i = 0; i < items.size(); ++i) {
        lcd->lcd_set_cursor(i, 0);
        if (i == current_index) {
            lcd->lcd_print_str(">");
        } else {
            lcd->lcd_print_str(" ");
        }
        lcd->lcd_print_str(items[i].display_name);
        if (items[i].type == MENU_TYPE_VALUE && items[i].value) {
            char buffer[10];
            sprintf(buffer, ": %d", *items[i].value);
            lcd->lcd_print_str(buffer);
        } else if (items[i].type == MENU_TYPE_VALUE_READ && items[i].value_read_function) {
            char buffer[10];
            sprintf(buffer, ": %d", items[i].value_read_function());
            lcd->lcd_print_str(buffer);
        }
    }
}

void Menu::handle_input() {
    bool up_b = up->is_pressed();
    bool down_b = down->is_pressed();
    bool middle_b = middle->is_pressed();
    bool rigtt_b = right->is_pressed();
    bool left_b = left->is_pressed();
    // while (!up->is_pressed() && !down->is_pressed() && !right->is_pressed() && !left->is_pressed() && !middle->is_pressed()) {
    //     // just wait and do nothing
    // }

    while(!up_b && !down_b && !middle_b && !rigtt_b && !left_b){
        // just wait and do nothing
    }

    if (up_b) {
        current_index = (current_index - 1 + items.size()) % items.size();
    } else if (down_b) {
        current_index = (current_index + 1) % items.size();
    } else if (middle_b) {
        MenuItem& item = items[current_index];
        switch (item.type) {
            case MENU_TYPE_CALLBACK:
                if (item.callback) item.callback();
                break;
            case MENU_TYPE_NUMBER_INPUT:
                while (!middle->is_pressed()) {
                    if (left->is_pressed() && item.value) {
                        if (*item.value >= 10) (*item.value) -= 10;
                    }
                    if (right->is_pressed() && item.value) {
                        (*item.value) += 10;
                    }
                    display();
                }
                break;
            case MENU_TYPE_DEFAULT:
            case MENU_TYPE_VALUE:
            case MENU_TYPE_VALUE_READ:
                if (item.submenu) {
                    item.submenu->run();
                }
                break;
        }
    }
    display();
}

void Menu::run() {
    display();
    while (true) {
        handle_input();
    }
}


// kolos
// historia mikroprocesorów

    // bool up_b = up->is_pressed();
    // bool down_b = down->is_pressed();
    // bool middle_b = middle->is_pressed();
    // bool rigtt_b = right->is_pressed();
    // bool left_b = left->is_pressed();
    // // while (!up->is_pressed() && !down->is_pressed() && !right->is_pressed() && !left->is_pressed() && !middle->is_pressed()) {
    // //     // just wait and do nothing
    // // }

    // while(!up_b && !down_b && !middle_b && !rigtt_b && !left_b){
    //     // just wait and do nothing
    // }