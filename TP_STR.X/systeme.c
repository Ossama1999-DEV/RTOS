/**
 * @file systeme.c
 * @brief Gesion du systeme d ordonnancement cooperative
 * @date 2024-15
 * @Ossama1999-DEV
 */
#include "systeme.h"

// fptr pointe vers la fonction choisie
// val_tos recoit l adresse de la fonction
// puc pointe sur l octet de poids faible
// TOS (Top Of Stack) est mis a jour octet par octet pour initialiser la pile
#define TOS_EGAL_FUNC(fonction)  \
    fptr = &fonction;            \
    val_tos = (unsigned short int) fptr; \
    puc = (unsigned char *)&val_tos;     \
    TOSL = *puc; puc++;          \
    TOSH = *puc; puc++;          \
    TOSU = *puc


void __interrupt(high_priority) fonction_d_interruption(void)
{
    // Sauvegarde des registres sensibles (ils seront modifies lors du changement de tache)
    STATUS_TEMPORAIRE = STATUS;
    W_TEMPORAIRE      = WREG;
    BSR_TEMPORAIRE    = BSR;
    FSR0H_TEMPORAIRE  = FSR0H;
    FSR0L_TEMPORAIRE  = FSR0L;

    // INTERRUPTION TIMER0 UTILISEE PAR L ORDONNANCEUR
    if (T0IE && T0IF)
    {
        // Initialisation du pointeur de pile materielle au tout premier passage
        if (DEMARRAGE == 1)
        {
            STKPTR   = 27; // Au demarrage du systeme on positionne STKPTR sur la pile la plus haute
            DEMARRAGE = 0;
        }

        // -------------------------------------------------------------
        // SAUVEGARDE DU CONTEXTE DE LA TACHE ACTIVE
        // -------------------------------------------------------------
        // FSR0H selectionne la banque de RAM associee au contexte de la tache
        FSR0H = tache_active;
        FSR0L = 0;

        // Sauvegarde des registres CPU de la tache (zone contexte)
        POSTINC0 = W_TEMPORAIRE;
        POSTINC0 = STATUS_TEMPORAIRE;
        POSTINC0 = BSR_TEMPORAIRE;
        POSTINC0 = STKPTR;

        POSTINC0 = FSR0H_TEMPORAIRE;
        POSTINC0 = FSR0L_TEMPORAIRE;
        POSTINC0 = FSR1H;
        POSTINC0 = FSR1L;

        POSTINC0 = FSR2H;
        POSTINC0 = FSR2L;
        POSTINC0 = PRODH;
        POSTINC0 = PRODL;

        POSTINC0 = TBLPTRU;
        POSTINC0 = TBLPTRH;
        POSTINC0 = TBLPTRL;
        POSTINC0 = TABLAT;

        POSTINC0 = PCLATU;
        POSTINC0 = PCLATH;
        // Fin de sauvegarde des registres (18 registres)

        // -------------------------------------------------------------
        // SAUVEGARDE DES ZONES UTILISEES PAR LE COMPILATEUR
        // -------------------------------------------------------------
        // FSR1 va balayer la zone 0x00 a 0x2F dans la banque 0
        FSR1H = 0;
        FSR1L = 0;

        // Copie 48 octets de la zone "compilateur" vers le contexte de la tache
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;

        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;

        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1; POSTINC0 = POSTINC1;
        // Fin de sauvegarde de la zone "compilateur"

        // Rechargement du Timer0 pour generer une IT dans environ 10 ms
        TMR0 = (0xFFFF - 30000);
        T0IF = 0;   // Effacement du flag d interruption Timer0

        // Incrementation du compteur de ticks temps reel
        Tick_Count++;

        // -------------------------------------------------------------
        // CHANGEMENT DE TACHE (ROUND ROBIN SUR QUEUE)
        // -------------------------------------------------------------
        pointeur_de_tache++;
        if (pointeur_de_tache == NOMBRE_DE_TACHES)
            pointeur_de_tache = 0;   // Repart sur la premiere tache

        tache_active = queue[pointeur_de_tache];  // Selection de la nouvelle tache active

        // -------------------------------------------------------------
        // RESTAURATION DU CONTEXTE DE LA NOUVELLE TACHE ACTIVE
        // -------------------------------------------------------------

        // Debut de restauration des zones utilisees par le compilateur
        FSR1H = 0;
        FSR1L = 0;

        FSR0H = tache_active;
        FSR0L = 18;   // Acces a la zone "compilateur" dans le contexte de la tache

        // Copie 48 octets du contexte de la tache vers la zone "compilateur"
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;

        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;

        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0; POSTINC1 = POSTINC0;
        // Fin de restauration de la zone "compilateur"

        // Acces a la zone de contexte (partie registres) de la tache
        FSR0H = tache_active;
        FSR0L = 0;

        // Restauration des registres CPU a partir du contexte de la tache
        W_TEMPORAIRE      = POSTINC0;
        STATUS_TEMPORAIRE = POSTINC0;
        BSR_TEMPORAIRE    = POSTINC0;
        STKPTR            = POSTINC0;

        FSR0H_TEMPORAIRE  = POSTINC0;
        FSR0L_TEMPORAIRE  = POSTINC0;
        FSR1H             = POSTINC0;
        FSR1L             = POSTINC0;

        FSR2H             = POSTINC0;
        FSR2L             = POSTINC0;
        PRODH             = POSTINC0;
        PRODL             = POSTINC0;

        TBLPTRU           = POSTINC0;
        TBLPTRH           = POSTINC0;
        TBLPTRL           = POSTINC0;
        TABLAT            = POSTINC0;

        PCLATU            = POSTINC0;
        PCLATH            = POSTINC0;

        // Fin de restauration du contexte (18 registres + 48 octets compilateur)

        // Restitution des registres de travail
        FSR0L   = FSR0L_TEMPORAIRE;
        FSR0H   = FSR0H_TEMPORAIRE;
        BSR     = BSR_TEMPORAIRE;
        WREG    = W_TEMPORAIRE;
        STATUS  = STATUS_TEMPORAIRE;

        // Retour de l interruption avec restauration de la pile materielle
        asm("RETFIE"); // RETFIE 0: ne restaure pas les registres shadow W, STATUS, BSR
                       // car ceux sauvegardes appartiennent a la tache precedente
    }

    // Si nous arrivons ici, ce n est pas une IT Timer0
    // On restitue tout de meme les registres avant de quitter l IT
    FSR0L   = FSR0L_TEMPORAIRE;
    FSR0H   = FSR0H_TEMPORAIRE;
    BSR     = BSR_TEMPORAIRE;
    WREG    = W_TEMPORAIRE;
    STATUS  = STATUS_TEMPORAIRE;

    asm("RETFIE"); // Retour d interruption pour les autres sources

    // Ces appels de taches ne seront jamais executes a l execution
    // Ils sont presents uniquement pour forcer le compilateur / linker
    // a ne pas supprimer les fonctions tache1..tache6 comme code mort.
    tache1();
    tache2();
    tache3();
    tache4();
    tache5();
    tache6();
}


