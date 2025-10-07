
#include "stid.h"


unsigned char lecture_normale(unsigned char * stid_id)
{
    unsigned char temps_attente,n_octet,n_bit,octet;

    STID_SENS_DATA=1;//DATA en entrée
    STID_READ=0;
    STID_CLOCK=0;
    stid_delai_us(100);
    STID_READ=1;
    temps_attente=0;
    do
    {
        stid_delai_us(1000);
        temps_attente++;
        if (STID_CODE==1)
            temps_attente=65;
    }
    while(temps_attente<65);

    if (STID_CODE==1)
    {
        n_octet=0;
        n_bit=0;
        do
        {
            octet=0;
            for(n_bit=0;n_bit<8;n_bit++)
            {
                if (STID_DATA_I==1)
                    octet=(octet|0x01);
                octet=octet<<1;
                stid_delai_us(100);
                STID_CLOCK=1;
                stid_delai_us(500);
                STID_CLOCK=0;
                stid_delai_us(400);
            }
            stid_id[n_octet]=octet;
            n_octet++;
        }
        while(STID_CODE==1);

        return n_octet;
    }
    else
    {
        return 0;
    }
}


void stid_delai_us(unsigned int isdu)
{
    while(isdu>0) //la boucle dure 12 cycles machines
    {
        Nop();Nop();Nop();
        isdu--;
    }
}