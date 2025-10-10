#include "T4.h"

void tache4(void)
{
    unsigned int a;
    while(1)
    {
        //P(SEM_RXTX);
        //while(RXTX_libre==0);
        //RXTX_libre=0;
        while (PIR1bits.TX1IF==0);   TXREG1='T';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='A';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='C';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='H';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='E';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='4';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='_';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='E';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='N';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='_';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='C';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='O';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='U';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='R';while (TXSTA1bits.TRMT==0);
        while (PIR1bits.TX1IF==0);   TXREG1='S';while (TXSTA1bits.TRMT==0);
        //RXTX_libre=1;
        //V(SEM_RXTX);
        for (a=0;a<65000;a++)
            ;
    }
}
