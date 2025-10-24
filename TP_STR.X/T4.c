/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Détection du choc et affichage unique des infos
 * @version 7.0
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
        while (PIR1bits.TX1IF == 0);  // attente que le buffer soit libre
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
        // --- Détection du front montant (nouveau choc détecté) ---
        if (CHOC == 1 && choc_precedent == 0)
        {
            send_string_uart("***************************************\r\n");
            send_string_uart("choc detecter\r\n");

            sprintf(buffer, "vitesse: %u km/h\r\n", vitesse);
            send_string_uart(buffer);

            send_string_uart("badge: ");
            if (n_octet_badge > 0)
            {
                for (unsigned char i = 0; i < n_octet_badge; i++)
                {
                    sprintf(buffer, "%02X", badge[i]);
                    send_string_uart(buffer);
                }
                send_string_uart("\r\n");
            }
            else
            {
                send_string_uart("AUCUN\r\n");
            }

            send_string_uart("***************************************\r\n");

            choc_precedent = 1; // empêche répétition tant que le bouton est maintenu
        }

        // --- Réinitialisation à la relâche du bouton ---
        if (CHOC == 0 && choc_precedent == 1)
        {
            choc_precedent = 0;
        }

        // Boucle non bloquante, ultra légère
        Nop(); Nop();
    }
}
