#include "rxtx.h"


void init_rxtx(void)
{
    TRISCbits.TRISC6=0; // TX1 en sortie
    TRISCbits.TRISC7=1; // RX1 en entrée
    BAUDCON1=0x08; //baudrate 16bits
    SPBRGH1=0x04;  //48000000/4(1249+1)=9600 (1249=0x04E1)
    SPBRG1=0xE1;   //vitesse 9600 bauds
    TXSTA1=0x04; // 8 bits, asynchrone, high speed
    RCSTA1=0x80; // 8 bits, Serial port enable
    TXSTA1=0x24; // TX Enable
    RCSTA1=0x90; // RX enable
}