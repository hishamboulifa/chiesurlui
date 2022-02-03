/// \Travail : probleme 1 du TP2
/// \auteurs : Laurent Langlois (2075635), Thomas Duperron (2016459)
/// \Section # : 5
/// \Equipe # : 204
/// \correcteur : Eya-Tom Augustin Sangam, Tristan Rioux

/*
Tableau des états:
------------------------------------------------------------
| Etat present |   D2   |   D3   | Etat suivant | Sortie Z |
------------------------------------------------------------
|     INIT     |   0    |   X    |     INIT     |  Eteint  |
------------------------------------------------------------
|     INIT     |   1    |   X    |      V0      |  Eteint  |
------------------------------------------------------------
|      V0      |   X    |   0    |      V0      |   Vert   |
------------------------------------------------------------
|      V0      |   X    |   1    |      V1      |   Vert   |
------------------------------------------------------------
|      V1      |   X    |   0    |      V1      |   Vert   |
------------------------------------------------------------
|      V1      |   X    |   1    |      V2      |   Vert   |
------------------------------------------------------------
|      V2      |   X    |   0    |      V2      |   Vert   |
------------------------------------------------------------
|      V2      |   X    |   1    |      R       |   Vert   |
------------------------------------------------------------
|      R       |   X    |   X    |     INIT     |   Rouge  |
------------------------------------------------------------
*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void relache(bool &pressedD2, bool &pressedD3)
{
    if (PIND & 0x04)
    {
        while (PIND & 0x04)
        {
        }
        pressedD2 = true;
    }
    else if (PIND & 0x08)
    {
        while (PIND & 0x08)
        {
        }
        pressedD3 = true;
    }
}

int main()
{
    DDRA = 0xff; // PORT A est en mode sortie
    DDRD = 0x00;
    enum state
    {
        INIT,
        V0,
        V1,
        V2,
        R
    };
    state currentstate = INIT, nextstate = INIT;
    bool pressedD2 = false, pressedD3 = false;
    for (;;) // boucle sans fin
    {
        pressedD2 = false;
        pressedD3 = false;
        relache(pressedD2, pressedD3);
        switch (currentstate)
        {
        case INIT:
            if (pressedD2)
            {
                nextstate = V0;
            }
            else
            {
                nextstate = INIT;
            }
            break;
        case V0:
            if (pressedD3)
            {
                nextstate = V1;
            }
            break;
        case V1:
            if (pressedD3)
            {
                nextstate = V2;
            }
            break;
        case V2:
            if (pressedD3)
            {
                nextstate = R;
            }
            break;
        case R:
            _delay_ms(1000);
            nextstate = INIT;
            break;
        default:
            break;
        }
        currentstate = nextstate;
        switch (currentstate)
        {
        case INIT:
            PORTA = 0;
            break;
        case V0:
            PORTA = 1;
            break;
        case V1:
            PORTA = 1;
            break;
        case V2:
            PORTA = 1;
            break;
        case R:
            PORTA = 2;
            break;
        default:
            break;
        }
    }
    return 0;
}
