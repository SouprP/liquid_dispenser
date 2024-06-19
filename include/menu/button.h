#ifndef BUTTON_H_
#define BUTTON_H_

#include <pico/stdlib.h>
#include <hardware/gpio.h>

class Button {
    public:
        Button(uint gpio_pin);
        bool is_pressed();
        void set_debounce_time(uint32_t debounce_time);

    private:
        uint gpio_pin;
        uint32_t debounce_time;
        absolute_time_t last_press_time;

        bool debounce();
};

#endif