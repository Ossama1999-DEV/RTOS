#include "systeme.h"


//fptr pointe sur la fonction choisie
//val_tos reçoit l'adresse de la fonction
//puc pointe sur l'octet poid faible
//TOS est mis à jour octet par octet

#define TOS_EGAL_FUNC(fonction) fptr=&fonction;\
                        val_tos=(unsigned short int) fptr;\
                        puc=(unsigned char *)&val_tos;\
                        TOSL=*puc;puc++;\
                        TOSH=*puc;puc++;\
                        TOSU=*puc


void __interrupt(high_priority) fonction_d_interruption(void)
{
// Sauvegarde de registres sensibles (ils sont modifiés au cours du changement de tache)
    STATUS_TEMPORAIRE=STATUS; W_TEMPORAIRE=WREG; BSR_TEMPORAIRE=BSR;
    FSR0H_TEMPORAIRE=FSR0H; FSR0L_TEMPORAIRE=FSR0L;

// INTERRUPTION TIMER0 UITLISEE PAR L'ORDONNANCEUR
    if(T0IE && T0IF)
    {
        if (DEMARRAGE==1)
        {
            STKPTR=27;// Au démarrage du système on initialise STKPTR
            DEMARRAGE=0;
        }

// sauvegarde du contexte de la tache active
// FSR0H permet de selectionner la banque qui correspond au contexte de la tache
        FSR0H=tache_active;
        FSR0L=0;
        POSTINC0=W_TEMPORAIRE; POSTINC0=STATUS_TEMPORAIRE; POSTINC0=BSR_TEMPORAIRE; POSTINC0=STKPTR;
        POSTINC0=FSR0H_TEMPORAIRE; POSTINC0=FSR0L_TEMPORAIRE; POSTINC0=FSR1H; POSTINC0=FSR1L;
        POSTINC0=FSR2H; POSTINC0=FSR2L; POSTINC0=PRODH; POSTINC0=PRODL;
        POSTINC0=TBLPTRU; POSTINC0=TBLPTRH; POSTINC0=TBLPTRL; POSTINC0=TABLAT;
        POSTINC0=PCLATU; POSTINC0=PCLATH;
// Fin de sauvegarde des registres (18 registres)
// Debut de sauvegarde des zones utilisées par le compilateur
// FSR1 va servir à balayer la zone 0x00 à 0x2F
        FSR1H=0;
        FSR1L=0;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;

        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;

        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
        POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;POSTINC0=POSTINC1;
// Fin de sauvegarde des zones utilisées par le compilateur

        TMR0=(0xFFFF-30000);// l'IT se redéclenchera dans 10ms
        T0IF=0;   // effacement du flag pour attendre la prochaine it

        Tick_Count++;// Incrémentation du compteur de tick

        pointeur_de_tache++;                        //
        if (pointeur_de_tache==NOMBRE_DE_TACHES)    // Evolution du cycle des taches
            pointeur_de_tache=0;                    // 1-2-3-4-5-6-1-2-3...
        tache_active=queue[pointeur_de_tache];      //

 // Restauration du contexte de la tache active
// Debut de restauration des zones utilisées par le compilateur
// FSR1 va servir à balayer la zone 0x00 à 0x2F
        FSR1H=0;
        FSR1L=0;
        FSR0H=tache_active;
        FSR0L=18;// acces à la zone compilateur de la tache à restaurer
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;

        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;

        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
        POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;POSTINC1=POSTINC0;
// Fin de sauvegarde des zones utilisées par le compilateur

//accès à la zone du contexte (partie registres) de la tache
        FSR0H=tache_active;
        FSR0L=0;

        W_TEMPORAIRE=POSTINC0; STATUS_TEMPORAIRE=POSTINC0; BSR_TEMPORAIRE=POSTINC0; STKPTR=POSTINC0;
        FSR0H_TEMPORAIRE=POSTINC0; FSR0L_TEMPORAIRE=POSTINC0; FSR1H=POSTINC0; FSR1L=POSTINC0;
        FSR2H=POSTINC0; FSR2L=POSTINC0; PRODH=POSTINC0; PRODL=POSTINC0;
        TBLPTRU=POSTINC0; TBLPTRH=POSTINC0; TBLPTRL=POSTINC0; TABLAT=POSTINC0;
        PCLATU=POSTINC0; PCLATH=POSTINC0;
// Fin de restauration du contexte (18 registres + 48 octet compilateur)
        //Restitution des registres courants
        FSR0L=FSR0L_TEMPORAIRE; FSR0H=FSR0H_TEMPORAIRE; BSR=BSR_TEMPORAIRE;
        WREG=W_TEMPORAIRE; STATUS=STATUS_TEMPORAIRE;

        asm("RETFIE");// Le compilateur ne sait pas que c'est une fonction IT
        //on est obligé de mettre manuellement RETFIE
        // RETFIE 0 => On ne restitue pas les registers shadow W,STATUS,BSR
        // puisque ceux qui ont été sauvés sont ceux de la tache précédente
    }

//Restitution des registres courants
    FSR0L=FSR0L_TEMPORAIRE; FSR0H=FSR0H_TEMPORAIRE; BSR=BSR_TEMPORAIRE;
    WREG=W_TEMPORAIRE; STATUS=STATUS_TEMPORAIRE;

    asm("RETFIE");// Le compilateur ne sait pas que c'est une fonction IT
    //on est obligé de mettre manuellement RETFIE
    // RETFIE 0 => On ne restitue pas les registers shadow W,STATUS,BSR
    // puisque ceux qui ont été sauvés sont ceux de la tache précédente
    tache1();//
    tache2();//
    tache3();// Les taches sont lancées ici pour qu'elles soient prises
    tache4();// en compte, car comme elles ne sont appelées nulle part
    tache5();// ailleur, le compilateur les aurait ignorées.
    tache6();//
}

void initialisation_du_systeme(void)
{
    unsigned char temp;

    DEMARRAGE=1;

    /* Ordre de départ des taches */
    queue[0]=TACHE1;
    queue[1]=TACHE2;
    queue[2]=TACHE3;
    queue[3]=TACHE4;
    queue[4]=TACHE5;
    queue[5]=TACHE6;

    pointeur_de_tache=NOMBRE_DE_TACHES-1;
    tache_active=queue[pointeur_de_tache];
    // démarrage en tache 6
    // la première tache exécutée sera la tache 1

    /* Création de la pile associée au taches */
    temp=STKPTR;

    // La pile est initialisée avec l'adresse de début de chaque tâche

    STKPTR=2;TOS_EGAL_FUNC(tache1);
    STKPTR=7;TOS_EGAL_FUNC(tache2);
    STKPTR=12;TOS_EGAL_FUNC(tache3);
    STKPTR=17;TOS_EGAL_FUNC(tache4);
    STKPTR=22;TOS_EGAL_FUNC(tache5);
    STKPTR=27;TOS_EGAL_FUNC(tache6);

    STKPTR=temp;

    //initialisation du pointeur de pile de chaque contexte
    STKPTR_T1=2;
    STKPTR_T2=7;
    STKPTR_T3=12;
    STKPTR_T4=17;
    STKPTR_T5=22;
    STKPTR_T6=27;

    /* Configuration du timer0 associé à l'ordonanceur */
    Tick_Count=0;
    //T0CON=0x08;//16 bit, no prescaler, OFF, 5.46ms period
    T0CON=0x01;//16 bit, 1:4 Prescaler, 22ms period
    T0IE=1; // Autorisation IT Ordonnanceur
}