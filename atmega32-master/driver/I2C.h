#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>
#include <stdint.h>

void I2C_Init(uint32_t f_cpu, uint32_t scl_freq);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNack(void);

#endif /* I2C_H_ */