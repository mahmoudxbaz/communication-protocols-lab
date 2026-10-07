#include "SPI.h"

void SPI_MasterInit(void) {
    /* Set MOSI, SCK, and SS as output, MISO as input */
    SPI_DDR |= (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN);
    SPI_DDR &= ~(1 << MISO_PIN);
    
    /* Keep SS high initially */
    SPI_PORT |= (1 << SS_PIN);
    
    /* Enable SPI, Master mode, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_SlaveInit(void) {
    /* Set MISO as output, MOSI, SCK, SS as input */
    SPI_DDR |= (1 << MISO_PIN);
    SPI_DDR &= ~((1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << SS_PIN));
    
    /* Enable SPI */
    SPCR = (1 << SPE);
}

uint8_t SPI_Transfer(uint8_t data) {
    /* Start transmission */
    SPDR = data;
    
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    
    /* Return Data Register */
    return SPDR;
}

void SPI_SelectSlave(void) {
    SPI_PORT &= ~(1 << SS_PIN);
}

void SPI_DeselectSlave(void) {
    SPI_PORT |= (1 << SS_PIN);
}

void SPI_WaitForMasterSelection(void){
    while (PINB & (1 << SS_PIN)); // Wait while SS is 1 
}

void SPI_WaitForMasterDeselection(void){
    while (!(PINB & (1 << SS_PIN))); // Wait while SS is 0
}