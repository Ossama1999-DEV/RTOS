/**!
 * \file T4.c
 * \brief Tâche 4 : Supervision + alerte >30 km/h
 * \author DBIBIH Oussama
 */

#include "T4.h"
#include "afficheur.h"   // draw_string, clear_graphics, goto_lico, draw_dec8, draw_hex8...
#include "main.h"        // LED_R/G/B, JOYSTICK_X/Y, TEMPERATURE_EAU/HUILE, VITESSE_PLUS/MOINS

// vitesse : si elle est déjà définie ailleurs (T1), laisse cette ligne.
// Sinon, décommente la définition locale plus bas (voir option B).

static void uart_print(const char *s)
{
    while (*s)
    {
        while (PIR1bits.TX1IF == 0);
        TXREG1 = *s++;
        while (TXSTA1bits.TRMT == 0);
    }
}

void tache4(void)
{
    unsigned int a;

    while (1)
    {
        // 1) Nettoyage + en-tête
        clear_graphics();
        goto_lico(0, 0);
        draw_string((unsigned char*)"=== SUPERVISION T4 ===");

        // 2) Envoi série statut
        uart_print("TACHE4_ENCOURS\r\n");

        // 3) Lecture des capteurs (comme T1, mais ici en T4)
        unsigned char te = lecture_8bit_analogique(TEMPERATURE_EAU);
        unsigned char th = lecture_8bit_analogique(TEMPERATURE_HUILE);
        unsigned char jx = lecture_8bit_analogique(JOYSTICK_X);
        unsigned char jy = lecture_8bit_analogique(JOYSTICK_Y);

        // (Option B) Si tu veux gérer la vitesse UNIQUEMENT en T4,
        // décommente ce bloc et COMENTE le 'extern unsigned char vitesse;' en haut.
        /*
        static unsigned char vitesse = 0;
        if (VITESSE_PLUS == 0 && vitesse < 255) vitesse++;
        if (VITESSE_MOINS == 0 && vitesse > 0)  vitesse--;
        */

        // 4) Affichage propre et aligné
        goto_lico(2, 0);
        draw_string((unsigned char*)"Parametres :");

        goto_lico(3, 0);
        draw_string((unsigned char*)"Vitesse : ");
        draw_dec8(vitesse);
        draw_string((unsigned char*)" km/h");

        goto_lico(4, 0);
        draw_string((unsigned char*)"Temp. Eau : ");
        draw_hex8(te);
        draw_string((unsigned char*)" C");

        goto_lico(5, 0);
        draw_string((unsigned char*)"Temp. Huile : ");
        draw_hex8(th);
        draw_string((unsigned char*)" C");

        goto_lico(6, 0);
        draw_string((unsigned char*)"Joystick X=");
        draw_dec8(jx);
        draw_string((unsigned char*)" Y=");
        draw_dec8(jy);

        // 5) Alerte > 30 km/h (LED rouge + message)
        goto_lico(8, 0);
        if (vitesse > 30)
        {
            LED_R = 0; LED_G = 1; LED_B = 1;  // Rouge ON
            draw_string((unsigned char*)"!!! DANGER: >30 km/h !!!");
            // Si ta lib gère un fond rouge, tu pourrais faire:
            // set_color(255, 0, 0); clear_graphics(); ... (mais optionnel)
        }
        else
        {
            LED_R = 1; LED_G = 0; LED_B = 1;  // Vert ON
            draw_string((unsigned char*)"Vitesse OK (<30 km/h)");
        }

        goto_lico(10, 0);
        draw_string((unsigned char*)"-----------------------------");

        // 6) Petite pause
        for (a = 0; a < 65000; a++);
    }
}
