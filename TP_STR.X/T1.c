/**
 * @file T1.c
 * @author DBIBIH O.
 * @brief TP_STR - Tache 1
 * @version 8.0
 */
#include "T1.h"

void tache1(void)
{
    unsigned char i;

    di();
    initialisation_afficheur();
    clear_text();
    clear_graphics();
    init_rxtx();
    Init(SEM_RXTX);
    RXTX_libre = 1;
    TXREG1 = 'R';
    ei();

    LED_R = 0;
    LED_G = 0;
    LED_B = 0;

    vitesse = 0;
    batterie = 100;     // NEW : demarrage a 100 % max
    n_octet_badge = 0;

    // Petit repere RVB en bas a droite
    goto_lico(13, 34); draw_char('R'); draw_char(' '); draw_char('V'); draw_char(' '); draw_char('B');
    goto_lico(14, 34); draw_char('0'); draw_char(' '); draw_char('0'); draw_char(' '); draw_char('0');
    goto_lico(15, 34); draw_char('1'); draw_char(' '); draw_char('1'); draw_char(' '); draw_char('1');

    TP_appui = 0;

    while (1)
    {
        /* ---------------------------------------------------------
         * Ligne 0 : Marche + Badge
         * --------------------------------------------------------- */
        goto_lico(0, 0);
        draw_string((unsigned char*)"Marche: ");
        if (MARCHE_AVANT == 0)
            draw_string((unsigned char*)"AV ");
        else if (MARCHE_ARRIERE == 0)
            draw_string((unsigned char*)"AR ");
        else
            draw_string((unsigned char*)"N  ");

        goto_lico(0, 20);
        draw_string((unsigned char*)"Badge: ");
        if (n_octet_badge == 0)
        {
            draw_string((unsigned char*)"AUCUN       ");
        }
        else
        {
            for (i = 0; i < n_octet_badge; i++)
            {
                draw_hex8(badge[i]);
            }
            draw_string((unsigned char*)"   ");
        }

        /* ---------------------------------------------------------
         * Ligne 1 : Siege + Frein
         * --------------------------------------------------------- */
        goto_lico(1, 0);
        draw_string((unsigned char*)"Siege: ");
        if (SIEGE == 0)
            draw_char('1');
        else
            draw_char('0');

        draw_string((unsigned char*)"   Frein: ");
        if (FREIN_A_MAIN == 0)
            draw_string((unsigned char*)"ON ");
        else
            draw_string((unsigned char*)"OFF");

        /* ---------------------------------------------------------
         * Ligne 2 : Dessin du chariot (placeholder texte)
         * --------------------------------------------------------- */
        goto_lico(2, 10);
        draw_string((unsigned char*)"CHARIOT");

        /* ---------------------------------------------------------
         * Ligne 4 : Temp Eau et Temp Huile sur la meme ligne
         * --------------------------------------------------------- */
        goto_lico(4, 0);
        draw_string((unsigned char*)"Temp Eau: ");
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_EAU));
        draw_char('C');

        goto_lico(4, 20);
        draw_string((unsigned char*)"Temp Huile: ");
        draw_dec8(lecture_8bit_analogique(TEMPERATURE_HUILE));
        draw_char('C');

        /* ---------------------------------------------------------
         * Ligne 5 : Choc + Vitesse
         * --------------------------------------------------------- */
        goto_lico(5, 0);
        draw_string((unsigned char*)"Choc: ");
        if (CHOC == 0){
            draw_char('1');
            LED_R = 0;
            LED_G = 1;
            LED_B = 1;
        }
        else {
            draw_char('0');
        }

        draw_string((unsigned char*)"   Vitesse: ");

        // Logique vitesse conservee
        if (VITESSE_PLUS == 0 && vitesse < 255)
            vitesse++;
        if (VITESSE_MOINS == 0 && vitesse > 0)
            vitesse--;

        // NEW : si frein ON et marche = N -> vitesse forcee a 0
        if (FREIN_A_MAIN == 0 && MARCHE_AVANT != 0 && MARCHE_ARRIERE != 0)
        {
            vitesse = 0;
        }

        draw_dec8(vitesse);
        draw_string((unsigned char*)" km/h");

        /* ---------------------------------------------------------
         * Ligne 6 : Alerte vitesse
         * --------------------------------------------------------- */
        goto_lico(6, 0);
        if (vitesse > 30)
        {
            LED_R = 0;
            LED_G = 1;
            LED_B = 1;
            draw_string((unsigned char*)"ALERTE: vitesse > 30 km/h ");
        }
        else
        {
            LED_R = 1;
            LED_G = 0;
            LED_B = 1;
            draw_string((unsigned char*)"                         ");
        }

        /* ---------------------------------------------------------
         * Ligne 7 : Joystick X / Y sur la meme ligne
         * --------------------------------------------------------- */
        goto_lico(7, 0);
        draw_string((unsigned char*)"Joystick X:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(7, 20);
        draw_string((unsigned char*)"Y:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        /* ---------------------------------------------------------
         * Ligne 8 : Batterie en pourcentage (0%..100%)
         * --------------------------------------------------------- */
        goto_lico(8, 0);
        draw_string((unsigned char*)"Batterie: ");

        if (BATTERIE_PLUS == 0 && batterie < 100)   // NEW : limite haute
            batterie++;
        if (BATTERIE_MOINS == 0 && batterie > 0)    // NEW : limite basse
            batterie--;

        // Securite si jamais batterie depasse 100 ailleurs
        if (batterie > 100)                         // NEW : clamp
            batterie = 100;

        draw_dec8(batterie);
        draw_char('%');

        /* ---------------------------------------------------------
         * Affichage tactile
         * --------------------------------------------------------- */
        if (TP_appui == 1)
        {
            goto_lico(10, 0);
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
