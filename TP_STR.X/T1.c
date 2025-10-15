/**!
 * \file T1.c
 * \brief Tâche 1 : Interface utilisateur
 * \author DBIBIH Oussama
 * \version 1.0
 */
#include "T1.h"

void tache1(void)
{
    unsigned char i;

    // === Initialisation ===
    di();
    initialisation_afficheur();
    clear_text();
    clear_graphics();
    init_rxtx();
    Init(SEM_RXTX);
    RXTX_libre = 1;
    TXREG1 = 'R';
    ei();

    LED_R = 0; LED_G = 0; LED_B = 0;
    vitesse = 0;
    batterie = 120;
    n_octet_badge = 0;
    TP_appui = 0;

    // === Bande d’état LEDs ===
    goto_lico(13, 34);
    draw_char('R'); draw_char(' '); draw_char('V'); draw_char(' '); draw_char('B');
    goto_lico(14, 34);
    draw_char('0'); draw_char(' '); draw_char('0'); draw_char(' '); draw_char('0');
    goto_lico(15, 34);
    draw_char('1'); draw_char(' '); draw_char('1'); draw_char(' '); draw_char('1');

    while (1)
    {
        // ==========================
        //   SECTION 1 : ETAT MOTEUR
        // ==========================
        goto_lico(0, 0);
        draw_string((unsigned char*)"=== ETAT VEHICULE ===");

        goto_lico(1, 0);
        draw_string((unsigned char*)"Marche: ");
        if (MARCHE_AVANT == 0)
        {
            draw_string((unsigned char*)"AV");
            LED_R = 1; LED_G = 0; LED_B = 1; // vert
        }
        else if (MARCHE_ARRIERE == 0)
        {
            draw_string((unsigned char*)"AR");
            LED_R = 0; LED_G = 0; LED_B = 1; // jaune
        }
        else
        {
            draw_string((unsigned char*)"N ");
            LED_R = 1; LED_G = 1; LED_B = 1; // neutre
        }

        goto_lico(2, 0);
        draw_string((unsigned char*)"Siege: ");
        draw_char(SIEGE == 0 ? '1' : '0');

        goto_lico(3, 0);
        draw_string((unsigned char*)"Frein: ");
        if (FREIN_A_MAIN == 0)
        {
            draw_string((unsigned char*)"((!))");
            LED_R = 0; LED_G = 1; LED_B = 1; // rouge
            goto_lico(4, 0);
            draw_string((unsigned char*)"ENLEVEZ FREIN A MAIN");
            if (MARCHE_AVANT == 0)
                vitesse = 0;
        }
        else
        {
            draw_string((unsigned char*)"OFF ");
            goto_lico(4, 0);
            draw_string((unsigned char*)"                    ");
        }

        // ==========================
        //   SECTION 2 : CAPTEURS
        // ==========================
        goto_lico(6, 0);
        draw_string((unsigned char*)"=== CAPTEURS ===");

        goto_lico(7, 0);
        draw_string((unsigned char*)"Temp. Eau:   ");
        // draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU));
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_EAU));
        

        goto_lico(8, 0);
        draw_string((unsigned char*)"Temp. Huile: ");
        // draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE));
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_HUILE));

        goto_lico(9, 0);
        draw_string((unsigned char*)"Choc: ");
        if (CHOC == 0)
        {
            draw_char('1');
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(10, 0);
            draw_string((unsigned char*)"!!! ALERTE CHOC !!!");
        }
        else
        {
            draw_char('0');
            goto_lico(10, 0);
            draw_string((unsigned char*)"                   ");
        }

        // ==========================
        //   SECTION 3 : PERFORMANCES
        // ==========================
        goto_lico(11, 0);
        draw_string((unsigned char*)"=== PERFORMANCE ===");

        goto_lico(12, 0);
        draw_string((unsigned char*)"Vitesse: ");
        if (VITESSE_PLUS == 0 && vitesse < 255)
            vitesse++;
        if (VITESSE_MOINS == 0 && vitesse > 0)
            vitesse--;
        // draw_hex8(vitesse);
        draw_dec8(vitesse);

        if (vitesse > 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(13, 0);
            draw_string((unsigned char*)"ALERTE: >30 km/h");
        }
        else
        {
            goto_lico(13, 0);
            draw_string((unsigned char*)"                 ");
        }

        goto_lico(14, 0);
        draw_string((unsigned char*)"Batterie: ");
        if (BATTERIE_PLUS == 0)
            batterie++;
        if (BATTERIE_MOINS == 0 && batterie > 0)
            batterie--;
        // draw_hex8(batterie);
        draw_dec8(batterie);

        if (batterie < 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(15, 0);
            draw_string((unsigned char*)"!!! BATTERIE FAIBLE !!!");
        }
        else
        {
            goto_lico(15, 0);
            draw_string((unsigned char*)"                       ");
        }

        // ==========================
        //   SECTION 4 : BADGE ET CONTROLE
        // ==========================
        goto_lico(17, 0);
        draw_string((unsigned char*)"=== ACCES & CONTROLE ===");

        goto_lico(18, 0);
        draw_string((unsigned char*)"Badge: ");
        if (n_octet_badge == 0)
        {
            draw_string((unsigned char*)"AUCUN");
            goto_lico(19, 0);
            draw_string((unsigned char*)"Inserez carte...");
        }
        else
        {
            goto_lico(19, 0);
            draw_string((unsigned char*)"                  ");
            for (i = 0; i < n_octet_badge; i++)
                // draw_hex8(badge[i]);
                draw_dec8(badge[i]);
        }

        // ==========================
        //   SECTION 5 : COMMANDES MANUELLES
        // ==========================
        goto_lico(21, 0);
        draw_string((unsigned char*)"=== COMMANDES ===");

        goto_lico(22, 0);
        draw_string((unsigned char*)"Joystick X: ");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(23, 0);
        draw_string((unsigned char*)"Joystick Y: ");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        // === Touch Panel ===
        if (TP_appui == 1)
        {
            goto_lico(24, 0);
            draw_string((unsigned char*)"TP: ");
            draw_string((unsigned char*)"x=");
            draw_dec8(TP_x);
            draw_string((unsigned char*)" y=");
            draw_dec8(TP_y);
            plot1(TP_x, TP_y);
        }
        else
        {
            Nop();
        }
    }
}
