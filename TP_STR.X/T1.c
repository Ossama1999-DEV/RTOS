#include "T1.h"

#define BTN_REPEAT_DELAY        4u
#define VITESSE_ALERT_THRESHOLD 30u
#define BATTERIE_ALERT_THRESHOLD 30u

static void draw_text(unsigned char ligne, unsigned char colonne, const char *texte);
static void initialiser_affichage(void);
static void mettre_a_jour_marche(void);
static void mettre_a_jour_siege(void);
static void mettre_a_jour_temperatures(void);
static void mettre_a_jour_choc(void);
static void mettre_a_jour_vitesse(void);
static void mettre_a_jour_batterie(void);
static void mettre_a_jour_frein(void);
static void mettre_a_jour_badge(void);
static void mettre_a_jour_joystick(void);
static void mettre_a_jour_tactile(void);
static void mettre_a_jour_leds(void);
static void mettre_a_jour_alertes(void);
static unsigned char lire_canal(unsigned char canal);

static unsigned char alerte_choc = 0;
static unsigned char alerte_vitesse = 0;
static unsigned char alerte_batterie = 0;

void tache1(void)
{
    di();
    initialiser_affichage();
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

    while (1)
    {
        mettre_a_jour_marche();
        mettre_a_jour_siege();
        mettre_a_jour_temperatures();
        mettre_a_jour_choc();
        mettre_a_jour_vitesse();
        mettre_a_jour_batterie();
        mettre_a_jour_frein();
        mettre_a_jour_badge();
        mettre_a_jour_joystick();
        mettre_a_jour_tactile();
        mettre_a_jour_leds();
        mettre_a_jour_alertes();
    }
}

static void draw_text(unsigned char ligne, unsigned char colonne, const char *texte)
{
    goto_lico(ligne, colonne);
    draw_string((unsigned char *)texte);
}

static void initialiser_affichage(void)
{
    initialisation_afficheur();
    clear_text();
    clear_graphics();

    draw_text(0, 0, "ALERTE:");
    draw_text(1, 0, "Marche:");
    draw_text(2, 0, "Siege:");
    draw_text(3, 0, "Temp. Eau:");
    draw_text(4, 0, "Temp. Huile:");
    draw_text(5, 0, "Choc:");
    draw_text(6, 0, "Vitesse:");
    draw_text(7, 0, "Batterie:");
    draw_text(8, 0, "Frein:");
    draw_text(9, 0, "Badge:");
    draw_text(11, 0, "X-Joystick:");
    draw_text(12, 0, "Y-Joystick:");
    draw_text(13, 0, "Tactile:");
    draw_text(14, 0, "LEDs R/G/B:");

    goto_lico(0, 8);
    draw_string((unsigned char *)"OK");
    goto_lico(9, 7);
    draw_string((unsigned char *)"AUCUN");
    goto_lico(14, 13);
    draw_string((unsigned char *)"0 0 0");

    afficher_chariot();
}

static void mettre_a_jour_marche(void)
{
    static unsigned char precedent = 0xFFu;
    unsigned char etat;

    if (MARCHE_AVANT == 0)
    {
        etat = 1u;
    }
    else if (MARCHE_ARRIERE == 0)
    {
        etat = 2u;
    }
    else
    {
        etat = 0u;
    }

    if (etat != precedent)
    {
        goto_lico(1, 8);
        if (etat == 1u)
        {
            draw_string((unsigned char *)"AV");
        }
        else if (etat == 2u)
        {
            draw_string((unsigned char *)"AR");
        }
        else
        {
            draw_string((unsigned char *)"N ");
        }
        precedent = etat;
    }
}

static void mettre_a_jour_siege(void)
{
    static unsigned char precedent = 0xFFu;
    unsigned char etat = (SIEGE == 0) ? 1u : 0u;

    if (etat != precedent)
    {
        goto_lico(2, 7);
        draw_char(etat ? '1' : '0');
        precedent = etat;
    }
}

