#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <menu/lcd.h>
#include <sensors/hc_sr04.h>

class PumpDriver{
    private:
        // ml to time in ms convertion rate
        // 35ml/s => 28,21ms/ml
        float FLOW_CONV = 28.21f;

        LCD lcd;
        HC_SR04 sensor;

        uint PUMP_1;
        uint PUMP_2;

    public:
        void init(LCD lcd, uint PUMP_1, uint PUMP_2);
        void drive(uint64_t ml_1, uint64_t ml_2);
};