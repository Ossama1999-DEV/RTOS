// /**!
//  * \file main.c
//  * \brief Point d'entrée principal du programme
//  * \author DBIBIH Oussama
//  */
// #include "main.h"

// // Prototypes
// void initialisation_des_ports(void);

// void main(void)
// {
//     initialisation_des_ports();
//     initialisation_du_systeme();
//     Init(SEM_CAN);

//     // Démarrage du système
//     T0CONbits.TMR0ON=1; //D�marrage du Timer0
//     T0IF=1; // For�age du declenchement de la premiere IT
//     ei(); //Activation des interruptions => Démarrage du système

//     // La suite ne sera jamais exécutée
//     while(1)
//         Nop();
// }


// void initialisation_des_ports(void)
// {
// // Désactivation du bus externe
//     MEMCON=0xA0;    //ebdis=1 bus désactivé (sauf en cas d'accès externe)

// // Désactivation des fonctions analogiques
//     ANCON0=0x00;
//     ANCON1=0x00;
//     ANCON2=0x00;
//     ANCON0bits.ANSEL0   =1; // RA0 analogique Touch pad BOTTOM (y)
//     ANCON0bits.ANSEL1   =1; // RA1 analogique Touch pad LEFT (x)
//     ANCON0bits.ANSEL2   =1; // RA2 analogique Temperature d'eau
//     ANCON0bits.ANSEL3   =1; // RA3 analogique Temperature d'huile
//     ANCON1bits.ANSEL10  =1; // RF5 analogique Joystick (x)
//     ANCON1bits.ANSEL11  =1; // RF6 analogique Joystick (y)


// // Valeur initiale des ports en sortie
//     LED_R=1;
//     LED_G=1;
//     LED_B=1;

// // Définition du sens des ports
//     TRISCbits.TRISC2    =0; //LED_R en sortie
//     TRISCbits.TRISC1    =0; //LED_G en sortie
//     TRISGbits.TRISG0    =0; //LED_B en sortie
//     TRISGbits.TRISG3    =0; //DRIVEA en sortie
//     TRISGbits.TRISG4    =0; //DRIVEB en sortie
//     TRISDbits.TRISD2    =0; //STID_CLOCK en sortie
//     TRISDbits.TRISD3    =0; //STID_READ en sortie

// // Mise en place des pull up
//     INTCON2bits.RBPU    =0; // Pull up PORTB active
//     PADCFG1bits.REPU    =1; // Pull up PORTE active

// }

// unsigned char lecture_8bit_analogique(unsigned char channel)
// {
//     unsigned char ret;

//     //P(SEM_CAN);
//     ADCON1=0;               //Vref+ = Vdd / Vref- = Vss / pas de canal negatif
//     ADCON2=0x16;            // Fosc/64 Left justified Tacq=4Tad(5.3µs)
//     channel=channel&0x1F;   // les valeurs acceptables sont 0 a 31
//     channel=channel<<2;
//     ADCON0=channel|0x01;    //Selection du canal et demarrage du Module AD

//     ADCON0bits.GO=1;
//     while(ADCON0bits.DONE==1)
//         ;

//     if ((ADRESL&0x01)==0x01)
//         ret=0;
//     else
//         ret=ADRESH;
//     //V(SEM_CAN);

//     return ret;
// }
/**
 * @file main.c
 * @author 
 *  DBIBIH Oussama
 * @brief Scénario complet du chariot élévateur (RTOS)
 */

#include <xc.h>
#include "scenarios.h"
#include "systeme.h"
#include "semaphore.h"

#include "main.h"

// ===================================================
// === CONFIGURATION DU PIC ===
// ===================================================


// ===================================================
// === Fonctions internes ===
// ===================================================
void initialisation_des_ports(void);
void scenario_chariot_elevateur(void);

// ===================================================
// === MAIN PRINCIPAL ===
// ===================================================
void main(void)
{
    initialisation_des_ports();
    initialisation_du_systeme();
    Init(SEM_CAN);

    T0CONbits.TMR0ON = 1;   // Démarrage du Timer0
    T0IF = 1;               // Forçage de la première IT
    ei();                   // Activation des interruptions

    scenario_chariot_elevateur();

    while (1)
        Nop();
}

