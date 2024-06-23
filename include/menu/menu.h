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
 *          CLEANING
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

#define MAIN_MAX_INDEX 2
#define POUR_MAX_INDEX 3
#define SETTINGS_MAX_INDEX 3

#define CLEAN_AMOUNT 1000

#define LIQUID_VALUE_CHANGE 25
#define PUMP_PERC_CHANGE 10
#define DEFAULT_LIQUID_AMOUNT 250

#define RT_UPDATE 0 

enum Page {
    MENU_ROOT,
    POURING_PAGE,
    SETTINGS_PAGE
};

enum MAIN_SUB{
    POURING,
    SETTINGS,
    CLEANING,
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

        // pump driver
        PumpDriver pump_driver;

        // max liquid value
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

        /**
         * @brief Displays the menu on the screen based on the currently
         * selected menu page.
         */
        void display();

        /**
         * @brief Handles user input.
         */
        void handle_input();

        /**
         * @brief Handle the usage of the MIDDLE button.
         */
        void handle_action();

        /**
         * @brief Handles the usage of PumpDriver object when selecting
         * the POUR option.
         */
        void handle_pour();

        /**
         * @brief Handles the usage of PumpDriver object when selecting
         * the CLEANING option
         */
        void hanlde_cleaning();

        /**
         * @brief This function should be launched
         * on another core using multicore_launch_core1 command. It allows
         * for reading button states while also displaying the menu without interference.
         */
        static void update_buttons_task();

        /**
         *      HELPER FUNCTIONS
         */

        /**
         * @brief Helper function for displaying ROOT menu.
         */
        void display_root();

        /**
         * @brief Helper function for displaying POUR menu.
         */
        void display_pour();

        /**
         * @brief Helper function for displaying SETTINGS menu.
         */
        void display_settings();

        /**
         * @brief Helper function for handling variable editing using buttons.
         * 
         * @param var_type type of variable that is edited.
         */
        void handle_var_action(VAR_TYPE var_type);

        /**
         * @brief Helper function for chaning current menu page.
         * 
         * @param new_page the page we want to go to.
         */
        void handle_menu_change(Page new_page);

        /**
         * @brief Helper function for checking if
         * there is an avaliable item to selected one index up or down.
         * 
         * @param down do we want navigate down.
         */
        bool can_navigate(bool down);

        /**
         * @brief Changes the selected variable value by an amount provided
         * at the of menu.h file.
         * 
         * @param var_type type of variable that is being edited.
         * @param decrease should the variable be decreased.
         */
        void change_var(VAR_TYPE var_type, bool decrease);

    public:
        /**
         * @brief Menu object initialization.
         * 
         * @param lcd the LCD object that handles connection to the LCD display.
         * @param bmp280 the BMP280 object that handles connection to the BMP280 device.
         * @param pump_driver the PumpDriver object that handles pump driving.
         * @param up up button.
         * @param down down button.
         * @param middle middle button.
         * @param left left button.
         * @param right right button.
         */
        Menu(LCD lcd, BMP280 bmp280, PumpDriver pump_driver
            , Button up, Button down, Button middle, Button left, Button right);
        void run();
};
#endif