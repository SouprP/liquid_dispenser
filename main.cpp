#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>
#include <hardware/i2c.h>

#include <i2c_scan.hpp>
#include <sensors/hc_sr04.h>
#include <sensors/bmp280.h>

#include <menu/button.h>
#include <menu/lcd.h>
#include <menu/menu.h>

#define LCD_ADDR 0x27
#define BMP_ADDR 0x76 // nothing for now
#define SDA_PIN 4
#define SCL_PIN 5

// Pump pins
#define PUMP_1 12 // ?
#define PUMP_2 13 // ?

// Button pins
#define UP_PIN     10
#define DOWN_PIN   13
#define RIGHT_PIN  10
#define LEFT_PIN   12
#define MIDDLE_PIN 11

// PumpDriver* pump_driver;
  int pump_1_val = 0;
  int pump_2_val = 0;

int read_temperature() {
    return 25; 
}

void drive_pump(){
  // pump_driver->drive(pump_1_val, pump_2_val);
}

int main() {
  i2c_inst_t* i2c = i2c0;

  // Initialize I2C once
  i2c_init(i2c, 100 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  // Initialize sensors with the same I2C instance
  BMP280 bmp280;
  bmp280.init(i2c);

  HC_SR04 sensor;
  sensor.init(3, 2);

  LCD lcd;
  lcd.lcd_init(i2c, LCD_ADDR);

  while (true) {
      lcd.lcd_clear();

      float temperature = bmp280.get_temperature();
      float pressure = bmp280.get_pressure();
      printf("Temperature: %.2f C\n", temperature);
      printf("Pressure: %.2f kPa\n", pressure / 1000.0f);

      // Display the readings on the LCD
      char buffer[16];
      snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperature);
      lcd.lcd_set_cursor(0,0);
      lcd.lcd_print_str(buffer);
      snprintf(buffer, sizeof(buffer), "Press: %.2f kPa", pressure / 1000.0f);
      lcd.lcd_set_cursor(1,0);
      lcd.lcd_print_str(buffer);

      lcd.lcd_set_cursor(2,0);
      char str[20];
      sprintf(str, "%dcm", sensor.get_cm());
      lcd.lcd_print_str(str);

      sleep_ms(1000);
  }

  return 0;

    // Initialize hardware components
    // LCD lcd;
    // Button left(LEFT_PIN), up(UP_PIN), middle(MIDDLE_PIN), down(DOWN_PIN), right(RIGHT_PIN);
    // PumpDriver pump_driver;

    // // Initialize LCD and buttons
    // lcd.lcd_init(SDA_PIN, SCL_PIN, LCD_ADDR);
    // // pump_driver.init(lcd, PUMP_1, PUMP_2);

    // lcd.lcd_clear();
    // lcd.lcd_set_cursor(0,0);
    // lcd.lcd_print_str("mikas to takt");

    // pump_driver.drive(3000, 6000);

    // Initialize main menu
    // Menu main_menu(&lcd);
    // //main_menu.init_pump(&pump_driver);
    // main_menu.init_button(&left, &up, &middle, &down, &right);

    // // Define submenus and items
    // Menu nalej_submenu(&lcd);
    // Menu system_submenu(&lcd);

    // // int pump_1_val = 0;
    // // int pump_2_val = 0;

    // nalej_submenu.add_item(MenuItem("Nalej", MENU_TYPE_CALLBACK, drive_pump));
    // nalej_submenu.add_item(MenuItem("Pump 1", MENU_TYPE_NUMBER_INPUT, nullptr, &pump_1_val));
    // nalej_submenu.add_item(MenuItem("Pump 2", MENU_TYPE_NUMBER_INPUT, nullptr, &pump_2_val));
    // nalej_submenu.add_item(MenuItem("Exit", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &main_menu));

    // system_submenu.add_item(MenuItem("Temp", MENU_TYPE_VALUE_READ, nullptr, nullptr, read_temperature));
    // system_submenu.add_item(MenuItem("Exit", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &main_menu));

    // main_menu.add_item(MenuItem("Nalej Sub", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &nalej_submenu));
    // main_menu.add_item(MenuItem("System Sub", MENU_TYPE_DEFAULT, nullptr, nullptr, nullptr, &system_submenu));

    // // Run the main menu
    // main_menu.run();

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