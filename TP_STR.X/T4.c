/**!
 * \file T4.c
 * \brief Tâche 4 : Supervision du chariot (affichage global + alerte vitesse)
 * \author DBIBIH Oussama
 */

#include "T4.h"
#include "T1.h"          // Pour accéder aux données globales de la T1
#include "afficheur.h"   // Fonctions graphiques (draw_string, clear_graphics, goto_lico)
#include "main.h"

// === Variables globales externes provenant de T1 ===
extern unsigned char vitesse;
extern unsigned char temp_eau;
extern unsigned char temp_huile;
extern unsigned char joystick_x;
extern unsigned char joystick_y;

void tache4(void)
{
    unsigned int i;

    while (1)
    {
        // === Étape 1 : Nettoyage + en-tête clair ===
        clear_graphics();
        goto_lico(0, 0);
        draw_string("=== SUPERVISION TACHE 4 ===");

        // === Étape 2 : Envoi série du message de statut ===
        const char *msg = "TACHE4_ENCOURS\r\n";
        const char *p = msg;
        while (*p)
        {
            while (PIR1bits.TX1IF == 0);
            TXREG1 = *p++;
            while (TXSTA1bits.TRMT == 0);
        }

        // === Étape 3 : Affichage des données de la T1 ===
        goto_lico(2, 0);
        draw_string("Parametres Chariot :");

        goto_lico(3, 0);
        draw_string("Vitesse : ");
        draw_hex8(vitesse);
        draw_string(" km/h");

        goto_lico(4, 0);
        draw_string("Temp. Eau : ");
        draw_hex8(temp_eau);
        draw_string(" C");

        goto_lico(5, 0);
        draw_string("Temp. Huile : ");
        draw_hex8(temp_huile);
        draw_string(" C");

        goto_lico(6, 0);
        draw_string("Joystick X=");
        draw_hex8(joystick_x);
        draw_string(" Y=");
        draw_hex8(joystick_y);

        // === Étape 4 : Alerte vitesse (affichage rouge si > 30 km/h) ===
        if (vitesse > 30)
        {
            LED_R = 0;   // Rouge ON
            LED_G = 1;
            LED_B = 1;

            goto_lico(8, 0);
            draw_string("!!! DANGER: VITESSE > 30 KM/H !!!");
            goto_lico(9, 0);
            draw_string("⚠️  RALENTIR IMMEDIATEMENT ⚠️");

            // Si ta lib le supporte :
            // set_color(255, 0, 0);
            // clear_graphics();
        }
        else
        {
            LED_R = 1;
            LED_G = 0;   // Vert ON
            LED_B = 1;

            goto_lico(8, 0);
            draw_string("Vitesse normale (<30 km/h)");
        }

        // === Étape 5 : Ligne de séparation et boucle d’attente ===
        goto_lico(10, 0);
        draw_string("-----------------------------");

        for (i = 0; i < 65000; i++);  // Pause avant mise à jour
    }
}
