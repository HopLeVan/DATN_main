/*
 * tsl2561.h
 *
 * Created: 17/10/2024 12:50:40 AM
 *  Author: hop
 */ 


#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "ssd1306.h"

#define TSL2561_ADDR 0x39 // Địa chỉ I2C mặc định của TSL2561

// Thanh ghi điều khiển và thanh ghi dữ liệu của TSL2561
#define TSL2561_COMMAND_BIT 0x80
#define TSL2561_CONTROL_REG 0x00
#define TSL2561_TIMING_REG  0x01
#define TSL2561_DATA0_LOW   0x0C
#define TSL2561_DATA0_HIGH  0x0D
#define TSL2561_DATA1_LOW   0x0E
#define TSL2561_DATA1_HIGH  0x0F

// Lệnh bật và tắt TSL2561
#define TSL2561_POWER_ON    0x03
#define TSL2561_POWER_OFF   0x00


 void tsl2561_init(void);
 uint16_t tsl2561_readLight(void);
 void tsl2561_powerOff(void);
 void display_light_level(uint16_t light_level);
 void display_light_level1(uint16_t light_level);
 