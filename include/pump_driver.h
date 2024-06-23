#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <menu/lcd.h>
#include <sensors/hc_sr04.h>

class PumpDriver{
    private:
        // ml to time in ms convertion rate
        // 35ml/s => 28,21ms/ml
        //float FLOW_CONV = 28.21f;
        //float FLOW_CONV = 32.0f;

        // amount of ms needed for 1ml of liquid to be moved to a cup
        float FLOW_CONV = 30.0f;

        LCD lcd;
        HC_SR04 sensor;

        uint PUMP_1;
        uint PUMP_2;

    public:
        /**
         * @brief Initialized the PumpDriver object.
         * 
         * @param lcd LCD object will handle display of information about liquid pouring.
         * @param PUMP_1 GPIO pin of pump 1.
         * @param PUMP_2 GPIO pin of pump 2.
         */
        void init(LCD lcd, uint PUMP_1, uint PUMP_2);

        /**
         * @brief Activate the poruing mechanism.
         * 
         * @param ml_1 amount of liquid to be poured via the first pump.
         * @param ml_2 amount of liquid to be poured via the second pump.
         */
        void drive(uint64_t ml_1, uint64_t ml_2);
};