static void mettre_a_jour_temperatures(void)
{
    static unsigned char eau_prec = 0xFFu;
    static unsigned char huile_prec = 0xFFu;
    unsigned char eau = lire_canal(TEMPERATURE_EAU);
    unsigned char huile = lire_canal(TEMPERATURE_HUILE);

    if (eau != eau_prec)
    {
        goto_lico(3, 11);
        draw_hex8(eau);
        eau_prec = eau;
    }

    if (huile != huile_prec)
    {
        goto_lico(4, 12);
        draw_hex8(huile);
        huile_prec = huile;
    }
}

static void mettre_a_jour_choc(void)
{
    static unsigned char precedent = 0xFFu;
    unsigned char choc = (CHOC == 0) ? 1u : 0u;

    if (choc != precedent)
    {
        goto_lico(5, 6);
        draw_char(choc ? '1' : '0');
        precedent = choc;
    }

    alerte_choc = choc;
}

static void mettre_a_jour_vitesse(void)
{
    static unsigned char precedent = 0xFFu;
    static unsigned char delai = 0u;
    unsigned char modifie = 0u;

    if (delai > 0u)
    {
        delai--;
    }

    if (delai == 0u)
    {
        if ((VITESSE_PLUS == 0) && (vitesse < 0xFFu))
        {
            vitesse++;
            modifie = 1u;
            delai = BTN_REPEAT_DELAY;
        }
        else if ((VITESSE_MOINS == 0) && (vitesse > 0u))
        {
            vitesse--;
            modifie = 1u;
            delai = BTN_REPEAT_DELAY;
        }
    }

    if ((vitesse != precedent) || (modifie == 1u && delai == BTN_REPEAT_DELAY))
    {
        goto_lico(6, 9);
        draw_dec8(vitesse);
        precedent = vitesse;
    }

    alerte_vitesse = (vitesse > VITESSE_ALERT_THRESHOLD) ? 1u : 0u;
}

static void mettre_a_jour_batterie(void)
{
    static unsigned char precedent = 0xFFu;
    static unsigned char delai = 0u;
    unsigned char modifie = 0u;

    if (delai > 0u)
    {
        delai--;
    }

    if (delai == 0u)
    {
        if ((BATTERIE_PLUS == 0) && (batterie < 0xFFu))
        {
            batterie++;
            modifie = 1u;
            delai = BTN_REPEAT_DELAY;
        }
        else if ((BATTERIE_MOINS == 0) && (batterie > 0u))
        {
            batterie--;
            modifie = 1u;
            delai = BTN_REPEAT_DELAY;
        }
    }

    if ((batterie != precedent) || (modifie == 1u && delai == BTN_REPEAT_DELAY))
    {
        goto_lico(7, 10);
        draw_hex8(batterie);
        precedent = batterie;
    }

    alerte_batterie = (batterie < BATTERIE_ALERT_THRESHOLD) ? 1u : 0u;
}

static void mettre_a_jour_frein(void)
{
    static unsigned char precedent = 0xFFu;
    unsigned char actif = (FREIN_A_MAIN == 0) ? 1u : 0u;

    if (actif != precedent)
    {
        goto_lico(8, 7);
        if (actif)
        {
            draw_string((unsigned char *)"ACTIF ");
        }
        else
        {
            draw_string((unsigned char *)"LIBRE ");
        }
        precedent = actif;
    }
}

