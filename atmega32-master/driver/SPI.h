#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

/* Pin Definitions for ATmega32 Port B */
#define SPI_DDR   DDRB
#define SPI_PORT  PORTB
#define SS_PIN    PB4
#define MOSI_PIN  PB5
#define MISO_PIN  PB6
#define SCK_PIN   PB7

void SPI_MasterInit(void);
void SPI_SlaveInit(void);
uint8_t SPI_Transfer(uint8_t data);
void SPI_SelectSlave(void);
void SPI_DeselectSlave(void);
void SPI_WaitForMasterSelection(void);
void SPI_WaitForMasterDeselection(void);

#endif /* SPI_H_ */