/* 
 * File:   systeme.h
 * Author: Franck
 *
 * Created on 29 janvier 2015, 09:47
 */

#ifndef SYSTEME_H
#define	SYSTEME_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"



// Prototypes
    void __interrupt(high_priority) fonction_d_interruption(void);
    void initialisation_du_systeme(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEME_H */

