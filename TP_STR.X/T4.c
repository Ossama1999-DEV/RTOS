/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Transmission caractère par caractère (CHOC / VITESSE / BADGE)
 * @version 6.0
 */

#include "T4.h"
#include <stdio.h>

/**
 * @brief Envoie une chaîne caractère par caractère sur TX1
 */
static void send_string_uart(const char *s)
{
    while (*s)
    {
        while (PIR1bits.TX1IF == 0);  // attente buffer libre
        TXREG1 = *s++;
        while (TXSTA1bits.TRMT == 0); // attente fin d’envoi
    }
}

void tache4(void)
{
    char buffer[64];
    unsigned char choc_precedent = 0;

    while (1)
    {
        // --- Affichage continu : choc + vitesse ---
        sprintf(buffer, "choc %d  vitesse: %u km/h\r\n", CHOC, vitesse);
        send_string_uart(buffer);

        // --- Détection du front montant de CHOC ---
        if (CHOC == 1 && choc_precedent == 0)
        {
            send_string_uart("--------------------------------------------------------\r\n");

            sprintf(buffer, "CHOC 1  vitesse: %u km/h  badge: ", vitesse);
            send_string_uart(buffer);

            if (n_octet_badge > 0)
            {
                for (unsigned char i = 0; i < n_octet_badge; i++)
                {
                    sprintf(buffer, "%02X", badge[i]);
                    send_string_uart(buffer);
                }
            }
            else
            {
                send_string_uart("AUCUN");
            }

            send_string_uart("\r\n--------------------------------------------------------\r\n");

            choc_precedent = 1; // évite répétition pendant l’appui
        }

        // --- Réinitialisation une fois le bouton relâché ---
        if (CHOC == 0 && choc_precedent == 1)
        {
            choc_precedent = 0;
        }

        // tâche ultra-légère, aucun délai bloquant
        Nop(); Nop();
    }
}
