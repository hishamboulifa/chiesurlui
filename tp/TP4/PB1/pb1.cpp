/// \Travail : probleme 2 du TP2
/// \auteurs : Laurent Langlois (2075635), Thomas Duperron (2016459)
/// \Section # : 5
/// \Equipe # : 204
/// \correcteur : Eya-Tom Augustin Sangam, Tristan Rioux

/*
Tableau des etats :
------------------------------------------------------------
| Etat present |   D2   |   D3   | Etat suivant | Sortie Z |
------------------------------------------------------------
|     INIT     |   X    |   1    |     INIT     |  Eteint  |
------------------------------------------------------------
|     INIT     |   1    |   X    |      R0      |  Eteint  |
------------------------------------------------------------
|      R0      |   1    |   X    |      V0      |  Rouge   |
------------------------------------------------------------
|      R0      |   X    |   1    |      E0      |  Rouge   |
------------------------------------------------------------
|      V0      |   1    |   X    |     INIT     |   Vert   |
------------------------------------------------------------
|      V0      |   X    |   1    |     INIT     |   Vert   |
------------------------------------------------------------
|      E0      |   X    |   1    |      V0      |  Eteint  |
------------------------------------------------------------
*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
volatile uint8_t etat = 0;
ISR(INT0_vect)
{
    _delay_ms(30);
    //vérifier si pressed
    etat = (etat + 1) % 3;
    EIFR |= (1 << INTF0) ;
}

void initialisation(void)
{

    cli();
    DDRA = 0xff;
    DDRD = 0x00;
    EIMSK |= (1 << INT0);
    EICRA |= 0b11;
    sei();
}

int main()
{
    initialisation();
    for (;;) // boucle sans fin
    {
        switch (etat)
        {
        case 0:
            PORTA = 0;
            break;
        case 1:
            PORTA = 1;
            break;
        case 2:
            PORTA = 2;
            break;
        default:
            break;
        }
    }

    return 0;
}
