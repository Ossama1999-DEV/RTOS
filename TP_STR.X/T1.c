#include "T1.h"

/**
 * @brief Tâche 1 - Affichage principal du tableau de bord.
 * @details Rafraîchit périodiquement l’état des capteurs et variables système.
 */
void tache1(void)
{
    unsigned char i;

    di(); // Désactivation IT pendant init
    initialisation_afficheur();
    clear_text();
    clear_graphics();
    init_rxtx();
    Init(SEM_RXTX);
    RXTX_libre = 1;
    TXREG1 = 'R';
    ei(); // Réactivation IT

    LED_R = LED_G = LED_B = 0;
    vitesse = 0;
    batterie = 120;
    n_octet_badge = 0;
    TP_appui = 0;

    while (1)
    {
        clear_text(); // rafraîchissement propre

        // === Lignes de statut ===
        goto_lico(0, 0);
        draw_string("MODE : ");
        if (MARCHE_AVANT == 0)      draw_string("AVANT");
        else if (MARCHE_ARRIERE==0) draw_string("ARRIERE");
        else                        draw_string("NEUTRE");

        goto_lico(1, 0);
        draw_string("SIEGE : "); draw_char(SIEGE==0 ? '1':'0');

        goto_lico(2, 0);
        draw_string("T_EAU : "); draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU));

        goto_lico(3, 0);
        draw_string("T_HUILE : "); draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE));

        goto_lico(4, 0);
        draw_string("CHOC : "); draw_char(CHOC==0 ? '1':'0');

        goto_lico(5, 0);
        draw_string("VITESSE : ");
        if (VITESSE_PLUS == 0)  vitesse++;
        if (VITESSE_MOINS == 0) vitesse--;
        draw_dec8(vitesse);

        goto_lico(6, 0);
        draw_string("BATTERIE : ");
        if (BATTERIE_PLUS == 0)  batterie++;
        if (BATTERIE_MOINS == 0) batterie--;
        draw_dec8(batterie);

        goto_lico(7, 0);
        draw_string("FREIN : ");
        if (FREIN_A_MAIN == 0) draw_string("ACTIF");
        else                   draw_string("OFF");

        goto_lico(8, 0);
        draw_string("BADGE : ");
        if (n_octet_badge == 0) draw_string("AUCUN");
        else for (i=0; i<n_octet_badge; i++) draw_hex8(badge[i]);

        goto_lico(9, 0);
        draw_string("JOYSTICK X:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(10, 0);
        draw_string("JOYSTICK Y:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        if (TP_appui == 1)
        {
            goto_lico(12, 0);
            draw_string("TOUCH x="); draw_dec8(TP_x);
            draw_string(" y="); draw_dec8(TP_y);
            plot1(TP_x, TP_y);
        }

        for (i=0; i<10; i++) Nop(); // petit délai d’affichage
    }
}
