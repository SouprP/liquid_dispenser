#include <menu/menu.h>

Menu* Menu::instance = nullptr;

Menu::Menu(LCD lcd, BMP280 bmp280, PumpDriver pump_driver, Button up, Button down, Button middle, Button left, Button right) 
    : up_button(up), down_button(down), middle_button(middle), left_button(left), right_button(right){
        this->lcd = lcd;
        this->bmp280 = bmp280;
        this->pump_driver = pump_driver;

        // up_button = up;
        // down_button = down;
        // middle_button = middle;
        // left_button = left;
        // right_button = right;

        liquid_value = DEFAULT_LIQUID_AMOUNT;
        pump_1_value = 50;
        pump_2_value = 50;

        is_editing = false;

        current_page = Page::MENU_ROOT;
        current_index = 0;

        instance = this;
}

void Menu::run() {
    display();

    multicore_launch_core1(Menu::update_buttons_task);

    while(true)
        handle_input();
}

void Menu::display(){
    lcd.clear();
    for(uint8_t i = 0; i < 4; i++){
        lcd.set_cursor(i, 0);
        if(current_index == i){
            lcd.print_char('>');

            if(is_editing){
                lcd.set_cursor(i, 19);
                lcd.print_str("#");
            }
            continue;
        }

        lcd.print_char(' ');
    }

    switch (current_page)
    {
        case MENU_ROOT:
            display_root();
            break;
        
        case POURING_PAGE:
            display_pour();
            break;  

        case SETTINGS_PAGE:
            display_settings();
            break;

        default:
            break;
    }
}

void Menu::handle_input(){
    // sleep_ms(500);
    input_avaliable = true;

    if(up_button.was_down()){
        if(can_navigate(false))
            current_index--;
    }

    else if(down_button.was_down()){
        if(can_navigate(true))
            current_index++;
    }

    else if(middle_button.was_down()){
        handle_action();
    }

    else{
        input_avaliable = false;
    }

    if(input_avaliable){
        sleep_ms(300);
        display();
    }
    else if (current_page == Page::SETTINGS_PAGE && RT_UPDATE)
    {
        sleep_ms(300);
        display();
    }
    
}

void Menu::display_root(){
    lcd.set_cursor(0, 1);
    lcd.print_str("POURING [A]");

    lcd.set_cursor(1, 1);
    lcd.print_str("SETTINGS [A]");

    lcd.set_cursor(2, 1);
    lcd.print_str("CLEANING [A]");

    lcd.set_cursor(3, 1);
    lcd.print_str("Action[A] Input[IN]");
}

void Menu::display_pour(){
    char buffer[20];

    lcd.set_cursor(0, 1);
    lcd.print_str("POUR [A]");

    lcd.set_cursor(1, 1);
    sprintf(buffer, "PUMP_1 %d%% [IN]", pump_1_value);
    lcd.print_str(buffer);

    lcd.set_cursor(2, 1);
    sprintf(buffer, "PUMP_2 %d%% [IN]", pump_2_value);
    lcd.print_str(buffer);

    lcd.set_cursor(3, 1);
    lcd.print_str("EXIT [A]");
}

void Menu::display_settings(){
    char buffer[20];

    lcd.set_cursor(0, 1);
    sprintf(buffer, "LIQUID: %dml [IN]", liquid_value);
    lcd.print_str(buffer);

    lcd.set_cursor(1, 1);
    sprintf(buffer, "PRESSURE: %.1fPa", bmp280.get_pressure());
    lcd.print_str(buffer);

    lcd.set_cursor(2, 1);
    sprintf(buffer, "TEMP: %.2fC", bmp280.get_temperature());
    lcd.print_str(buffer);

    lcd.set_cursor(3, 1);
    lcd.print_str("EXIT [A]");
}

bool Menu::can_navigate(bool down){
    switch (current_page)
    {
        case MENU_ROOT:
            // can go down?
            if(down){
                if(current_index == MAIN_MAX_INDEX)
                    return false;

                return true;
            }

            // can go up?
            if(current_index == 0)
                return false;
            
            return true;
            
            break;

        case POURING_PAGE:
            // can go down?
            if(down){
                if(current_index == POUR_MAX_INDEX)
                    return false;

                return true;
            }
            
            // can go up?
            if(current_index == 0)
                return false;

            return true;

            break;

        case SETTINGS_PAGE:
            // can go down?
            if(down){
                if(current_index == SETTINGS_MAX_INDEX)
                    return false;

                return true;
            }
            
            // can go up?
            if(current_index == 0)
                return false;

            return true;

            break;

        default:
            break;
    }

    // shouldnt get here
    return false;
}