void initialisation_du_systeme(void)
{
    unsigned char temp;

    // Indique au systeme que le prochain passage dans l IT Timer0
    // correspondra au demarrage (initialisation de la pile)
    DEMARRAGE = 1;

    /* Ordre de depart des taches dans la file d ordonnancement */
    queue[0] = TACHE1;
    queue[1] = TACHE2;
    queue[2] = TACHE3;
    queue[3] = TACHE4;
    queue[4] = TACHE5;
    queue[5] = TACHE6;

    // Le pointeur de tache est positionne sur la derniere entree
    pointeur_de_tache = NOMBRE_DE_TACHES - 1;
    tache_active      = queue[pointeur_de_tache];
    // Demarrage en tache 6, la premiere tache executee sera la tache 1
    // car l IT d ordonnancement fera avancer le pointeur

    /* Creation et initialisation de la pile pour chaque tache */
    temp   = STKPTR;  // Sauvegarde du STKPTR courant

    // La pile materielle est initialisee avec l adresse de debut de chaque tache
    STKPTR = 2;  TOS_EGAL_FUNC(tache1);
    STKPTR = 7;  TOS_EGAL_FUNC(tache2);
    STKPTR = 12; TOS_EGAL_FUNC(tache3);
    STKPTR = 17; TOS_EGAL_FUNC(tache4);
    STKPTR = 22; TOS_EGAL_FUNC(tache5);
    STKPTR = 27; TOS_EGAL_FUNC(tache6);

    // Restauration de la valeur initiale du pointeur de pile
    STKPTR = temp;

    // Initialisation du pointeur de pile sauve pour chaque contexte de tache
    STKPTR_T1 = 2;
    STKPTR_T2 = 7;
    STKPTR_T3 = 12;
    STKPTR_T4 = 17;
    STKPTR_T5 = 22;
    STKPTR_T6 = 27;

    /* Configuration du Timer0 associe a l ordonnanceur */
    Tick_Count = 0;
    //T0CON=0x08; // Exemple: 16 bit, pas de prescaler, environ 5.46 ms
    T0CON = 0x01; // 16 bit, prescaler 1:4, periode d environ 22 ms
    T0IE  = 1;    // Autorisation de l IT Timer0 pour l ordonnanceur
}
