// /**!
//  * @file T5.c
//  * @author DBIBIH Oussama
//  * @brief Tache 5 : Envoi d'un message sur le port serie
//  * @version 1.0
//  */
#include "T5.h"
#include <stdlib.h>
#include <stdio.h>

// static const char message_tache5[] = "tache5_en_cours";

// void tache5(void)
// {
//     unsigned int a;
//     while(1)
//     {
//         rxtx_send_string(message_tache5);
//         for (a = 0; a < 65000; a++)
//             ;
//         for (a = 0; a < 65000; a++)
//             ;
//     }
// }

#include "T5.h"

/**
 * @brief Tâche 5 - Transmission périodique de la vitesse du véhicule.
 */
void tache5(void)
{
    unsigned int a;
    char buffer[32];

    while (1)
    {
        sprintf(buffer, "VITESSE=%u\r\n", vitesse);
        rxtx_send_string(buffer);

        for (a=0; a<65000; a++); // délai d’envoi régulier
    }
}
