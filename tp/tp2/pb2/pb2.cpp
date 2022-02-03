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
        V,
        R,
        E
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
                nextstate = R;
            }
            else
            {
                nextstate = INIT;
            }
            break;
        case R:
            if (pressedD2)
            {
                nextstate = V;
            }
            else if (pressedD3)
            {
                nextstate = E;
            }
            break;
        case V:
            if (pressedD2 || pressedD3)
            {
                nextstate = INIT;
            }
            break;
        case E:
            if (pressedD3)
            {
                nextstate = V;
            }
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
        case V:
            PORTA = 1;
            break;
        case R:
            PORTA = 2;
            break;
        case E:
            PORTA = 0;
            break;
        default:
            break;
        }
    }

    return 0;
}
