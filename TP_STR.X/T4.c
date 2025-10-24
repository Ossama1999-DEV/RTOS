/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Détection du choc avec protection du port série
 * @version 8.0
 */

#include "T4.h"
#include <stdio.h>

/**
 * @brief Envoie une chaîne caractère par caractère sur TX1
 * protégée par le sémaphore SEM_RXTX
 */
static void send_string_uart(const char *s)
{
    P(SEM_RXTX);
    while (RXTX_libre == 0);
    RXTX_libre = 0;

    while (*s)
    {
        while (PIR1bits.TX1IF == 0);
        TXREG1 = *s++;
        while (TXSTA1bits.TRMT == 0);
    }

    RXTX_libre = 1;
    V(SEM_RXTX);
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

            choc_precedent = 1;
        }

        // --- Réinitialisation quand le bouton est relâché ---
        if (CHOC == 0 && choc_precedent == 1)
        {
            choc_precedent = 0;
        }

        // Laisse tourner librement, sans blocage
        Nop(); Nop();
    }
}
