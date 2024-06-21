#ifndef BMP280_H_
#define BMP280_H_

#include <hardware/i2c.h>
#include <pico/stdlib.h>

struct bmp280_calib_param {
    // temperature params
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    // pressure params
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
};

class BMP280 {
private:
    uint8_t address;
    i2c_inst_t* i2c;
    struct bmp280_calib_param calib_params;

    void write_register(uint8_t reg, uint8_t value);
    void read_registers(uint8_t reg, uint8_t* buf, uint8_t len);
    void get_calib_params();
    int32_t convert_temp(int32_t raw_temp);
    int32_t convert_pressure(int32_t raw_pressure, int32_t raw_temp);

public:
    void init(i2c_inst_t* i2c_instance, uint8_t addr = 0x76);
    void reset();
    void read_raw(int32_t* raw_temp, int32_t* raw_pressure);
    float get_temperature();
    float get_pressure();
};

#endif // BMP280_H_
