#include <menu/lcd.h>

/**
 * 
 *          PROGRAM VARIABLES
 * 
*/

// uint8_t LCD_ADDR;

/**
 * 
 *          CORE FUNCTIONS
 * 
*/

void LCD::lcd_init(uint8_t SDA_PIN, uint8_t SCL_PIN, uint8_t addr){
    LCD_ADDR = addr;

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Make the I2C pins available to picotool
    // bi_decl( bi_2pins_with_func(sda, scl, GPIO_FUNC_I2C)); For some reason this produces an error.

    lcd_command(0x03, LCD_COMMAND, 1);
    lcd_command(0x03, LCD_COMMAND, 1);
    lcd_command(0x03, LCD_COMMAND, 1);
    lcd_command(0x02, LCD_COMMAND, 1);

    lcd_command(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT, LCD_COMMAND, 1);
    lcd_command(LCD_FUNCTION_SET | LCD_2LINE, LCD_COMMAND, 1);
    lcd_command(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON, LCD_COMMAND, 1);
    lcd_clear();
}

void LCD::i2c_write_byte(uint8_t val){
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

void LCD::lcd_command(uint8_t val, uint8_t mode, uint8_t backlight){
    uint8_t high;
    uint8_t low;

    if(backlight)
    {
        high = mode | (val & 0xF0) | LCD_BACKLIGHT;
        low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;
    }
    else {
        high = mode | (val & 0xF0);
        low = mode | ((val << 4) & 0xF0);
    }

    i2c_write_byte(high);
    lcd_display_handle(high);
    i2c_write_byte(low);
    lcd_display_handle(low);
}

/**
 * 
 *           FUNCTIONALITY 
 * 
*/

void LCD::lcd_clear(){
    lcd_command(LCD_CLEAR_DISPLAY, LCD_COMMAND, backlight_state);
}

void LCD::lcd_set_cursor(uint8_t line, uint8_t position){
    uint8_t line_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    uint8_t val = 0x80 + line_offsets[line] + position;
    lcd_command(val, LCD_COMMAND, backlight_state);
}

void LCD::lcd_print_char(char val){
    lcd_command(val, LCD_CHARACTER, backlight_state);
}

void LCD::lcd_print_str(const char *str){
    while(*str)
        lcd_print_char(*str++);
}

void LCD::lcd_print_custom(uint8_t location, uint8_t charmap[]){
    location &= 0x7; // we only have 8 locations 0-7
    lcd_command(LCD_SET_CGRAM_ADDR | (location << 3), LCD_COMMAND, 1);
    for (int i=0; i<8; i++) {
        lcd_command(charmap[i], 1, 0);
    }
}

void LCD::lcd_display_handle(uint8_t val){
    #define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void LCD::lcd_backlight(bool enabled){
    if(enabled){
        lcd_command(0x0C, LCD_COMMAND, 1);
        backlight_state = 1;
        return;
    }

    lcd_command(0x08, LCD_COMMAND, 0);
    backlight_state = 0;
}   