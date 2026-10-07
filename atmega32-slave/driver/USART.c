#include "USART.h"

void USART_Init(uint32_t baud_rate, uint32_t f_cpu) {
    uint16_t ubrr_value = (uint16_t)((f_cpu / (16UL * baud_rate)) - 1);
    
    /* Set Baud Rate Registers */
    UBRRH = (uint8_t)(ubrr_value >> 8);
    UBRRL = (uint8_t)(ubrr_value);
    
    /* Enable Receiver and Transmitter */
    UCSRB = (1 << RXEN) | (1 << TXEN);
    
    /* Set Frame Format: 8 data bits, 1 stop bit, No parity */
    /* URSEL bit must be 1 when writing to UCSRC */
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void USART_TransmitChar(char data) {
    /* Wait for empty transmit buffer */
    while (!(UCSRA & (1 << UDRE)));
    
    /* Put data into buffer, sends the data */
    UDR = data;
}

void USART_TransmitString(const char *str) {
    while (*str) {
        USART_TransmitChar(*str++);
    }
}

char USART_ReceiveChar(void) {
    /* Wait for data to be received */
    while (!(UCSRA & (1 << RXC)));
    
    /* Get and return received data from buffer */
    return UDR;
}