/**
 * \file semaphore.c
 * \brief Module de gestion de semaphores
 * \author DBIBIH Oussama
 * \version 1.0
 */
#include "semaphore.h"
/*
 * Module de gestion de semaphores
 *
 * Fonctions :
 *  - Init(sem) : initialise le semaphore et le place en etat libre.
 *  - P(sem) : operation "wait", bloque tant que la ressource est occupee,
 *             puis la reserve des qu elle devient libre.
 *  - V(sem) : operation "signal", libere la ressource.
 *
 * Comportement :
 *  - SEM_CAN  : protege l acces a l interface CAN.
 *  - SEM_RXTX : protege l acces a la liaison serie.
 *
 * Notes d implementation :
 *  - Val_sem_cna et Val_sem_rxtx sont des semaphores binaires
 *    (0 = verrouille, 1 = libre).
 *  - P() utilise une attente active (spinlock) jusqu a disponibilite.
 *  - V() libere immediatement la ressource.
 */



void Init(unsigned char sem)
{
    switch (sem)
    {
        case SEM_CAN:
            Val_sem_cna = 1;
            break;
        case SEM_RXTX:
            Val_sem_rxtx = 1;
            break;
        default:
            ;
        
    }
}

void __reentrant P(unsigned char sem)
{
    switch(sem)
    {
        case SEM_CAN:
            while (Val_sem_cna < 1) // Tant que le cna est occup�
                ;                   // On attend
            Val_sem_cna = 0;        // D�s qu'il est libre, on le r�serve
            break;
        case SEM_RXTX:
            while (Val_sem_rxtx < 1)    // Tant que la liaison s�rie est occup�
                ;                       // On attend
            Val_sem_rxtx = 0;           // D�s qu'elle est libre, on la r�serve
            break;
        default:
            ;
    }
}

void __reentrant V(unsigned char sem)
{
    switch(sem)
    {
        case SEM_CAN:
            Val_sem_cna = 1;
            break;
        case SEM_RXTX:
            Val_sem_rxtx = 1;
            break;
        default:
            ;
    }
}
