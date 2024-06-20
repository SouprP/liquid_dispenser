#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>

#include <i2c_scan.hpp>
#include <sensors/hc_sr04.h>

#include <menu/button.h>
#include <menu/lcd.h>
#include <menu/menu.h>

#define LCD_ADDR 0x27
#define BMP_ADDR // nothing for now
#define SDA_PIN 4
#define SCL_PIN 5

// Pump pins
#define PUMP_1 22 // ?
#define PUMP_2 23 // ?

// Button pins
#define UP_PIN     10
#define DOWN_PIN   13
#define RIGHT_PIN  10
#define LEFT_PIN   12
#define MIDDLE_PIN 11

PumpDriver* pump_driver;
  int pump_1_val = 0;
  int pump_2_val = 0;

int read_temperature() {
    return 25; 
}

void drive_pump(){
  pump_driver->drive(pump_1_val, pump_2_val);
}

int main() {
    // Initialize hardware components
    LCD* lcd;
    Button left(LEFT_PIN), up(UP_PIN), middle(MIDDLE_PIN), down(DOWN_PIN), right(RIGHT_PIN);
    // PumpDriver* pump_driver;

    // Initialize LCD and buttons
    lcd->lcd_init(SDA_PIN, SCL_PIN, LCD_ADDR);
    pump_driver->init(lcd, PUMP_1, PUMP_2);

    // Initialize main menu
    Menu main_menu(lcd);
    //main_menu.init_pump(&pump_driver);
    main_menu.init_button(&left, &up, &middle, &down, &right);

    // Define submenus and items
    Menu nalej_submenu(lcd);
    Menu system_submenu(lcd);

    // int pump_1_val = 0;
    // int pump_2_val = 0;

    nalej_submenu.add_item(MenuItem("Nalej", MENU_TYPE_CALLBACK, drive_pump));
    nalej_submenu.add_item(MenuItem("Pump 1", MENU_TYPE_NUMBER_INPUT, nullptr, &pump_1_val));
    nalej_submenu.add_item(MenuItem("Pump 2", MENU_TYPE_NUMBER_INPUT, nullptr, &pump_2_val));
    nalej_submenu.add_item(MenuItem("Exit", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &main_menu));

    system_submenu.add_item(MenuItem("Temp", MENU_TYPE_VALUE_READ, nullptr, nullptr, read_temperature));
    system_submenu.add_item(MenuItem("Exit", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &main_menu));

    main_menu.add_item(MenuItem("Nalej Sub", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &nalej_submenu));
    main_menu.add_item(MenuItem("System Sub", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &system_submenu));

    // Run the main menu
    main_menu.run();

    return 0;
}


// kolos
/**
 * zadania zamkniete
 * brak ujemnych punktow
 * kartka A4
 * kalkulator
 * 
 * wszystkie wykłady
*/