static void mettre_a_jour_badge(void)
{
    static unsigned char precedent_nb = 0xFFu;
    static unsigned char cache[10];
    unsigned char nb = n_octet_badge;
    unsigned char different = 0u;
    unsigned char i;

    if (nb != precedent_nb)
    {
        different = 1u;
    }
    else
    {
        for (i = 0u; i < nb; i++)
        {
            if (badge[i] != cache[i])
            {
                different = 1u;
                break;
            }
        }
    }

    if (different)
    {
        goto_lico(9, 7);
        draw_string((unsigned char *)"                    ");
        if (nb == 0u)
        {
            goto_lico(9, 7);
            draw_string((unsigned char *)"AUCUN");
        }
        else
        {
            goto_lico(9, 7);
            for (i = 0u; (i < nb) && (i < 10u); i++)
            {
                draw_hex8(badge[i]);
                draw_char(' ');
                cache[i] = badge[i];
            }
        }
        precedent_nb = nb;
    }
}

static void mettre_a_jour_joystick(void)
{
    static unsigned char x_prec = 0xFFu;
    static unsigned char y_prec = 0xFFu;
    unsigned char x = lire_canal(JOYSTICK_X);
    unsigned char y = lire_canal(JOYSTICK_Y);

    if (x != x_prec)
    {
        goto_lico(11, 12);
        draw_dec8(x);
        x_prec = x;
    }

    if (y != y_prec)
    {
        goto_lico(12, 12);
        draw_dec8(y);
        y_prec = y;
    }
}

static void mettre_a_jour_tactile(void)
{
    static unsigned char appui_prec = 0xFFu;
    static unsigned char x_prec = 0u;
    static unsigned char y_prec = 0u;
    unsigned char appui = TP_appui;
    unsigned char x = TP_x;
    unsigned char y = TP_y;

    if ((appui != appui_prec) || (appui == 1u && (x != x_prec || y != y_prec)))
    {
        goto_lico(13, 9);
        draw_string((unsigned char *)"                     ");
        if (appui == 1u)
        {
            goto_lico(13, 9);
            draw_string((unsigned char *)"x=");
            draw_dec8(x);
            draw_string((unsigned char *)" y=");
            draw_dec8(y);
            plot1(x, y);
        }
        else
        {
            goto_lico(13, 9);
            draw_string((unsigned char *)"--");
        }
        appui_prec = appui;
        x_prec = x;
        y_prec = y;
    }
}


static unsigned char lire_canal(unsigned char canal)
{
    unsigned char valeur;

    P(SEM_CAN);
    valeur = lecture_8bit_analogique(canal);
    V(SEM_CAN);

    return valeur;
}

static void mettre_a_jour_leds(void)
{
    static unsigned char r_prec = 0xFFu;
    static unsigned char g_prec = 0xFFu;
    static unsigned char b_prec = 0xFFu;
    unsigned char r = LED_R ? 1u : 0u;
    unsigned char g = LED_G ? 1u : 0u;
    unsigned char b = LED_B ? 1u : 0u;

    if ((r != r_prec) || (g != g_prec) || (b != b_prec))
    {
        goto_lico(14, 13);
        draw_char(r ? '1' : '0');
        draw_char(' ');
        draw_char(g ? '1' : '0');
        draw_char(' ');
        draw_char(b ? '1' : '0');
        draw_string((unsigned char *)"   ");
        r_prec = r;
        g_prec = g;
        b_prec = b;
    }
}

static void mettre_a_jour_alertes(void)
{
    static unsigned char niveau_prec = 0xFFu;
    static const char *message_prec = (const char *)0;
    const char *message;
    unsigned char niveau;

    if (alerte_choc == 1u)
    {
        message = "CHOC DETECTE";
        niveau = 3u;
    }
    else if (alerte_batterie == 1u)
    {
        message = "BATTERIE FAIBLE";
        niveau = 2u;
    }
    else if (alerte_vitesse == 1u)
    {
        message = "VITESSE ELEVEE";
        niveau = 1u;
    }
    else
    {
        message = "OK";
        niveau = 0u;
    }

    if ((niveau != niveau_prec) || (message != message_prec))
    {
        goto_lico(0, 8);
        draw_string((unsigned char *)"                   ");
        goto_lico(0, 8);
        draw_string((unsigned char *)message);
        niveau_prec = niveau;
        message_prec = message;
    }
}
