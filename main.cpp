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
#define BMP_ADDR 0x76
#define SDA_PIN 4
#define SCL_PIN 5

// Pump pins
#define PUMP_PIN_1 12 // ?
#define PUMP_PIN_2 13 // ?

// Button pins
#define UP_PIN     6
#define DOWN_PIN   7
#define MIDDLE_PIN 8
#define LEFT_PIN   9
#define RIGHT_PIN  10

int main() {
  // stdio_init_all();

  Button up(UP_PIN);
  Button down(DOWN_PIN);
  Button middle(MIDDLE_PIN);
  Button left(LEFT_PIN);
  Button right(RIGHT_PIN);


  i2c_inst_t* i2c = i2c0;

  // Initialize I2C once
  i2c_init(i2c, 100 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  // // Initialize sensors with the same I2C instance
  BMP280 bmp280;
  bmp280.init(i2c);

  // HC_SR04 sensor;
  // sensor.init(3, 2);

  LCD lcd;
  lcd.init(i2c, LCD_ADDR);

  PumpDriver pump_driver;
  pump_driver.init(lcd, PUMP_PIN_1, PUMP_PIN_2);

  Menu menu(lcd, bmp280, pump_driver, up, down, middle, left, right);
  menu.run();

  // while (true) {
  //     lcd.clear();

  //     float temperature = bmp280.get_temperature();
  //     float pressure = bmp280.get_pressure();
  //     printf("Temperature: %.2f C\n", temperature);
  //     printf("Pressure: %.2f kPa\n", pressure / 1000.0f);

  //     // Display the readings on the LCD
  //     char buffer[16];
  //     snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperature);
  //     lcd.set_cursor(0,0);
  //     lcd.print_str(buffer);
  //     snprintf(buffer, sizeof(buffer), "Press: %.2f kPa", pressure / 1000.0f);
  //     lcd.set_cursor(1,0);
  //     lcd.print_str(buffer);

  //     lcd.set_cursor(2,0);
  //     char str[20];
  //     sprintf(str, "%dcm", sensor.get_cm());
  //     lcd.print_str(str);

  //     sleep_ms(1000);
  // }

  // while(true){
  //   lcd.clear();
  //   lcd.set_cursor(0,0);

  //   if(upButton.is_pressed())
  //     lcd.print_str("UP");

  //   if(downButton.is_pressed())
  //     lcd.print_str("DOWN");

  //   if(middleButton.is_pressed())
  //     lcd.print_str("MIDDLE");

  //   if(leftButton.is_pressed())
  //     lcd.print_str("LEFT");
    
  //   if(rightButton.is_pressed())
  //     lcd.print_str("RIGTH");

  //   sleep_ms(50);
  // }

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