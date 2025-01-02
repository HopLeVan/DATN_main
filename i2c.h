/*
 * i2c.h
 *
 * Created: 7/11/2024 3:05:36 PM
 *  Author: hop
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define I2C_ADDR 0x50 // 0x10100000 
#define F_CPU 8000000UL

/* TODO: setup i2c/twi */
#define F_I2C			100000UL// clock i2c
#define PSC_I2C			1		// prescaler i2c
#define SET_TWBR		(F_CPU/F_I2C-16UL)/(PSC_I2C*2UL)

#define I2C_START		0			// bit 0: timeout start-condition
#define I2C_SENDADRESS	1			// bit 0: timeout device-adress
#define I2C_BYTE		2			// bit 0: timeout byte-transmission
#define I2C_READACK		3			// bit 0: timeout read acknowledge
#define I2C_READNACK	4			// bit 0: timeout read nacknowledge


void i2c_init(void);	//init hw-i2c
void i2c_start(uint8_t i2c_addr);		//send i2c_start_condition
void i2c_stop(void);		//send i2c_stop_condition
void i2c_byte(uint8_t byte);		// send data_byte
void i2c_byte_string(char *str);	// send data_byte is string

uint8_t i2c_readAck(void);          // read byte with ACK
uint8_t i2c_readNAck(void);         // read byte with NACK

void i2c_writeToAddress(uint8_t i2c_addr, uint8_t mem_addr, uint8_t data);	//write data to address
uint8_t i2c_readFromAddress(uint8_t i2c_addr, uint8_t mem_addr);			//read data from address



#endif /* I2C_H_ */