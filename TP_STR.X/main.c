/**!
 * @file main.c
 * @author DBIBIH Oussama
 * @brief Programme principal
 * @version 1.0
 */
#include "main.h"

// Prototypes
void initialisation_des_ports(void);

void main()
{
    initialisation_des_ports();
    initialisation_du_systeme();
    Init(SEM_CAN);
    Init(SEM_RXTX);

        // D�marrage du syst�me
    T0CONbits.TMR0ON=1; //D�marrage du Timer0
    T0IF=1; // For�age du d�clenchement de la premi�re IT
    ei(); //Activation des interruptions => D�marrage du syst�me

        // La suite ne sera jamais �x�cut�e
    while(1)
        Nop();
}


void initialisation_des_ports(void)
{
// D�sactivation du bus externe
    MEMCON=0xA0;    //ebdis=1 bus d�sactiv� (sauf en cas d'acc�s externe)

// D�sactivation des fonctions analogiques
    ANCON0=0x00;
    ANCON1=0x00;
    ANCON2=0x00;
    ANCON0bits.ANSEL0=1; // RA0 analogique Touch pad BOTTOM (y)
    ANCON0bits.ANSEL1=1; // RA1 analogique Touch pad LEFT (x)
    ANCON0bits.ANSEL2=1; // RA2 analogique Temp�rature d'eau
    ANCON0bits.ANSEL3=1; // RA3 analogique Temp�rature d'huile
    ANCON1bits.ANSEL10=1; // RF5 analogique Joystick (x)
    ANCON1bits.ANSEL11=1; // RF6 analogique Joystick (y)


// Valeur initiale des ports en sortie
    LED_R=1;
    LED_G=1;
    LED_B=1;

// D�finition du sens des ports
    TRISCbits.TRISC2=0; //LED_R en sortie
    TRISCbits.TRISC1=0; //LED_G en sortie
    TRISGbits.TRISG0=0; //LED_B en sortie
    TRISGbits.TRISG3=0; //DRIVEA en sortie
    TRISGbits.TRISG4=0; //DRIVEB en sortie
    TRISDbits.TRISD2=0; //STID_CLOCK en sortie
    TRISDbits.TRISD3=0; //STID_READ en sortie

// Mise en place des pull up
    INTCON2bits.RBPU=0; // Pull up PORTB activ�
    PADCFG1bits.REPU=1; // Pull up PORTE activ�

}

unsigned char lecture_8bit_analogique(unsigned char channel)
{
    unsigned char ret;

    P(SEM_CAN);
    ADCON1=0;//Vref+ = Vdd / Vref- = Vss / pas de canal negatif
    ADCON2=0x16; // Fosc/64 Left justified Tacq=4Tad(5.3�s)
    channel=channel&0x1F; // les valeurs acceptables sont 0 � 31
    channel=channel<<2;
    ADCON0=channel|0x01; //S�lection du canal et d�marrage du Module AD

    ADCON0bits.GO=1;
    while(ADCON0bits.DONE==1)
        ;

    if ((ADRESL&0x01)==0x01)
        ret=0;
    else
        ret=ADRESH;
    V(SEM_CAN);

    return ret;
}