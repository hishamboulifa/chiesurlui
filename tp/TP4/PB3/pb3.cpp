/// \Travail : probleme 2 du TP2
/// \auteurs : Laurent Langlois (2075635), Thomas Duperron (2016459)
/// \Section # : 5
/// \Equipe # : 204
/// \correcteur : Eya-Tom Augustin Sangam, Tristan Rioux

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

void ajustementPWM(uint16_t pourcentage)
{

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5

    // page 130 de la description technique du ATmega324PA)

    OCR1A = pourcentage;

    OCR1B = pourcentage;

    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A = 0b10100001;

    TCCR1B = 0b00000010;

    TCCR1C = 0;
}
int main()
{
    TCNT1 = 0;
    DDRD = 0xff;
    for (;;)
    {
        ajustementPWM(254); //100%
        _delay_ms(2000);
        ajustementPWM(191); //75%
        _delay_ms(2000);
        ajustementPWM(127); //50%
        _delay_ms(2000);
        ajustementPWM(64); //25%
        _delay_ms(2000);
        ajustementPWM(0); //0%
        _delay_ms(2000);
    }

    return 0;
}
