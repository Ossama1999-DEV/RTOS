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
    static unsigned char blink_div = 0;
    static unsigned char blink_state = 0;

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

    while (1)
    {
        // =============================
        //         ZONE GAUCHE
        //     (informations système)
        // =============================
        goto_lico(0, 0);
        draw_string((unsigned char*)"Temp. Eau:");
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_EAU));

        goto_lico(1, 0);
        draw_string((unsigned char*)"Temp. Huile:");
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_HUILE));

        goto_lico(2, 0);
        draw_string((unsigned char*)"Batterie:");
        if (BATTERIE_PLUS == 0) batterie++;
        if (BATTERIE_MOINS == 0 && batterie > 0) batterie--;
        draw_dec8(batterie);

        if (batterie < 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;  // Rouge
            goto_lico(10, 0);
            draw_string((unsigned char*)"!!! BATTERIE FAIBLE !!!");
        }

        goto_lico(3, 0);
        draw_string((unsigned char*)"Frein:");
        if (FREIN_A_MAIN == 0)
        {
            draw_string((unsigned char*)"((!))");
            goto_lico(11, 0);
            draw_string((unsigned char*)"ENLEVEZ FREIN A MAIN");
            vitesse = 0;
        }
        else
        {
            draw_string((unsigned char*)"OFF ");
            goto_lico(11, 0);
            draw_string((unsigned char*)"                     ");
        }

        goto_lico(4, 0);
        draw_string((unsigned char*)"Choc:");
        if (CHOC == 0)
        {
            draw_char('1');

            // gestion du clignotement rouge
            if (++blink_div >= 20)
            {
                blink_div = 0;
                blink_state ^= 1;
            }

            if (blink_state)
            {
                LED_R = 0; LED_G = 1; LED_B = 1;
                goto_lico(12, 0);
                draw_string((unsigned char*)"!!! ALERTE CHOC !!!");
            }
            else
            {
                LED_R = 1; LED_G = 0; LED_B = 1;
                goto_lico(12, 0);
                draw_string((unsigned char*)"                   ");
            }
        }
        else
        {
            draw_char('0');
            goto_lico(12, 0);
            draw_string((unsigned char*)"                   ");
        }

        // === Badge ===
        goto_lico(5, 0);
        draw_string((unsigned char*)"Badge:");
        if (n_octet_badge == 0)
        {
            draw_string((unsigned char*)"AUCUN");
            goto_lico(6, 0);
            draw_string((unsigned char*)"Inserez carte...");
        }
        else
        {
            goto_lico(6, 0);
            draw_string((unsigned char*)"                 ");
            for (i = 0; i < n_octet_badge; i++)
                draw_dec8(badge[i]);
        }

        // =============================
        //         ZONE DROITE
        //     (mouvement & action)
        // =============================

        goto_lico(0, 20);
        draw_string((unsigned char*)"Marche:");

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

        goto_lico(1, 20);
        draw_string((unsigned char*)"Vitesse:");
        if (VITESSE_PLUS == 0 && vitesse < 255)
            vitesse++;
        if (VITESSE_MOINS == 0 && vitesse > 0)
            vitesse--;
        draw_dec8(vitesse);

        if (vitesse > 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(2, 20);
            draw_string((unsigned char*)"ALERTE: >30 km/h");
        }
        else
        {
            goto_lico(2, 20);
            draw_string((unsigned char*)"                 ");
        }

        goto_lico(3, 20);
        draw_string((unsigned char*)"X-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(4, 20);
        draw_string((unsigned char*)"Y-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        if (TP_appui == 1)
        {
            goto_lico(6, 20);
            draw_string((unsigned char*)"TP: x=");
            draw_dec8(TP_x);
            draw_string((unsigned char*)" y=");
            draw_dec8(TP_y);
            plot1(TP_x, TP_y);
        }
        else
        {
            goto_lico(6, 20);
            draw_string((unsigned char*)"TP:               ");
        }
    }
}
