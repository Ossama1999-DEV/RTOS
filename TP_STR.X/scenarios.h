#ifndef SCENARIOS_H
#define SCENARIOS_H

#include <xc.h>
#include <stdint.h>
#include "main.h"   // Contient SEM_CAN, définitions LED, etc.

// ===================================================
// === Prototypes des fonctions utilisées par le scénario ===
// ===================================================

void initialisation_du_systeme(void);
void Init(unsigned char sem);
unsigned char lecture_8bit_analogique(unsigned char channel);

// Fonctions utilitaires (affichage, attente)
void afficher_message(const char *msg);
void attendre(uint16_t ms);

// Fonctions de mouvement du chariot
void avancer_chariot(void);
void reculer_chariot(void);
void arreter_chariot(void);

// Fonctions du mât (levage)
void lever_materiel(void);
void descendre_materiel(void);
void stabiliser_hauteur(void);

// Fonctions de sécurité et de supervision
void alerter(const char *msg);
void surveiller_obstacles(void);
void gerer_vitesse(void);
void mettre_a_jour_affichage(void);

// ===================================================
// === Constantes des capteurs ===
// ===================================================
#define JOYSTICK_X          10
#define JOYSTICK_Y          11
#define CAPTEUR_HAUTEUR     2
#define CAPTEUR_TEMP_MOTEUR 3

#endif // SCENARIOS_H
