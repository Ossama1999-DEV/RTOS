/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tache 4 - Detection du choc avec protection du port serie
 * @version 8.0
 */

#include "T4.h"
#include <stdio.h>
#include <string.h>

/**
 * @brief Envoie une chaine sur TX1, protegee par RXTX_libre
 */
static void send_string_uart(const char *s)
{
    /* Protection simple du port serie */
    while (RXTX_libre == 0)
    {
        /* attente active */
    }
    RXTX_libre = 0;

    while (*s)
    {
        while (PIR1bits.TX1IF == 0) { }
        TXREG1 = *s++;
        while (TXSTA1bits.TRMT == 0) { }
    }

    RXTX_libre = 1;
}

/* Petit helper pour envoyer une ligne + fin de ligne */
static void send_line_uart(const char *s)
{
    send_string_uart(s);
    send_string_uart("\r\n");
}

void tache4(void)
{
    char buffer[64];
    unsigned char choc_precedent = 0;
    unsigned char i;

    memset(buffer, 0, sizeof(buffer));

    while (1)
    {
        /* --- Front montant: nouveau choc detecte --- */
        if (CHOC == 1 && choc_precedent == 0)
        {
            send_line_uart("");
            send_line_uart("========================================");
            send_line_uart("=          CHOC DETECTE                =");
            send_line_uart("========================================");

            /* Vitesse */
            snprintf(buffer, sizeof(buffer), "Vitesse : %3u km/h", (unsigned int)vitesse);
            send_line_uart(buffer);

            /* Badge */
            send_string_uart("Badge   : ");
            if (n_octet_badge > 0)
            {
                for (i = 0; i < n_octet_badge; i++)
                {
                    /* Espace + 2 chiffres hexa */
                    snprintf(buffer, sizeof(buffer), "%s%02X",
                             (i == 0) ? "" : " ", (unsigned int)badge[i]);
                    send_string_uart(buffer);
                }
                send_line_uart("");
            }
            else
            {
                send_line_uart("AUCUN");
            }

            send_line_uart("========================================");
            send_line_uart("");

            choc_precedent = 1;
        }

        /* --- Reinitialisation quand le bouton est relache --- */
        if (CHOC == 0 && choc_precedent == 1)
        {
            choc_precedent = 0;
        }

        /* Laisse tourner librement, sans blocage long */
        Nop();
        Nop();
    }
}
