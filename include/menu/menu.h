#ifndef MENU_H_
#define MENU_H_

#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <menu/button.h>
#include <menu/lcd.h>

#include <pump_driver.h>
#include <sensors/bmp280.h>

/**
 *      MAIN_ROOT
 *          POURING
 *          SETTINGS_SUB
 *          
 *      POURING
 *          POUR
 *          PUMP_1 : INPUT
 *          PUMP_2 : INPUT
 *          EXIT
 * 
 *      SETTINGS
 *          MAX_POUR: INPUT
 *          TEMP : VAL
 *          PRESS : VAL
 *          EXIT
 * 
 */

#define MAIN_MAX_INDEX 1
#define POUR_MAX_INDEX 3
#define SETTINGS_MAX_INDEX 3

#define LIQUID_VALUE_CHANGE 25
#define PUMP_PERC_CHANGE 10
#define DEFAULT_LIQUID_AMOUNT 100

#define RT_UPDATE 0 

enum Page {
    MENU_ROOT,
    POURING_PAGE,
    SETTINGS_PAGE
};

enum MAIN_SUB{
    POURING,
    SETTINGS,
};

enum SETTINGS_SUB{
    LIQUID,
    PRESSURE,
    TEMP,
    EXIT_S,
};

enum POURING_SUB{
    POUR,
    PUMP_1,
    PUMP_2,
    EXIT_P,
};

enum VAR_TYPE{
    LIQUID_VAR,
    PUMP_1_VAR,
    PUMP_2_VAR,
};

class Menu {
    private:
        /**
         *      DISPLAY VARIABLES
         */

        // LCD display
        LCD lcd;

        // All buttons for navigation handling
        Button up_button;
        Button down_button;
        Button middle_button;
        Button left_button;
        Button right_button;

        bool input_avaliable;

        /**
         *      SENSOR VARIABLES
         */

        // BMP280 sensor for temperature and pressure reading
        BMP280 bmp280;
        
        /**
         *      PUMP VARIABLES
         */

        // Pump driver
        PumpDriver pump_driver;

        // Max liquid value
        // should be HIGHER than 0
        // default will be set to something like 250
        int liquid_value;

        // These values should be between 0 - 100%
        uint8_t pump_1_value;
        uint8_t pump_2_value;

        /**
         *      PAGE VARIABLES
         */

        Page current_page;
        uint8_t current_index;
        bool is_editing;

        /**
         *      CORE FUNCTIONS
         */
        
        static Menu* instance;

        void display();
        void handle_input();
        void handle_action();
        void handle_pour();

        static void update_buttons_task();

        /**
         *      HELPER FUNCTIONS
         */

        void display_root();
        void display_pour();
        void display_settings();

        void handle_var_action(VAR_TYPE var_type);
        void handle_menu_change(Page new_page);

        bool can_navigate(bool down);
        void change_var(VAR_TYPE var_type, bool decrease);

    public:
        Menu(LCD lcd, BMP280 bmp280, PumpDriver pump_driver
            , Button up, Button down, Button middle, Button left, Button right);
        void run();
};
#endif