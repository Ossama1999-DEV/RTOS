/**!
 * \file T1.c
 * \brief Tache 1 : Affichage des informations sur l'afficheur graphique
 * \author DBIBIH Oussama
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
    RXTX_libre=1;
    TXREG1='R';
    ei();

    /*
        cette partie initialise les variables RGB (RVB)
    */
    LED_R=0;LED_G=0;LED_B=0;

    vitesse=0;
    batterie=120;
    n_octet_badge=0;

    /*
        cette partie est statique, on l'affiche une fois pour toutes
    */
    goto_lico(13,34);draw_char('R');draw_char(' ');draw_char('V');draw_char(' ');draw_char('B');
    goto_lico(14,34);draw_char('0');draw_char(' ');draw_char('0');draw_char(' ');draw_char('0');
    goto_lico(15,34);draw_char('1');draw_char(' ');draw_char('1');draw_char(' ');draw_char('1');

    TP_appui=0; // initialise la variable de l'ecran tactile

    while(1)
    {

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
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU)); // return la lecture du capteur de température d'eau

        goto_lico(3,0);
        draw_string("Temp. Huile:");
        draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE)); // return la lecture du capteur de température d'huile

        goto_lico(4,0);
        draw_string("Choc:");
        if (CHOC==0)
            draw_char('1');
        else
            draw_char('0');

        goto_lico(5,0);
        draw_string("Vitesse:");
        if (VITESSE_PLUS==0) // button pour augmenter la valeur de la vitesse
            vitesse++;
        if (VITESSE_MOINS==0) // button pour diminuer la valeur de la vitesse
            vitesse--;
        draw_hex8(vitesse);

        goto_lico(6,0);
        draw_string("Batterie:");
        if (BATTERIE_PLUS==0) // button pour augmenter la valeur de la batterie
            batterie++;
        if (BATTERIE_MOINS==0) // button pour diminuer la valeur de la batterie
            batterie--;
        draw_hex8(batterie);

        goto_lico(7,0);
        if (FREIN_A_MAIN==0) // si le frein a main est active
            draw_string("((!))");
        else
            draw_string("     ");

        goto_lico(8,0);
        draw_string("Badge:");
        if (n_octet_badge==0) // si aucun badge n'est detecte
            draw_string(" AUCUN              ");
        else
        {
            for (i=0;i<n_octet_badge;i++)
            {
                draw_hex8(badge[i]); // affiche le contenu du badge en hexadecimal de T6
            }
        }

        goto_lico(9,0);
        draw_string("X-Joystick:"); 
        draw_dec8(lecture_8bit_analogique(JOYSTICK_X)); // return la lecture du joystick en X

        goto_lico(10,0);
        draw_string("Y-Joystick:");
        draw_dec8(lecture_8bit_analogique(JOYSTICK_Y)); // return la lecture du joystick en Y

        if (TP_appui==1)
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