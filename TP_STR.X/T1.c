#include "T1.h"
void attendre(unsigned int temps)
{
    unsigned int i, j;
    for (i = 0; i < temps; i++)
        for (j = 0; j < 255; j++)
            ; // petite boucle vide pour temporisation
}

void tache1(void)
{
    unsigned char i;

    di();
    initialisation_afficheur();
    clear_text();
    clear_graphics();
    init_rxtx();
    Init(SEM_RXTX);
    RXTX_libre=1;
    TXREG1='R';
    ei();

    LED_R=0; LED_G=0; LED_B=0;

    vitesse=0;
    batterie=120;
    n_octet_badge=0;

    goto_lico(13,34); draw_char('R'); draw_char(' '); draw_char('V'); draw_char(' '); draw_char('B');
    goto_lico(14,34); draw_char('0'); draw_char(' '); draw_char('0'); draw_char(' '); draw_char('0');
    goto_lico(15,34); draw_char('1'); draw_char(' '); draw_char('1'); draw_char(' '); draw_char('1');

    TP_appui=0;

    while(1)
    {
        /** ========================================================
         *  ÉTAPE 1 : Attente d’une carte valide
         * ======================================================== */
        if (n_octet_badge == 0)
        {
            clear_graphics();
            goto_lico(0,0);
            draw_string((unsigned char*)"Inserez carte:");
            LED_R = 0; LED_G = 0; LED_B = 1; // Bleu = attente
            continue; // retourne au début de la boucle
        }

        // Vérifie que la carte est "valide" (contient au moins 1 et que badge[0] == 1)
        if (!(n_octet_badge == 1 && badge[0] == 1))
        {
            clear_graphics();
            goto_lico(0,0);
            draw_string((unsigned char*)"CARTE REFUSEE !");
            LED_R = 0; LED_G = 1; LED_B = 1; // Rouge = refus
            attendre(300); // pause courte
            n_octet_badge = 0; // Réinitialise pour redemander une carte
            continue;
        }

        /** ========================================================
         *  ÉTAPE 2 : Carte valide → Démarrage affichage principal
         * ======================================================== */

        LED_R = 1; LED_G = 0; LED_B = 1; // Vert = autorisé

        goto_lico(0,0);
        draw_string("Marche:");
        if (MARCHE_AVANT == 0)
            draw_string("AV");
        else if (MARCHE_ARRIERE == 0)
            draw_string("AR");
        else
            draw_string("N ");

        goto_lico(1,0);
        draw_string("Siege:");
        if (SIEGE == 0)
            draw_char('1');
        else
            draw_char('0');

        goto_lico(2,0);
        draw_string("Temp. Eau:");
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU));

        goto_lico(3,0);
        draw_string("Temp. Huile:");
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE));

        /**!
         * Gere le choc
         */
        goto_lico(4,0);
        draw_string((unsigned char*)"Choc:");

        if (CHOC == 0) {
            draw_char('1');
            static unsigned char blink_div = 0;
            static unsigned char blink_state = 0;

            if (++blink_div >= 20) {
                blink_div = 0;
                blink_state ^= 1;
            }

            if (blink_state) {
                LED_R = 0; LED_G = 1; LED_B = 1;
                goto_lico(7,0);
                draw_string((unsigned char*)"!!! ALERTE CHOC !!!");
            } else {
                LED_R = 1; LED_G = 0; LED_B = 1;
                goto_lico(7,0);
                draw_string((unsigned char*)"                   ");
            }

        } else {
            draw_char('0');
            LED_R = 1; LED_G = 0; LED_B = 1;
            goto_lico(7,0);
            draw_string((unsigned char*)"                   ");
        }

        /**!
         * Controle de vitesse
         */
        goto_lico(5,0);
        draw_string((unsigned char*)"Vitesse:");
        if (VITESSE_PLUS == 0 && vitesse < 255)
            vitesse++;
        if (VITESSE_MOINS == 0 && vitesse > 0)
            vitesse--;
        draw_hex8(vitesse);

        if (vitesse > 30) {
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(6,0);
            draw_string((unsigned char*)"ALERTE: >30 km/h");
        } else {
            LED_R = 1; LED_G = 0; LED_B = 1;
            goto_lico(6,0);
            draw_string((unsigned char*)"                 ");
        }

        goto_lico(7,0);
        draw_string("Batterie:");
        if (BATTERIE_PLUS == 0)
            batterie++;
        if (BATTERIE_MOINS == 0)
            batterie--;
        draw_hex8(batterie);

        goto_lico(8,0);
        if (FREIN_A_MAIN == 0)
            draw_string("((!))");
        else
            draw_string("     ");

        goto_lico(9,0);
        draw_string("Badge:");
        if (n_octet_badge == 0)
            draw_string(" AUCUN              ");
        else {
            for (i = 0; i < n_octet_badge; i++)
                draw_hex8(badge[i]);
        }

        goto_lico(10,0);
        draw_string("X-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(11,0);
        draw_string("Y-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        /**!
         * Touchpad affichage
         */
        if (TP_appui == 1) {
            goto_lico(0,20);
            draw_string("x=");
            draw_dec8(TP_x);
            draw_string(" y=");
            draw_dec8(TP_y);
            plot1(TP_x, TP_y);
        }
        else {
            Nop();
        }
    }
}
