/*
 * ssd1306.h
 *
 * Created: 7/10/2024 12:50:40 AM
 *  Author: hop
 */ 


#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
//#include "font.h"

// SSD1306 I2C address
#define SSD1306_I2C_ADDRESS 0x78

// SSD1306 commands
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY 0xA6

// #define SSD1306_WIDTH 128
// #define SSD1306_HEIGHT 64

// // Các lệnh điều khiển OLED
// #define SSD1306_SET_COLUMN_ADDR 0x21
// #define SSD1306_SET_PAGE_ADDR 0x22

#define BLACK 0
#define WHITE 1

void ssd1306_init(void);
void ssd1306_command(uint8_t command);
void ssd1306_data(uint8_t data);
//void ssd1306_display_pixel(uint8_t x, uint8_t y);
void ssd1306_clear_display(void);
void ssd1306_display_bytes(uint8_t x, uint8_t y, uint8_t* bytes);
void ssd1306_display_char(uint8_t x, uint8_t y, char c);
void ssd1306_display_string(uint8_t x, uint8_t y,char* str);
void ssd1306_clear_line(uint8_t line);
void ssd1306_display_begin(void);
void ssd1306_display_off();
void ssd1306_display_on();
void display_int(uint16_t int_value);

#endif /* SSD1306_H_ */