void Menu::handle_action(){
    switch (current_page)
    {
        case Page::MENU_ROOT:
            if(current_index == MAIN_SUB::POURING){
                handle_menu_change(Page::POURING_PAGE);
            }
            else if(current_index == MAIN_SUB::SETTINGS){
                handle_menu_change(Page::SETTINGS_PAGE);
            }
            else if(current_index == MAIN_SUB::CLEANING){
                hanlde_cleaning();
            }

            break;

        case Page::POURING_PAGE:
            if(current_index == POURING_SUB::POUR){
                handle_pour();
            }
            else if(current_index == POURING_SUB::PUMP_1){
                handle_var_action(VAR_TYPE::PUMP_1_VAR);
            }
            else if(current_index == POURING_SUB::PUMP_2){
                handle_var_action(VAR_TYPE::PUMP_2_VAR);
            }
            else if(current_index == POURING_SUB::EXIT_P){
                handle_menu_change(Page::MENU_ROOT);
            }

            
            break;

        case Page::SETTINGS_PAGE:
            if(current_index == SETTINGS_SUB::LIQUID){
                handle_var_action(VAR_TYPE::LIQUID_VAR);
            }
            else if(current_index == SETTINGS_SUB::PRESSURE){
                // should tight_loop_contents() be used?
                // do nothing
            }
            else if(current_index == SETTINGS_SUB::TEMP){
                // do nothing
            }
            else if(current_index == SETTINGS_SUB::EXIT_S){
                handle_menu_change(Page::MENU_ROOT);
            }

        default:
            break;
    }
}

void Menu::handle_var_action(VAR_TYPE var_type){
    is_editing = true;
    display();

    while(is_editing){
        input_avaliable = true;
        if(left_button.was_down()){
            change_var(var_type, true);
        }
        else if(right_button.was_down()){
            change_var(var_type, false);
        }
        else if(middle_button.was_down()){
            is_editing = false;
        }
        else{
            input_avaliable = false;
        }

        if(input_avaliable){
            display();
        }
        sleep_ms(200);
    }
    display();
}

void Menu::change_var(VAR_TYPE var_type, bool decrease){
    if(decrease){
        // liquid amount 
        if(var_type == VAR_TYPE::LIQUID_VAR)
            if(liquid_value >= LIQUID_VALUE_CHANGE){
                liquid_value -= LIQUID_VALUE_CHANGE;
                return;
            }

        // pump 1 percentage
        if(var_type == VAR_TYPE::PUMP_1_VAR)
            if(pump_1_value >= PUMP_PERC_CHANGE){
                pump_1_value -= PUMP_PERC_CHANGE;
                return;
            }

        // pump 2 percentage
        if(var_type == VAR_TYPE::PUMP_2_VAR)
            if(pump_2_value >= PUMP_PERC_CHANGE){
                pump_2_value -= PUMP_PERC_CHANGE;
                return;
            }

        return;
    }

    // liquid amount 
    if(var_type == VAR_TYPE::LIQUID_VAR)
        if(true){
            liquid_value += LIQUID_VALUE_CHANGE;
            return;
        }

    // pump 1 percentage
    if(var_type == VAR_TYPE::PUMP_1_VAR)
        if(pump_1_value < 100 && pump_1_value < 100 - pump_2_value){
            pump_1_value += PUMP_PERC_CHANGE;
            return;
        }

    // pump 2 percentage
    if(var_type == VAR_TYPE::PUMP_2_VAR)
        if(pump_2_value < 100 && pump_2_value < 100 - pump_1_value){
            pump_2_value += PUMP_PERC_CHANGE;
            return;
        }
}

void Menu::handle_menu_change(Page new_page){
    current_page = new_page;
    current_index = 0;
}

void Menu::update_buttons_task() {
    while (true) {
        if (instance != nullptr) {
            instance->up_button.update_state();
            instance->down_button.update_state();
            instance->middle_button.update_state();
            instance->left_button.update_state();
            instance->right_button.update_state();
        }
        sleep_ms(150); //  polling rate
    }
}

void Menu::handle_pour(){
    // uint64_t ml_1 = (uint64_t) (liquid_value / pump_1_value);
    // uint64_t ml_2 = (uint64_t) (liquid_value / pump_2_value);
    uint64_t ml_1 = (uint64_t) (liquid_value * ((float) pump_1_value / 100));
    uint64_t ml_2 = (uint64_t) (liquid_value * ((float) pump_2_value / 100));
    pump_driver.drive(ml_1, ml_2);
}

void Menu::hanlde_cleaning(){
    pump_driver.drive(CLEAN_AMOUNT, CLEAN_AMOUNT);
}