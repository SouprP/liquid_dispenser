#include <sensors/hc_sr04.h>


void HC_SR04::init(uint TRIG_PIN, uint ECHO_PIN){
    this->TRIG_PIN = TRIG_PIN;
    this->ECHO_PIN = ECHO_PIN;

    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);

    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

uint64_t HC_SR04::get_pulse(){
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    uint64_t width = 0;
    while(gpio_get(ECHO_PIN) == 0) 
        tight_loop_contents();

    absolute_time_t start_time = get_absolute_time();
    while(gpio_get(ECHO_PIN) == 1){
        width++;
        sleep_us(1);
        if(width > timeout)
            return 0;
    }

    absolute_time_t end_time = get_absolute_time();

    return absolute_time_diff_us(start_time, end_time);
}

uint64_t HC_SR04::get_cm(){
    return (long) get_pulse() / convertion_rate;
}