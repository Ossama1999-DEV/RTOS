#include "rxtx.h"

static void rxtx_send_byte_blocking(unsigned char data);

void init_rxtx(void)
{
    TRISCbits.TRISC6 = 0; // TX1 output
    TRISCbits.TRISC7 = 1; // RX1 input
    BAUDCON1 = 0x08;      // baudrate 16 bits
    SPBRGH1 = 0x04;       // 48000000/4(1249+1)=9600 (1249=0x04E1)
    SPBRG1 = 0xE1;        // speed 9600 bauds
    TXSTA1 = 0x04;        // 8 bits, asynchronous, high speed
    RCSTA1 = 0x80;        // 8 bits, serial port enable
    TXSTA1 = 0x24;        // TX Enable
    RCSTA1 = 0x90;        // RX enable
}

static void rxtx_send_byte_blocking(unsigned char data)
{
    while (PIR1bits.TX1IF == 0)
        ;
    TXREG1 = data;
    while (TXSTA1bits.TRMT == 0)
        ;
}

void rxtx_send_string(const char *message)
{
    if (message == 0)
        return;

    P(SEM_RXTX);
    while (*message != '\0')
    {
        rxtx_send_byte_blocking((unsigned char)*message);
        message++;
    }
    V(SEM_RXTX);
}
