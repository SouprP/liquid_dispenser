#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>

#include <i2c_scan.hpp>
#include <sensors/hc_sr04.h>

#include <menu/button.h>
#include <menu/lcd.h>
#include <menu/menu.h>

#define SDA_PIN 4
#define SCL_PIN 5

// Button pins
#define UP_PIN     10
#define DOWN_PIN   13
#define RIGHT_PIN  10
#define LEFT_PIN   12
#define MIDDLE_PIN 11

int main() {
  //stdio_init_all();

  // HC_SR04 sensor;
  // sensor.init(3, 2);

  // LCD lcd;

  // //i2c_scan(PIN_SDA, PIN_SCL);
  // lcd.lcd_init(SDA_PIN, SCL_PIN, 0x27);
  // lcd.lcd_set_cursor(0, 0);
  // lcd.lcd_print_str("mikas to cwel haha");

  // sleep_ms(2500);
  // while(true){
  //   sleep_ms(1000);
  //   lcd_clear();
  //   sleep_ms(500);

  //   uint8_t num = get_rand_32();
  //   char c = (char) ((num + 70) % 120);
  //   lcd_set_cursor(0, 0);
  //   lcd_print_char(c);
  // }

  // sleep_ms(2500);
  // while(true){
  //   lcd.lcd_clear();
  //   // lcd_backlight(false);

  //   char str[20];
  //   sprintf(str, "%dcm", sensor.get_cm());

  //   lcd.lcd_set_cursor(0,0);
  //   lcd.lcd_print_str(str);
  //   sleep_ms(1000);
  // }

  // stdio_init_all();

  // // Button button1(10);
  // // Button button2(11);
  // // Button button3(12);
  // // gpio_init(11);
  // // gpio_set_dir(11, GPIO_IN);
  // Button* mid = new Button(11);

  // while (true) {
  //   if(mid->is_pressed()){
  //     lcd.lcd_clear();
  //     lcd.lcd_set_cursor(0,0);
  //     lcd.lcd_print_str("fucking pressed");
  //     sleep_ms(500);
  //   }
  //     lcd.lcd_clear();
  //     sleep_ms(500); // Small delay to avoid spamming the output
  //     //printf("checked\n");
  // }

    stdio_init_all();

    // Initialize buttons
    Button* up_button = new Button(UP_PIN);
    Button* down_button = new Button(DOWN_PIN);
    Button* right_button = new Button(RIGHT_PIN);
    Button* left_button = new Button(LEFT_PIN);
    Button* middle_button = new Button(MIDDLE_PIN);

    // Initialize LCD
    LCD lcd;
    lcd.lcd_init(4, 5, 0x27); // Replace with your actual SDA, SCL pins and I2C address

    // Create menu structure
    // Menu main_menu("Main Menu");
    // Menu number_input_menu("Set Value", &main_menu, MENU_TYPE_NUMBER_INPUT);
    // Menu submenu1("Submenu 1", &main_menu);
    // Menu submenu2("Submenu 2", &main_menu);

    // // Set current menu
    // Menu::set_current_menu(&main_menu);

    // Main loop
    // while (true) {
    //     Menu *current_menu = Menu::get_current_menu();
    //     current_menu->display(lcd);
    //     current_menu->handle_input(*up_button, *down_button, *right_button, *left_button, *middle_button);
    //     sleep_ms(100); // Small delay to debounce the buttons
    // }
    int val = 0;
    while(true){
      // while(!left_button->is_pressed() && !middle_button->is_pressed() 
      //   && !right_button->is_pressed()){
      //     // do nothing
      //   }

        if(right_button->is_pressed())
          val += 10;
        if(left_button->is_pressed())
          val -= 10;


        lcd.lcd_clear();
        lcd.lcd_set_cursor(0,0);
        char str[20];
        sprintf(str, "Value: %d", val);
        lcd.lcd_print_str(str);
        sleep_ms(100);
    }

    return 0;
}