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
    RXTX_libre=1;
    TXREG1='R';
    ei();

    LED_R=0;LED_G=0;LED_B=0;

    vitesse=0;
    batterie=120;
    n_octet_badge=0;

    goto_lico(13,34);draw_char('R');draw_char(' ');draw_char('V');draw_char(' ');draw_char('B');
    goto_lico(14,34);draw_char('0');draw_char(' ');draw_char('0');draw_char(' ');draw_char('0');
    goto_lico(15,34);draw_char('1');draw_char(' ');draw_char('1');draw_char(' ');draw_char('1');

    TP_appui=0;

    while(1)
    {
        if (n_octet_badge == 0){
            draw_string("Inserez carte:");
        }
        else if(n_octet_badge == 1) {

        goto_lico(0,0);
        draw_string("Marche:");
        if (MARCHE_AVANT==0)
            draw_string("AV");
        else
            if (MARCHE_ARRIERE==0)
                draw_string("AR");
            else
                draw_string("N ");

        goto_lico(1,0);
        draw_string("Siege:");
        if (SIEGE==0)
        {draw_char('1');}
        else
        {draw_char('0');}

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

            // Petit clignotement sans bloquer : on divise la fréquence d'appel
            static unsigned char blink_div = 0;
            static unsigned char blink_state = 0;

            if (++blink_div >= 20) {   // ajuste 20 pour changer la vitesse du blink
                blink_div = 0;
                blink_state ^= 1;      // toggle 0/1
            }

            if (blink_state) {
                // Mode ALERTE : LED rouge ON
                LED_R = 0; LED_G = 1; LED_B = 1;

                // Si ta lib GLCD gère la couleur de fond, tu peux décommenter :
                // set_color(255, 0, 0);
                // clear_graphics();
                // goto_lico(0,0);
                draw_string("!!! ALERTE CHOC !!!");
            } else {
                // Etat alterné (pour clignoter) : repasse en "normal"
                LED_R = 1; LED_G = 0; LED_B = 1;
            }

            // Ligne d'alerte (clignote)
            goto_lico(7,0);
            if (blink_state)
                draw_string((unsigned char*)"!!! ALERTE CHOC !!!");
            else
                draw_string((unsigned char*)"                   "); // efface la ligne

        } else {
            draw_char('0');

            // Retour etat normal
            LED_R = 1; LED_G = 0; LED_B = 1;

            // Nettoie la ligne d'alerte si elle etait affichee
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

        // ===== AJOUT : alerte si > 30 km/h =====
        if (vitesse > 30) {
            // clear_graphics();        // fond rouge si géré, puis réafficher l'écran

            // Indicateur visuel via LEDs (sécurité)
            LED_R = 0;   // Rouge ON
            LED_G = 1;   // Vert OFF
            LED_B = 1;   // Bleu OFF

            // Message d'alerte
            goto_lico(6,0);
            draw_string((unsigned char*)"ALERTE: >30 km/h");
        } else {
            // Etat normal (vert)
            LED_R = 1;
            LED_G = 0;   // Vert ON
            LED_B = 1;

            // Efface juste la ligne d'alerte si elle etait affichée
            goto_lico(6,0);
            draw_string((unsigned char*)"                 ");
        }

        goto_lico(7,0);
        draw_string("Batterie:");
        if (BATTERIE_PLUS==0)
            batterie++;
        if (BATTERIE_MOINS==0)
            batterie--;
        draw_hex8(batterie);

        goto_lico(8,0);
        if (FREIN_A_MAIN==0)
            draw_string("((!))");
        else
            draw_string("     ");

        goto_lico(9,0);
        draw_string("Badge:");
        if (n_octet_badge==0)
            draw_string(" AUCUN              ");
        else
        {
            for (i=0;i<n_octet_badge;i++)
            {
                draw_hex8(badge[i]);
            }
        }

        goto_lico(10,0);
        draw_string("X-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X));

        goto_lico(11,0);
        draw_string("Y-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));
        }
        else if (TP_appui==1)
        {
            goto_lico(0,20);
            draw_string("x=");
            draw_dec8(TP_x);
            draw_string(" y=");
            draw_dec8(TP_y);
            plot1(TP_x,TP_y);
        }
        else
        {
            Nop();
        }
    }
}