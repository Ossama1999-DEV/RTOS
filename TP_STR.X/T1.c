/**!
 * @file T1.c
 * @author DBIBIH Oussama
 * @brief Tache 1 : Affichage des informations
 * @version 1.0
 */
#include "T1.h"

static void afficher_marche(void);
static void afficher_siege(void);
static void afficher_temperatures(void);
static void afficher_choc(void);
static void afficher_vitesse(void);
static void afficher_batterie(void);
static void afficher_frein_a_main(void);
static void afficher_badge(void);
static void afficher_joystick(void);
static void afficher_tactile(void);
static void rafraichir_affichage_si_necessaire(void);

void tache1(void)
{
    di();
    initialisation_afficheur();
    clear_text();
    clear_graphics();
    goto_lico(0, 0);
    draw_string((unsigned char*)"⚠ ⚙ 🪫 ");
    goto_lico(0, 40);
    draw_string((unsigned char*)"Badge:");
    // Batterie (icône fixe)
    draw_rectangle(110, 0, 125, 8); // cadre batterie
    draw_line(126, 2, 126, 6);      // tête batterie
    // Chariot (image pixel)
    afficher_chariot(); // dessin stylisé via pixels
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
    goto_lico(13, 34);
    draw_char('R');
    draw_char(' ');
    draw_char('V');
    draw_char(' ');
    draw_char('B');
    goto_lico(14, 34);
    draw_char('0');
    draw_char(' ');
    draw_char('0');
    draw_char(' ');
    draw_char('0');
    goto_lico(15, 34);
    draw_char('1');
    draw_char(' ');
    draw_char('1');
    draw_char(' ');
    draw_char('1');
    TP_appui = 0;
    while (1)
    {
        afficher_marche();
        afficher_siege();
        afficher_temperatures();
        afficher_choc();
        afficher_vitesse();
        afficher_batterie();
        afficher_frein_a_main();
        afficher_badge();
        afficher_joystick();
        afficher_tactile();
        rafraichir_affichage_si_necessaire();
    }
}
static void afficher_marche(void)
{
    goto_lico(0, 0);
    draw_string("Marche:");
    if (MARCHE_AVANT == 0)
    {
        draw_string("AV");
    }
    else if (MARCHE_ARRIERE == 0)
    {
        draw_string("AR");
    }
    else
    {
        draw_string("N ");
    }
}
static void afficher_siege(void)
{
    goto_lico(1, 0);
    draw_string("Siege:");
    if (SIEGE == 0)
    {
        draw_char('1');
    }
    else
    {
        draw_char('0');
    }
}
static void afficher_temperatures(void)
{
    goto_lico(2, 0);
    draw_string("Temp. Eau:");
    draw_hex8(lecture_8bit_analogique(TEMPERATURE_EAU));
    goto_lico(3, 0);
    draw_string("Temp. Huile:");
    draw_hex8(lecture_8bit_analogique(TEMPERATURE_HUILE));
}
static void afficher_choc(void)
{
    goto_lico(4, 0);
    draw_string("Choc:");
    if (CHOC == 0)
    {
        draw_char('1');
        afficher_alerte("!!! ALERTE CHOC !!!");
    }
    else
    {
        draw_char('0');
    }
}
static void afficher_vitesse(void)
{
    if ((VITESSE_PLUS == 0) && (vitesse < 0xFFu))
    {
        vitesse++;
    }
    if ((VITESSE_MOINS == 0) && (vitesse > 0u))
    {
        vitesse--;
    }
    goto_lico(5, 0);
    draw_string("Vitesse:");
    draw_hex8(vitesse);
    if (vitesse > 30)
    {
        afficher_alerte("!!! VITESSE !!!");
    }
}
static void afficher_batterie(void)
{
    if ((BATTERIE_PLUS == 0) && (batterie < 0xFFu))
    {
        batterie++;
    }
    if ((BATTERIE_MOINS == 0) && (batterie > 0u))
    {
        batterie--;
    }
    goto_lico(6, 0);
    draw_string("Batterie:");
    draw_hex8(batterie);
    if (batterie < 30)
    {
        afficher_alerte("!!! BATTERIE !!!");
    }
}
static void afficher_frein_a_main(void)
{
    goto_lico(7, 0);
    if (FREIN_A_MAIN == 0)
    {
        draw_string("((!))");
    }
    else
    {
        draw_string("     ");
    }
}
static void afficher_badge(void)
{
    goto_lico(8, 0);
    draw_string("Badge:");
    if (n_octet_badge == 0)
    {
        draw_string(" AUCUN              ");
    }
    else
    {
        unsigned char i;
        for (i = 0; i < n_octet_badge; i++)
        {
            draw_hex8(badge[i]);
        }
    }
}
static void afficher_joystick(void)
{
    goto_lico(9, 0);
    draw_string("X-Joystick:");
    draw_dec8(lecture_8bit_analogique(JOYSTICK_X));
    goto_lico(10, 0);
    draw_string("Y-Joystick:");
    draw_dec8(lecture_8bit_analogique(JOYSTICK_Y));
}
static void afficher_tactile(void)
{
    if (TP_appui == 1)
    {
        goto_lico(0, 20);
        draw_string("x=");
        draw_dec8(TP_x);
        draw_string(" y=");
        draw_dec8(TP_y);
        plot1(TP_x, TP_y);
    }
    else
    {
        Nop();
    }
}
static void rafraichir_affichage_si_necessaire(void)
{
    if ((Tick_Count % 3) == 0)
    {
        rafraichir_affichage();
    }
}