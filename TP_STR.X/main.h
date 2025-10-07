/* 
 * File:   main.h
 * Author: Franck
 *
 * Created on 29 janvier 2015, 09:29
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

// Include de base
#include <xc.h>
#include <pic18f87k22.h>
    
// Include du projet
#include "systeme.h"
#include "variables_globales.h"
#include "afficheur.h"
#include "semaphore.h"
#include "stid.h"
#include "rxtx.h"
#include "T1.h"
#include "T2.h"
#include "T3.h"
#include "T4.h"
#include "T5.h"
#include "T6.h"
    
        /* Configuration du PIC:
    Oscillateur: HS (12MHz)
    Watchdog Timer: désactivé
    Protection du code: désactivé */
#pragma config CONFIG1L = 0x15 //RC0 et RC1 actif
#pragma config CONFIG1H = 0x13 //HS1_OSC (4-16MHz) with pllx4 12MHz x 4 = 48MHz
#pragma config CONFIG2L = 0x61 //BOR et PWRT Disable
#pragma config CONFIG2H = 0x00 //WDT off
#pragma config CONFIG3L = 0x30 //0x30:Bus D:8bit A:8bit RTCC => LFINTOSC
#pragma config CONFIG3H = 0x80 //MCLR
#pragma config CONFIG4L = 0x81 //0x81=debug_off 0x01=DEBUG_ON
#pragma config CONFIG5L = 0xFF //No CODE PROTECT
#pragma config CONFIG5H = 0xC0 //No CP EEPROM + code
#pragma config CONFIG6L = 0xFF //No write protect...
#pragma config CONFIG6H = 0xE0 //No write protect config...
#pragma config CONFIG7L = 0xFF //No table read protection
#pragma config CONFIG7H = 0x40 //No table read protection

    // Défintion des broches

//PORTA
#define     BOTTOM      LATAbits.LATA0
#define     LEFT        LATAbits.LATA1
//#define     RA2    LATAbits.LATA2
//#define     RA3    LATAbits.LATA3
//#define     RA4    LATAbits.LATA4
//#define     RA5    LATAbits.LATA5

//PORTB
#define     MARCHE_AVANT        PORTBbits.RB0
#define     MARCHE_ARRIERE      PORTBbits.RB1
#define     SIEGE               PORTBbits.RB2
#define     CHOC                PORTBbits.RB3
//#define     RB4    LATBbits.LATB4
#define     MMC_CD      LATBbits.LATB5
#define     PGC         LATBbits.LATB6
#define     PGD         LATBbits.LATB7

//PORTC
#define     MMC_CS      LATCbits.LATC0
#define     LED_G       LATCbits.LATC1
#define     LED_R       LATCbits.LATC2
#define     RC3_SCK     LATCbits.LATC3
#define     RC4_MISO    LATCbits.LATC4
#define     RC5_MOSI    LATCbits.LATC5
#define     UART_RX     LATCbits.LATC6
#define     UART_TX     LATCbits.LATC7

//PORTD
#define     STID_CODE       PORTDbits.RD0
#define     STID_SENS_DATA  TRISDbits.TRISD1
#define     STID_DATA_I     PORTDbits.RD1
#define     STID_DATA_O     LATDbits.LATD1
#define     STID_CLOCK      LATDbits.LATD2
#define     STID_READ       LATDbits.LATD3
#define     RD4_MOSI    LATDbits.LATD4
#define     RD5_MISO    LATDbits.LATD5
#define     RD6_SCK     LATDbits.LATD6
//#define     RD7         LATDbits.LATD7

//PORTE
#define     VITESSE_PLUS          PORTEbits.RE0
#define     VITESSE_MOINS         PORTEbits.RE1
#define     BATTERIE_PLUS         PORTEbits.RE2
#define     BATTERIE_MOINS        PORTEbits.RE3
#define     FREIN_A_MAIN          PORTEbits.RE4
//#define     RE5         LATEbits.LATE5
//#define     RE6         LATEbits.LATE6
//#define     RE7         LATEbits.LATE7

//PORTF
//#define     RF1         LATEbits.LATF1
//#define     RF2         LATEbits.LATF2
//#define     RF3         LATEbits.LATF3
//#define     RF4         LATEbits.LATF4
//#define     RF5         LATEbits.LATF5
//#define     RF6         LATEbits.LATF6
//#define     RF7         LATEbits.LATF7

//PORTG
#define     LED_B       LATGbits.LATG0
//#define     RG1         LATGbits.LATG1
//#define     RG2         LATGbits.LATG2
#define     DRIVEA      LATGbits.LATG3
#define     DRIVEB      LATGbits.LATG4

//PORTH
// Bus de donnée afficheur GLCD

//PORTJ
#define     GLCD_CD     LATJbits.LATJ0
#define     GLCD_RD     LATJbits.LATJ1
#define     GLCD_WR     LATJbits.LATJ2
#define     GLCD_CE     LATJbits.LATJ3
#define     GLCD_RES    LATJbits.LATJ4
#define     GLCD_MD     LATJbits.LATJ5
#define     GLCD_FS     LATJbits.LATJ6
#define     BUZZER      LATJbits.LATJ7

// Definition des canaux analogiques

#define     TOUCHPAD_X          0
#define     TOUCHPAD_Y          1
#define     TEMPERATURE_EAU     2
#define     TEMPERATURE_HUILE   3
#define     JOYSTICK_X          10
#define     JOYSTICK_Y          11


// Prototypes
unsigned char lecture_8bit_analogique(unsigned char channel);


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

