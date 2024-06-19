#ifndef HC_SR04_HPP_
#define HC_SR04_HPP_

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <stdio.h>

class HC_SR04{
    private:
        uint TRIG_PIN;
        uint ECHO_PIN;

        float convertion_rate = 58.0f;
        static const int timeout = 26100;

        uint64_t get_pulse();

    public:
        void init(uint TRIG_PIN, uint ECHO_PIN);

        uint64_t get_cm();
};
#endif