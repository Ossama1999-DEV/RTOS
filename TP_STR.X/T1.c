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

    LED_R = 0;
    LED_G = 0;
    LED_B = 0;

    vitesse = 0;
    batterie = 120;
    n_octet_badge = 0;
    TP_appui = 0;

    // === En-tête affichage ===
    goto_lico(13, 34);
    draw_char('R'); draw_char(' '); draw_char('V'); draw_char(' '); draw_char('B');
    goto_lico(14, 34);
    draw_char('0'); draw_char(' '); draw_char('0'); draw_char(' '); draw_char('0');
    goto_lico(15, 34);
    draw_char('1'); draw_char(' '); draw_char('1'); draw_char(' '); draw_char('1');

    while (1)
    {
        // === Marche ===
        goto_lico(0, 0);
        draw_string((unsigned char*)"Marche:");

        if (MARCHE_AVANT == 0)
        {
            draw_string((unsigned char*)"AV");

            // === Écran vert (marche avant) ===
            LED_R = 1;  // OFF
            LED_G = 0;  // ON
            LED_B = 1;  // OFF

            // set_color(0, 255, 0);  // vert
            // clear_graphics();
        }
        else if (MARCHE_ARRIERE == 0)
        {
            draw_string((unsigned char*)"AR");

            // === Écran jaune (marche arrière) ===
            LED_R = 0;  // ON
            LED_G = 0;  // ON
            LED_B = 1;  // OFF

            // set_color(255, 255, 0);  // jaune
            // clear_graphics();
        }
        else
        {
            draw_string((unsigned char*)"N ");

            // === Neutre ===
            LED_R = 1;
            LED_G = 1;
            LED_B = 1;

            // set_color(255, 255, 255);  // blanc
            // clear_graphics();
        }

        // === Siege ===
        goto_lico(1, 0);
        draw_string((unsigned char*)"Siege:");
        draw_char(SIEGE == 0 ? '1' : '0');

        // === Températures ===
        goto_lico(2, 0);
        draw_string((unsigned char*)"Temp. Eau:");
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU));

        goto_lico(3, 0);
        draw_string((unsigned char*)"Temp. Huile:");
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE));

        // === Choc ===
        goto_lico(4, 0);
        draw_string((unsigned char*)"Choc:");

        if (CHOC == 0)
        {
            draw_char('1');
            LED_R = 0; LED_G = 1; LED_B = 1;

            goto_lico(7, 0);
            draw_string((unsigned char*)"!!! ALERTE CHOC !!!");
        }
        else
        {
            draw_char('0');
            LED_R = 1; LED_G = 0; LED_B = 1;

            goto_lico(7, 0);
            draw_string((unsigned char*)" ");
        }

        // === Vitesse ===
        goto_lico(5, 0);
        draw_string((unsigned char*)"Vitesse:");

        if (VITESSE_PLUS == 0 && vitesse < 255)
            vitesse++;

        if (VITESSE_MOINS == 0 && vitesse > 0)
            vitesse--;

        draw_hex8(vitesse);

        if (vitesse > 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;
            goto_lico(6, 0);
            draw_string((unsigned char*)"ALERTE: >30 km/h");
        }
        else
        {
            LED_R = 1; LED_G = 0; LED_B = 1;
            goto_lico(6, 0);
            draw_string((unsigned char*)" ");
        }

        // === Batterie ===
        goto_lico(8, 0);
        draw_string((unsigned char*)"Batterie:");

        // Variation manuelle
        if (BATTERIE_PLUS == 0)
            batterie++;

        if (BATTERIE_MOINS == 0 && batterie > 0)
            batterie--;

        // Alerte batterie faible
        if (batterie < 30)
        {
            // === Écran rouge (batterie faible) ===
            LED_R = 0;  // ON
            LED_G = 1;  // OFF
            LED_B = 1;  // OFF

            // set_color(255, 0, 0);  // rouge
            // clear_graphics();

            goto_lico(9, 0);
            draw_string((unsigned char*)"!!! BATTERIE FAIBLE !!!");
        }
        else
        {
            // Rétablir couleurs normales
            LED_R = 1;
            LED_G = 0;  // vert ON
            LED_B = 1;

            // Effacer le message précédent
            goto_lico(9, 0);
            draw_string((unsigned char*)" ");
        }

        // Affichage du niveau
        draw_hex8(batterie);

        // === Frein à main ===
        goto_lico(9, 0);
        draw_string((unsigned char*)"Frein:");

        // Si le frein à main est serré
        if (FREIN_A_MAIN == 0)
        {
            draw_string((unsigned char*)"((!))");

            // === Écran rouge (alerte) ===
            LED_R = 0;  // ON
            LED_G = 1;  // OFF
            LED_B = 1;  // OFF

            // set_color(255, 0, 0);
            // clear_graphics();

            goto_lico(10, 0);
            draw_string((unsigned char*)"ENLEVEZ FREIN A MAIN");

            // Empêche la marche avant
            if (MARCHE_AVANT == 0)
            {
                draw_string((unsigned char*)" - BLOQUE -");
                vitesse = 0;  // sécurité : force l'arrêt
            }
        }
        else
        {
            draw_string((unsigned char*)" ");

            // État normal : vert
            LED_R = 1;
            LED_G = 0;
            LED_B = 1;

            // Nettoie le message d’alerte
            goto_lico(10, 0);
            draw_string((unsigned char*)" ");
        }

        // === Badge ===
        goto_lico(10, 0);
        draw_string((unsigned char*)"Badge:");

        if (n_octet_badge == 0)
        {
            draw_string((unsigned char*)" AUCUN");
            goto_lico(11, 0);
            draw_string((unsigned char*)"Inserez carte:");
        }
        else
        {
            // Nettoie proprement la ligne "Inserez carte:"
            goto_lico(11, 0);
            draw_string((unsigned char*)" ");

            for (i = 0; i < n_octet_badge; i++)
                draw_hex8(badge[i]);
        }

        // === Joystick ===
        goto_lico(12, 0);
        draw_string((unsigned char*)"X-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(13, 0);
        draw_string((unsigned char*)"Y-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));

        // === Touch Panel ===
        if (TP_appui == 1)
        {
            goto_lico(0, 20);
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
