/**
 * @file T4.c
 * @author DBIBIH O.
 * @brief Tâche 4 – Détection de choc : envoi de l'événement avec vitesse et badge.
 * @version 3.0
 */

#include "T4.h"
#include <stdio.h>

/**
 * @details
 * - Surveille l'entrée CHOC.
 * - Lorsqu’un choc est détecté, envoie un message unique :
 *   ➤ vitesse au moment du choc,
 *   ➤ numéro du badge actif (ou “AUCUN” si non lu).
 * - Évite le spam tant que le choc est maintenu.
 */
void tache4(void)
{
    unsigned int a;
    char buffer[64];
    unsigned char choc_detecte_precedent = 0;
    unsigned char vitesse_choc = 0;

    while (1)
    {
        // --- Détection d’un nouveau choc ---
        if (CHOC == 1 && choc_detecte_precedent == 0)
        {
            choc_detecte_precedent = 1;
            vitesse_choc = vitesse;  // capture de la vitesse exacte

            // Construction du message de choc
            sprintf(buffer, "\r\n⚠️ CHOC détecté à %u km/h", vitesse_choc);
            rxtx_send_string(buffer);

            // Ajout du numéro de badge si disponible
            if (n_octet_badge > 0)
            {
                rxtx_send_string(" | BADGE=");
                for (unsigned char i = 0; i < n_octet_badge; i++)
                {
                    sprintf(buffer, "%02X", badge[i]);
                    rxtx_send_string(buffer);
                }
            }
            else
            {
                rxtx_send_string(" | BADGE=AUCUN");
            }

            rxtx_send_string("\r\n");

            // 🔴 Clignotement LED rouge témoin du choc (optionnel mais utile)
            LED_R = 1;
            for (a = 0; a < 40000; a++);
            LED_R = 0;
        }

        // --- Réinitialisation du flag une fois le choc relâché ---
        if (CHOC == 0 && choc_detecte_precedent == 1)
        {
            choc_detecte_precedent = 0;
        }

        // --- Envoi périodique de la vitesse (optionnel pour supervision) ---
        if (vitesse > 0)
        {
            sprintf(buffer, "VITESSE=%u\r\n", vitesse);
            rxtx_send_string(buffer);
        }

        // --- Temporisation de la tâche ---
        for (a = 0; a < 60000; a++);
    }
}
