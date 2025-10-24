/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Lecture continue de CHOC / VITESSE / BADGE
 * @version 5.0
 */

#include "T4.h"
#include <stdio.h>

void tache4(void)
{
    char buffer[128];
    unsigned char choc_precedent = 0;

    while (1)
    {
        // --- Lecture continue de la vitesse et du badge ---
        sprintf(buffer, "choc %d  vitesse: %u km/h\r\n", CHOC, vitesse);
        rxtx_send_string(buffer);

        // --- Détection d’un appui sur le bouton de choc ---
        if (CHOC == 1 && choc_precedent == 0)
        {
            rxtx_send_string("--------------------------------------------------------\r\n");
            sprintf(buffer, "CHOC 1  vitesse: %u km/h  badge: ", vitesse);
            rxtx_send_string(buffer);

            if (n_octet_badge > 0)
            {
                for (unsigned char i = 0; i < n_octet_badge; i++)
                {
                    sprintf(buffer, "%02X", badge[i]);
                    rxtx_send_string(buffer);
                }
            }
            else
            {
                rxtx_send_string("AUCUN");
            }

            rxtx_send_string("\r\n--------------------------------------------------------\r\n");

            choc_precedent = 1; // éviter répétition pendant appui
        }

        // --- Réinitialisation dès que le bouton est relâché ---
        if (CHOC == 0 && choc_precedent == 1)
        {
            choc_precedent = 0;
        }

        // Aucun delay bloquant, simple rafraîchissement régulier
        Nop(); Nop(); Nop();
    }
}
