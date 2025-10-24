// /**!
//  * @file T4.c
//  * @author DBIBIH Oussama
//  * @brief Tache 4 : Envoi d'un message sur le port serie
//  * @version 1.0
//  */
#include "T4.h"
#include <stdlib.h>
#include <stdio.h>

// static const char message_tache4[] = "TACHE4_EN_COURS";

// void tache4(void)
// {
//     unsigned int a;
//     while(1)
//     {
//         rxtx_send_string(message_tache4);
//         for (a = 0; a < 65000; a++)
//             ;
//     }
// }

#include "T4.h"

/**
 * @brief Tâche 4 - Envoi sur liaison série de l’état du capteur de choc.
 */
void tache4(void)
{
    unsigned int a;
    char buffer[32];

    while (1)
    {
        sprintf(buffer, "CHOC=%d\r\n", (CHOC==0) ? 1 : 0);
        rxtx_send_string(buffer);

        for (a=0; a<50000; a++); // temporisation
    }
}
