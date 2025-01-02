/*
 * i2c.c
 *
 * Created: 10/10/2024 3:11:33 PM
 *  Author: hop
 */ 
 #include "i2c.h"
 
// #if defined(__AVR_ATmega328P__) 
 #if PSC_I2C != 1 && PSC_I2C != 4 && PSC_I2C != 16 && PSC_I2C != 64
 #error "Wrong prescaler for TWI !"
 #elif SET_TWBR < 0 || SET_TWBR > 255
 #error "TWBR out of range, change PSC_I2C or F_I2C !"
 #endif
 
 uint8_t I2C_ErrorCode;
 
 void i2c_init(void) {
    // set clock
    switch (PSC_I2C) {
	    case 4:
	    TWSR = 0x1;
	    break;
	    case 16:
	    TWSR = 0x2;
	    break;
	    case 64:
	    TWSR = 0x3;
	    break;
	    default:
	    TWSR = 0x00;
	    break;
    }
    TWBR = (uint8_t)SET_TWBR;
    // enable
    TWCR = (1 << TWEN);
 }

void i2c_start(uint8_t i2c_addr){
	// i2c start
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
	uint16_t timeout = F_CPU/F_I2C*2.0;
	while((TWCR & (1 << TWINT)) == 0 && timeout !=0){
		timeout--;
		if(timeout == 0){
			I2C_ErrorCode |= (1 << I2C_START);
			return;
		}
	};
	// send adress
	TWDR = i2c_addr;
	TWCR = (1 << TWINT)|( 1 << TWEN);
	timeout = F_CPU/F_I2C*2.0;
	while((TWCR & (1 << TWINT)) == 0 && timeout !=0){
		timeout--;
		if(timeout == 0){
			I2C_ErrorCode |= (1 << I2C_SENDADRESS);
			return;
		}
	};
}

 void i2c_stop(void) {
	 TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN); //i2c stop
	 _delay_us(10);
 }
 
void i2c_byte(uint8_t byte){
	 TWDR = byte;
	 TWCR = (1 << TWINT)|( 1 << TWEN);
	 uint16_t timeout = F_CPU/F_I2C*2.0;
	 while((TWCR & (1 << TWINT)) == 0 && timeout !=0){
		 timeout--;
		 if(timeout == 0){
			 I2C_ErrorCode |= (1 << I2C_BYTE);
			 return;
		 }
	 };
 }
 
 void i2c_byte_string(char *str){
	 while (*str) {
		 i2c_byte(*str++);
	 }
 }
 
 uint8_t i2c_readAck(void){
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 uint16_t timeout = F_CPU/F_I2C*2.0;
	 while((TWCR & (1 << TWINT)) == 0 && timeout !=0){
		 timeout--;
		 if(timeout == 0){
			 I2C_ErrorCode |= (1 << I2C_READACK);
			 return 0;
		 }
	 };
	 return TWDR;
 }
 
 uint8_t i2c_readNAck(void){
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 uint16_t timeout = F_CPU/F_I2C*2.0;
	 while((TWCR & (1 << TWINT)) == 0 && timeout !=0){
		 timeout--;
		 if(timeout == 0){
			 I2C_ErrorCode |= (1 << I2C_READNACK);
			 return 0;
		 }
	 };
	 return TWDR;
 }
 
 void i2c_writeToAddress(uint8_t i2c_addr, uint8_t mem_addr, uint8_t data) {
	 i2c_start(i2c_addr << 1); // i2c address
	 i2c_byte(mem_addr);       // Memory Address
	 i2c_byte(data);			//i2c_writeToAddress(EEPROM_ADDR, 0xBB, 1);
	 i2c_stop();
 }
 
uint8_t i2c_readFromAddress(uint8_t i2c_addr, uint8_t mem_addr) {
	 i2c_start(i2c_addr << 1);
	 i2c_byte(mem_addr);
	 i2c_start((i2c_addr << 1) | 1);
	 uint8_t data = i2c_readNAck();  // Read from address and send NACK
	 i2c_stop();

	 return data;
 }
 //output hay input, sử dụng ngắt ngoài. Thường là output, sleepmode. IO
