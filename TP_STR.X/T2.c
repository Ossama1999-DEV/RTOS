/**!
 * @file T2.c
 * @author DBIBIH Oussama
 * @brief Tache 2 : Gestion du TouchPad
 * @version 1.0
 */
#include "T2.h"

void tache2(void)
{
    unsigned char hx,lx,hy,ly;
    unsigned int ix,iy;

    TP_appui=0;

    while(1)
    {
        DRIVEA=1;// Right=Vcc Left=GND Top en l'air
        DRIVEB=0;// Bottom en l'air
        tp_delai(3000);// D�lai d'�tablissement du signal ~= 3000 us

        P(SEM_CAN);

        ADCON2=0x16; // Fosc/64 Tacq=4TAD
        ADCON0=0x01; // Module AD ON channel 0 (X)
        ADCON0=0x03; //D�but de conversion
        while(ADCON0bits.DONE==1)
            ;
        hx=ADRESH;
        lx=ADRESL;

        V(SEM_CAN);

        if ((lx&0x0F)!=0){hx=0;lx=0;}// Si tension n�gative


        if (hx>=10)
        {
            DRIVEA=0;// Right en l'air Left en l'air Top=Vcc
            DRIVEB=1;// Bottom=GND
            tp_delai(3000);// D�lai d'�tablissement du signal ~= 3000 us

            P(SEM_CAN);

            ADCON2=0x16; // Fosc/64 Tacq=4TAD
            ADCON0=0x05; // Module AD ON channel 1 (Y)
            ADCON0=0x07; //D�but de conversion
            while(ADCON0bits.DONE==1)
                ;
            hy=ADRESH;
            ly=ADRESL;

            V(SEM_CAN);

            if ((ly&0x0F)!=0){hy=0;ly=0;}// Si tension n�gative

            if (hy>=20)
            {
                ix=lx+(((unsigned int)(hx))<<8);
                iy=ly+(((unsigned int)(hy))<<8);
                ix=ix-3150; // Offset
                iy=iy-6500; // Offset

                TP_x=ix/246; //Gain X
                TP_y=127-(iy/409); //Gain Y

                TP_appui=1;
            }
            else
            {
                TP_appui=0;
            }
        }
        else
        {
            TP_appui=0;
        }
    }
}

void tp_delai(unsigned int itpd)
{
    while(itpd>0) //la boucle dure 12 cycles machines
    {
        Nop();Nop();Nop();
        itpd--;
    }
}