// bmp280.cpp
#include <sensors/bmp280.h>
#include <cstdio>

// Registers and other constants
#define ADDR _u(0x76)
#define REG_CONFIG _u(0xF5)
#define REG_CTRL_MEAS _u(0xF4)
#define REG_RESET _u(0xE0)
#define REG_TEMP_XLSB _u(0xFC)
#define REG_TEMP_LSB _u(0xFB)
#define REG_TEMP_MSB _u(0xFA)
#define REG_PRESSURE_XLSB _u(0xF9)
#define REG_PRESSURE_LSB _u(0xF8)
#define REG_PRESSURE_MSB _u(0xF7)
#define REG_DIG_T1_LSB _u(0x88)
#define REG_DIG_T1_MSB _u(0x89)
#define REG_DIG_T2_LSB _u(0x8A)
#define REG_DIG_T2_MSB _u(0x8B)
#define REG_DIG_T3_LSB _u(0x8C)
#define REG_DIG_T3_MSB _u(0x8D)
#define NUM_CALIB_PARAMS 24

// struct bmp280_calib_param {
//     uint16_t dig_t1;
//     int16_t dig_t2;
//     int16_t dig_t3;
//     uint16_t dig_p1;
//     int16_t dig_p2;
//     int16_t dig_p3;
//     int16_t dig_p4;
//     int16_t dig_p5;
//     int16_t dig_p6;
//     int16_t dig_p7;
//     int16_t dig_p8;
//     int16_t dig_p9;
// };

void BMP280::write_register(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(i2c, address, buf, 2, false);
}

void BMP280::read_registers(uint8_t reg, uint8_t* buf, uint8_t len) {
    i2c_write_blocking(i2c, address, &reg, 1, true);  // true to keep master control of bus
    i2c_read_blocking(i2c, address, buf, len, false);  // false to release bus
}

void BMP280::get_calib_params() {
    uint8_t buf[NUM_CALIB_PARAMS] = {0};
    read_registers(REG_DIG_T1_LSB, buf, NUM_CALIB_PARAMS);

    calib_params.dig_t1 = (uint16_t)(buf[1] << 8) | buf[0];
    calib_params.dig_t2 = (int16_t)(buf[3] << 8) | buf[2];
    calib_params.dig_t3 = (int16_t)(buf[5] << 8) | buf[4];

    calib_params.dig_p1 = (uint16_t)(buf[7] << 8) | buf[6];
    calib_params.dig_p2 = (int16_t)(buf[9] << 8) | buf[8];
    calib_params.dig_p3 = (int16_t)(buf[11] << 8) | buf[10];
    calib_params.dig_p4 = (int16_t)(buf[13] << 8) | buf[12];
    calib_params.dig_p5 = (int16_t)(buf[15] << 8) | buf[14];
    calib_params.dig_p6 = (int16_t)(buf[17] << 8) | buf[16];
    calib_params.dig_p7 = (int16_t)(buf[19] << 8) | buf[18];
    calib_params.dig_p8 = (int16_t)(buf[21] << 8) | buf[20];
    calib_params.dig_p9 = (int16_t)(buf[23] << 8) | buf[22];
}

int32_t BMP280::convert_temp(int32_t raw_temp) {
    int32_t var1 = ((((raw_temp >> 3) - ((int32_t)calib_params.dig_t1 << 1))) * ((int32_t)calib_params.dig_t2)) >> 11;
    int32_t var2 = (((((raw_temp >> 4) - ((int32_t)calib_params.dig_t1)) * ((raw_temp >> 4) - ((int32_t)calib_params.dig_t1))) >> 12) * ((int32_t)calib_params.dig_t3)) >> 14;
    int32_t t_fine = var1 + var2;
    return (t_fine * 5 + 128) >> 8;
}

int32_t BMP280::convert_pressure(int32_t raw_pressure, int32_t raw_temp) {
    int32_t t_fine = convert_temp(raw_temp);
    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)calib_params.dig_p6;
    var2 = var2 + ((var1 * (int64_t)calib_params.dig_p5) << 17);
    var2 = var2 + (((int64_t)calib_params.dig_p4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_params.dig_p3) >> 8) + ((var1 * (int64_t)calib_params.dig_p2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_params.dig_p1) >> 33;
    if (var1 == 0) {
        return 0;  // avoid exception caused by division by zero
    }
    int64_t p = 1048576 - raw_pressure;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_params.dig_p9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_params.dig_p8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_params.dig_p7) << 4);
    return (int32_t)p;
}

void BMP280::init(i2c_inst_t* i2c_instance, uint8_t addr) {
    i2c = i2c_instance;
    address = addr;

    // Use the "handheld device dynamic" optimal setting (see datasheet)
    uint8_t buf[2];
    buf[0] = REG_CONFIG;
    buf[1] = ((0x04 << 5) | (0x05 << 2)) & 0xFC;
    i2c_write_blocking(i2c, address, buf, 2, false);

    buf[0] = REG_CTRL_MEAS;
    buf[1] = (0x01 << 5) | (0x03 << 2) | (0x03);
    i2c_write_blocking(i2c, address, buf, 2, false);

    get_calib_params();
}

void BMP280::read_raw(int32_t* raw_temp, int32_t* raw_pressure) {
    uint8_t buf[6];
    read_registers(REG_PRESSURE_MSB, buf, 6);

    *raw_pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    *raw_temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
}

float BMP280::get_temperature() {
    int32_t raw_temp, raw_pressure;
    read_raw(&raw_temp, &raw_pressure);
    int32_t temp = convert_temp(raw_temp);
    return temp / 100.0;
}

float BMP280::get_pressure() {
    int32_t raw_temp, raw_pressure;
    read_raw(&raw_temp, &raw_pressure);
    int32_t pressure = convert_pressure(raw_pressure, raw_temp);
    return pressure / 25600.0;
}
