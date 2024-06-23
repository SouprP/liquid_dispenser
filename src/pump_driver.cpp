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
    uint64_t time_1 = (ml_1 * FLOW_CONV);
    uint64_t time_2 = (ml_2 * FLOW_CONV);
    char buffer[18];

    lcd.clear();
    uint64_t distance = sensor.get_cm();
    while(distance <= 10 || distance >= 25){
        lcd.clear();
        lcd.set_cursor(1,1);
        lcd.print_str("NO GLASS IN RANGE");

        lcd.set_cursor(2,1);
        sprintf(buffer, "10 <= X=%d <= 25", distance);
        lcd.print_str(buffer);
        sleep_ms(500);
        distance = sensor.get_cm();
    }

    lcd.clear();
    lcd.set_cursor(1,6);
    lcd.print_str("PUMPING");


    // lcd.set_cursor(2,0);
    // sprintf(buffer, "time_1: %d", time_1);
    // lcd.print_str(buffer);

    // lcd.set_cursor(3,0);
    // sprintf(buffer, "time_2: %d", time_2);
    // lcd.print_str(buffer);

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

    lcd.clear();
}