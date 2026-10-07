#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdint.h>

void USART_Init(uint32_t baud_rate, uint32_t f_cpu);
void USART_TransmitChar(char data);
void USART_TransmitString(const char *str);
char USART_ReceiveChar(void);

#endif /* USART_H_ */