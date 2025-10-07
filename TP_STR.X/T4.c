/**!
 * \file T4.c
 * \brief Tache 4 : Envoi régulier du message "TACHE4_ENCOURS" sur le port série
 *        + surveillance de la vitesse (si > 30 km/h => écran rouge)
 * \author DBIBIH Oussama
 */

#include "T4.h"
#include "T1.h"       // pour accéder à la variable vitesse (si elle est globale)
#include "afficheur.h" // pour utiliser les fonctions draw_string, set_color, etc.

extern unsigned char vitesse;  // déclaration externe de la variable vitesse

void tache4(void)
{
    unsigned int a;

    while(1)
    {
        // === Transmission série du message "TACHE4_ENCOURS" ===
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

        // === Nouvelle partie : surveillance de la vitesse ===
        if (vitesse > 30)
        {
            // Mode alerte : LED rouge et message de danger
            LED_R = 0;   // ON
            LED_G = 1;   // OFF
            LED_B = 1;   // OFF

            goto_lico(6, 0);
            draw_string("!!! DANGER: >30 km/h !!!");

            // Optionnel : colorier l’écran en rouge si la lib le permet
            // set_color(255, 0, 0);
            // clear_graphics();
        }
        else
        {
            // Mode normal : LED verte
            LED_R = 1;
            LED_G = 0;   // ON
            LED_B = 1;

            goto_lico(6, 0);
            draw_string("Vitesse OK (<30 km/h)  ");
        }

        // === Délai entre deux envois ===
        for (a=0; a<65000; a++);
    }
}
