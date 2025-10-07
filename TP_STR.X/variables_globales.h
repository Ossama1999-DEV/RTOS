/* 
 * File:   variables_globales.h
 * Author: Franck
 *
 * Created on 29 janvier 2015, 09:59
 */

#ifndef VARIABLES_GLOBALES_H
#define	VARIABLES_GLOBALES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"
#include "systeme.h"

            /* Constantes */
#define     NOMBRE_DE_TACHES        6
#define     TACHE1                  1
#define     TACHE2                  2
#define     TACHE3                  3
#define     TACHE4                  4
#define     TACHE5                  5
#define     TACHE6                  6

/* Variables globales */
    // On stock dans la bank0 on laisse la zone access ram (00-5F) pour le compilateur
unsigned char W_TEMPORAIRE         __at(0x60);
unsigned char STATUS_TEMPORAIRE    __at(0x61);
unsigned char BSR_TEMPORAIRE       __at(0x62);
unsigned char FSR0H_TEMPORAIRE     __at(0x63);
unsigned char FSR0L_TEMPORAIRE     __at(0x64);
unsigned char DEMARRAGE            __at(0x65);

//Variables touch pad
unsigned char TP_appui             __at(0x66);
unsigned char TP_x                 __at(0x67);
unsigned char TP_y                 __at(0x68);

unsigned char queue[NOMBRE_DE_TACHES]        __at(0x69);
unsigned char tache_active         __at(0x6F);
unsigned char pointeur_de_tache    __at(0x70);
unsigned int Tick_Count            __at(0x71);
unsigned char vitesse              __at(0x73);
unsigned char batterie             __at(0x74);

unsigned char badge[10]            __at(0x75);
unsigned char n_octet_badge        __at(0x7F);

unsigned char RXTX_libre        __at(0x80);

    //pointeur de fonction
        //il va servir a modifier la pile pour executer les taches
void (*fptr)(void);
unsigned short int val_tos;
unsigned char * puc;
unsigned char tc[3];


//Contexte = 18 registres SFR + 48 octet de 00h à 2Fh = 66 octets
/* Réservation de la zone de 100h à 142h pour le contexte de la tache 1 */
/* Réservation de la zone de 200h à 242h pour le contexte de la tache 2 */
/* Ainsi de suite ...*/
/* Chaque contexte va utiliser 66 octets (0x42) au début de la banque correspondante */
unsigned char contexte1[66] __at(0x100);
unsigned char contexte2[66] __at(0x200);
unsigned char contexte3[66] __at(0x300);
unsigned char contexte4[66] __at(0x400);
unsigned char contexte5[66] __at(0x500);
unsigned char contexte6[66] __at(0x600);


unsigned char STKPTR_T1             __at(0x103);
unsigned char STKPTR_T2             __at(0x203);
unsigned char STKPTR_T3             __at(0x303);
unsigned char STKPTR_T4             __at(0x403);
unsigned char STKPTR_T5             __at(0x503);
unsigned char STKPTR_T6             __at(0x603);





#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_GLOBALES_H */

