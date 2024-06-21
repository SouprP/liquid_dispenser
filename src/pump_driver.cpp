#include <pump_driver.h>

void PumpDriver::init(LCD lcd, uint PUMP_1, uint PUMP_2){
    this->PUMP_1 = PUMP_1;
    this->PUMP_2 = PUMP_2;

    this->lcd = lcd;
    sensor.init(3, 2);

    gpio_init(PUMP_1);
    gpio_init(PUMP_2);

    gpio_set_dir(PUMP_1, GPIO_OUT);
    gpio_set_dir(PUMP_2, GPIO_OUT);

    gpio_put(PUMP_1, 1);
    gpio_put(PUMP_2, 1);
}

void PumpDriver::drive(uint64_t ml_1, uint64_t ml_2){
    // stdio_init_all();
    sleep_ms(5000);

    uint64_t time_1 = ml_1 * FLOW_CONV;
    uint64_t time_2 = ml_2 * FLOW_CONV;

    lcd.lcd_backlight(true);
    // while(1){
    //     uint64_t cm = sensor.get_cm();
    //     char str[20];
    //     sprintf(str, "%dcm", cm);
    //     lcd.lcd_clear();
    //     lcd.lcd_set_cursor(0,0);
    //     lcd.lcd_print_str(str);
    //     // printf("%dcm\n", cm);
    //     sleep_ms(500);
    // }
    // return;

    // sleep_us(100);
    // if(sensor.get_cm() <= 15){
    //     sleep_ms(50);
    //     lcd.lcd_clear();
    //     lcd.lcd_set_cursor(1, 5);
    //     lcd.lcd_print_str("BRAK SZKLANKI");

    //     // loop till there is no cup
    //     while(sensor.get_cm() <= 15){
    //         uint64_t cm = sensor.get_cm();
    //         printf("%dcm", cm);
    //         sleep_ms(50);
    //     }
    // } 

    lcd.lcd_clear();
    lcd.lcd_set_cursor(0,0);
    lcd.lcd_print_str("pumping");
    if(time_1 > time_2){
        gpio_put(PUMP_1, 0);
        gpio_put(PUMP_2, 0);

        sleep_us(10);

        sleep_ms(time_2);
        gpio_put(PUMP_2, 1);

        sleep_ms(time_1 - time_2);
        gpio_put(PUMP_1, 1);

        return;
    }

    // time_1 == time_2 OR time_2 > time_1
    gpio_put(PUMP_1, 0);
    gpio_put(PUMP_2, 0);

    sleep_us(10);

    sleep_ms(time_1);
    gpio_put(PUMP_1, 1);

    sleep_ms(time_2 - time_1);
    gpio_put(PUMP_2, 1);

    lcd.lcd_clear();
    lcd.lcd_set_cursor(0,0);
    lcd.lcd_print_str("done");
}