/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Surveillance du choc et envoi série des informations vitesse + badge
 * @version 4.0
 */

#include "T4.h"
#include <stdio.h>

void tache4(void)
{
    unsigned int a;
    char buffer[64];
    unsigned char choc_precedent = 0;
    unsigned char vitesse_choc = 0;

    while (1)
    {
        // --- Si aucun choc ---
        if (CHOC == 0)
        {
            // affichage régulier de l’état normal
            sprintf(buffer, "choc 0  vitesse: %u km/h\r\n", vitesse);
            rxtx_send_string(buffer);

            choc_precedent = 0; // réinitialisation pour détecter le prochain choc
        }
        // --- Si un choc vient d’être détecté ---
        else if (CHOC == 1 && choc_precedent == 0)
        {
            vitesse_choc = vitesse;
            sprintf(buffer, "CHOC DETECTE !!  vitesse: %u km/h", vitesse_choc);
            rxtx_send_string(buffer);

            // ajout du numéro de badge s’il existe
            if (n_octet_badge > 0)
            {
                rxtx_send_string("  badge: ");
                for (unsigned char i = 0; i < n_octet_badge; i++)
                {
                    sprintf(buffer, "%02X", badge[i]);
                    rxtx_send_string(buffer);
                }
            }
            else
            {
                rxtx_send_string("  badge: AUCUN");
            }

            rxtx_send_string("\r\n");

            choc_precedent = 1; // évite de répéter pendant tout le temps du choc
        }

        // --- Petite temporisation légère (évite surcharge CPU) ---
        for (a = 0; a < 50000; a++);
    }
}