// ===================================================
// === INITIALISATION DES PORTS ===
// ===================================================
void initialisation_des_ports(void)
{
    MEMCON = 0xA0; // Désactivation du bus externe

    // Désactivation des fonctions analogiques inutiles
    ANCON0 = 0x00;
    ANCON1 = 0x00;
    ANCON2 = 0x00;

    ANCON0bits.ANSEL0 = 1;  // RA0 analogique Touch pad Y
    ANCON0bits.ANSEL1 = 1;  // RA1 analogique Touch pad X
    ANCON0bits.ANSEL2 = 1;  // Température eau
    ANCON0bits.ANSEL3 = 1;  // Température huile
    ANCON1bits.ANSEL10 = 1; // RF5 Joystick X
    ANCON1bits.ANSEL11 = 1; // RF6 Joystick Y

    // LEDs
    LED_R = 1;
    LED_G = 1;
    LED_B = 1;

    TRISCbits.TRISC2 = 0; // LED_R
    TRISCbits.TRISC1 = 0; // LED_G
    TRISGbits.TRISG0 = 0; // LED_B
    TRISGbits.TRISG3 = 0; // DRIVEA
    TRISGbits.TRISG4 = 0; // DRIVEB

    INTCON2bits.RBPU = 0;  // Pull-up PORTB
    PADCFG1bits.REPU = 1;  // Pull-up PORTE
}

// ===================================================
// === LECTURE ANALOGIQUE ===
// ===================================================
unsigned char lecture_8bit_analogique(unsigned char channel)
{
    unsigned char ret;
    ADCON1 = 0;
    ADCON2 = 0x16;
    channel = (unsigned char)((channel & 0x1F) << 2);
    ADCON0 = channel | 0x01;

    ADCON0bits.GO = 1;
    while (ADCON0bits.DONE)
        ;

    ret = (ADRESL & 0x01) ? 0 : ADRESH;
    return ret;
}

// ===================================================
// === COMPORTEMENT DU CHARIOT ===
// ===================================================
void attendre(uint16_t ms) { for (uint16_t i = 0; i < ms * 100; i++) Nop(); }

void afficher_message(const char *msg) { /* UART/LCD afficher msg */ }

void avancer_chariot(void)  { DRIVEA = 1; DRIVEB = 0; }
void reculer_chariot(void)  { DRIVEA = 0; DRIVEB = 1; }
void arreter_chariot(void)  { DRIVEA = 0; DRIVEB = 0; }

void lever_materiel(void)      { /* moteur levage ON */ }
void descendre_materiel(void)  { /* moteur descente ON */ }
void stabiliser_hauteur(void)  { /* maintien position */ }

void alerter(const char *msg)  { LED_R = 0; /* buzzer ON */ }
void surveiller_obstacles(void){ /* capteurs IR/US */ }
void gerer_vitesse(void)       { /* PWM joystick */ }
void mettre_a_jour_affichage(void) { /* LCD refresh */ }

// ===================================================
// === SCÉNARIO PRINCIPAL DU CHARIOT ===
// ===================================================
void scenario_chariot_elevateur(void)
{
    afficher_message("INITIALISATION DU CHARIOT...");
    LED_G = 0;
    attendre(1000);

    unsigned char joystick_x = lecture_8bit_analogique(JOYSTICK_X);
    unsigned char joystick_y = lecture_8bit_analogique(JOYSTICK_Y);

    if (joystick_y > 200)
        avancer_chariot();
    else if (joystick_y < 50)
        reculer_chariot();
    else
        arreter_chariot();

    unsigned char capteur_levage = lecture_8bit_analogique(CAPTEUR_HAUTEUR);
    if (capteur_levage < 100)
        lever_materiel();
    else if (capteur_levage > 200)
        descendre_materiel();
    else
        stabiliser_hauteur();

    unsigned char temp_moteur = lecture_8bit_analogique(CAPTEUR_TEMP_MOTEUR);
    if (temp_moteur > 220)
    {
        LED_R = 0;
        alerter("SURCHAUFFE MOTEUR !");
        arreter_chariot();
    }

    while (1)
    {
        surveiller_obstacles();
        gerer_vitesse();
        mettre_a_jour_affichage();
        attendre(50);
    }
}
