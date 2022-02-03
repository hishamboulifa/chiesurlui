/// \Travail : probleme 1 du TP6
/// \auteurs : Laurent Langlois (2075635), Thomas Duperron (2016459)
/// \Section # : 5
/// \Equipe # : 204
/// \correcteur : Eya-Tom Augustin Sangam, Tristan Rioux

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"

const uint8_t NIVEAU_HAUT = 80; // Valeur obtenue par la methode essai-erreur
const uint8_t NIVEAU_MOY = 110; // Valeur obtenue par la methode essai-erreur
const uint8_t ROUGE = 2; // Pour mettre sous tension la pin B1
const uint8_t VERT = 1; // Pour mettre sous tension la pin B0

int main()
{
    DDRA = 0x00;
    DDRB = 0xff;
    can lecteur;
    uint16_t lect16;
    uint8_t niveaulum;

    for (;;)
    {
        lect16 = lecteur.lecture(0);
        niveaulum = lect16 >> 2;

        if (niveaulum < NIVEAU_HAUT)
        {
            PORTB = ROUGE;
        }
        else if (niveaulum < NIVEAU_MOY)
        {
            PORTB = VERT;
            _delay_ms(2);
            PORTB = ROUGE;
            _delay_ms(3);
        }
        else
        {
            PORTB = VERT;
        }
    }

    return 0;
}