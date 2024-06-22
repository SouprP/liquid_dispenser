#ifndef BUTTON_H_
#define BUTTON_H_

#include <pico/stdlib.h>
#include <hardware/gpio.h>

// #define DEFAULT_DEBOUNCE 200000
#define DEFAULT_DEBOUNCE 400000

class Button {
    private:
        uint gpio_pin;
        uint32_t debounce_time;
        absolute_time_t last_press_time;

        bool down_state;

        bool debounce();

    public:
        Button(uint gpio_pin);

        bool is_pressed();
        bool was_down();

        void set_debounce_time(uint32_t debounce_time);
        void update_state();
};

#endif