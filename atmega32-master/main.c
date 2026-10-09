#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "driver/SPI.h"
#include "driver/I2C.h"

#define DS1307_I2C_ADDR 0xD0

uint8_t BCDToDec(uint8_t val) {
    return (((val >> 4) * 10) + (val & 0x0F));
}

void RTC_Init(void) {
    /* Write 0 to Register 0x00 to enable the Oscillator (CH bit = 0) */
    I2C_Start();
    I2C_Write(DS1307_I2C_ADDR);
    I2C_Write(0x00); /* SECONDS register address */
    I2C_Write(0x00); /* Clear CH bit and set 00 seconds */
    I2C_Stop();
}

int main(void) {
    uint8_t rtc_raw[7];
    uint8_t rtc_data[7];

    I2C_Init(F_CPU, 100000UL); /* 100kHz I2C */
    SPI_MasterInit();

    _delay_ms(100);
    RTC_Init();

    while (1) {
        /* Read 7 RTC Bytes */
        I2C_Start();
        I2C_Write(DS1307_I2C_ADDR);
        I2C_Write(0x00);

        I2C_Start(); /* Repeated Start */
        I2C_Write(DS1307_I2C_ADDR | 1);

        for (uint8_t i = 0; i < 6; i++) {
            rtc_raw[i] = I2C_ReadAck();
        }
        rtc_raw[6] = I2C_ReadNack();
        I2C_Stop();

        /* Process and Mask Control Bits */
        rtc_data[0] = BCDToDec(rtc_raw[0] & 0x7F); /* Sec: Mask CH bit */
        rtc_data[1] = BCDToDec(rtc_raw[1] & 0x7F); /* Min: Mask reserved bit */
        
        /* Hours: Mask Bit 6 (12/24hr mode) and Bit 5 (AM/PM) */
        if (rtc_raw[2] & 0x40) {
            /* 12-Hour Mode */
            rtc_data[2] = BCDToDec(rtc_raw[2] & 0x1F);
        } else {
            /* 24-Hour Mode */
            rtc_data[2] = BCDToDec(rtc_raw[2] & 0x3F);
        }

        rtc_data[3] = BCDToDec(rtc_raw[3] & 0x07); /* Day */
        rtc_data[4] = BCDToDec(rtc_raw[4] & 0x3F); /* Date */
        rtc_data[5] = BCDToDec(rtc_raw[5] & 0x1F); /* Month */
        rtc_data[6] = BCDToDec(rtc_raw[6]);        /* Year */

        /* Send frame to Slave over SPI */
        SPI_SelectSlave();

        SPI_Transfer(0xAA); /* Sync header */

        for (uint8_t i = 0; i < 7; i++) {
            SPI_Transfer(rtc_data[i]);
        }

        SPI_DeselectSlave();

        _delay_ms(1000);
    }

    return 0;
}
