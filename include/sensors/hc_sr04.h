#ifndef HC_SR04_HPP_
#define HC_SR04_HPP_

#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <stdio.h>

class HC_SR04{
    private:
        // TRIG and ECHO GPIO pins
        uint TRIG_PIN;
        uint ECHO_PIN;

        // 
        float convertion_rate = 58.0f;
        static const int timeout = 26100;

        /**
         * @brief Reads the pulse anwser time length from
         * the ECHO_PIN.
         * 
         * @return pulse time length.
         */
        uint64_t get_pulse();

    public:
        /**
         * @brief Initializes the HC_SR04 sensor object.
         * 
         * @param TRIG_PIN GPIO pin of TRIG_PIN.
         * @param ECHO_PIN GPIO pin of ECHO_PIN.
         */
        void init(uint TRIG_PIN, uint ECHO_PIN);

        /**
         * @brief Gets the distance in centimeters
         * 
         * @return distance in centimeters.
         */
        uint64_t get_cm();
};
#endif