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
    unsigned char choc_precedent;
    unsigned char choc_actuel;
    unsigned char i;

    memset(buffer, 0, sizeof(buffer));

    /* On initialise l etat precedent avec l etat courant du bouton */
    choc_precedent = CHOC;

    while (1)
    {
        /* On lit l etat courant une seule fois */
        choc_actuel = CHOC;

        /* --- Front descendant: 1 -> 0 (bouton appuye) --- */
        if (choc_actuel == 0 && choc_precedent == 1)
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
        }

        /* On memorise l etat pour le prochain tour */
        choc_precedent = choc_actuel;

        /* Petite pause (anti rebond light) */
        Nop();
        Nop();
    }
}
