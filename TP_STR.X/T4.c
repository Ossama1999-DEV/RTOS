/**!
 * \file T4.c
 * \brief Tâche 4 : Envoi régulier du message "TACHE4_ENCOURS" sur le port série
 *        + affichage dédié sur l'écran (clear + message)
 *        + surveillance de la vitesse (> 30 km/h = alerte rouge)
 * \author DBIBIH Oussama
 */

#include "T4.h"
#include "T1.h"          // pour accéder à la variable vitesse
#include "afficheur.h"   // pour draw_string(), clear_graphics(), etc.

extern unsigned char vitesse;  // variable globale de T1

void tache4(void)
{
    unsigned int a;

    while (1)
    {
        // === Étape 1 : Nettoyage complet de l'écran ===
        clear_graphics();         // efface tout le contenu de l'afficheur
        goto_lico(0, 0);
        draw_string("=== TACHE 4 ACTIVE ===");

        // === Étape 2 : Envoi du message sur le port série ===
        while (PIR1bits.TX1IF == 0); TXREG1 = 'T'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'A'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'C'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'H'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'E'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = '4'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = '_'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'E'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'N'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = '_'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'C'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'O'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'U'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'R'; while (TXSTA1bits.TRMT == 0);
        while (PIR1bits.TX1IF == 0); TXREG1 = 'S'; while (TXSTA1bits.TRMT == 0);

        // === Étape 3 : Surveillance et affichage selon la vitesse ===
        goto_lico(2, 0);
        if (vitesse > 30)
        {
            // Mode alerte
            LED_R = 0;  // rouge ON
            LED_G = 1;
            LED_B = 1;

            draw_string("⚠️  DANGER: >30 km/h !");
            // Optionnel : colorier fond rouge si supporté
            // set_color(255, 0, 0);
        }
        else
        {
            // Mode normal
            LED_R = 1;
            LED_G = 0;  // vert ON
            LED_B = 1;

            draw_string("✅ Vitesse OK (<30 km/h)");
        }

        // === Étape 4 : Pause avant la prochaine exécution ===
        for (a = 0; a < 65000; a++);
    }
}
