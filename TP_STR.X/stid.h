/* 
 * File:   stid.h
 * Author: Franck
 *
 * Created on 10 mars 2015, 09:56
 */

#ifndef STID_H
#define	STID_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "main.h"

unsigned char lecture_normale(unsigned char * stid_id);
void stid_delai_us(unsigned int isdu);

#ifdef	__cplusplus
}
#endif

#endif	/* STID_H */

