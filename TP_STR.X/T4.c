/**!
 * @file T4.c
 * @author DBIBIH Oussama
 * @brief Tache 4 : Envoi d'un message sur le port serie
 * @version 1.0
 */
#include "T4.h"

static const char message_tache4[] = "TACHE4_EN_COURS";

void tache4(void)
{
    unsigned int a;
    while(1)
    {
        rxtx_send_string(message_tache4);
        for (a = 0; a < 65000; a++)
            ;
    }
}
