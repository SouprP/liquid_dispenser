#include <menu/button.h>

Button::Button(uint gpio_pin) : gpio_pin(gpio_pin), 
        debounce_time(200000), 
        last_press_time(get_absolute_time()) {
    
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
    //gpio_pull_up(gpio_pin); // Enable pull-up resistor
}

bool Button::is_pressed() {
    if (debounce() && gpio_get(gpio_pin) == 1) {
        last_press_time = get_absolute_time();
        return true;
    }
    return false;
}

void Button::set_debounce_time(uint32_t debounce_time) {
    this->debounce_time = debounce_time;
}

bool Button::debounce() {
    absolute_time_t now = get_absolute_time();
    int64_t elapsed_time = absolute_time_diff_us(last_press_time, now);
    return elapsed_time > debounce_time;
}