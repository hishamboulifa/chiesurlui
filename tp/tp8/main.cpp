/*
etat    D2      D3      etatsuivant
-----------------------------------
eteint  1->0    X       pret
pret    X       1->0    click1
click1  X       1->0    click2
click2  X       1->0    click3
click3  X       X       eteint (1s)


*/

#define F_CPU 8000000

#include <avr/io.h> 
#include <util/delay.h>
#include "debug.h" 

int main(){
    DDRA = 0xff;
    DDRD = 0x00;

    enum Etat {eteint, pret, click1, click2, click3};

    Etat etatPresent = eteint;
    uint8_t D2 = 0x04;
    uint8_t D3 = 0x08;
    uint8_t boutonsAvant = 0x0;

    

    for(;;){
        uint8_t boutons = PIND;
        _delay_ms(10.0);
        uint8_t boutonsConf = PIND & boutons;
        // bouton était pressé mais plus maintenant
        uint8_t boutonsClick = boutonsAvant & ~boutonsConf;


        switch(etatPresent){
            case eteint:
                PORTA = 0x0;
                if(boutonsClick & D2){
                    etatPresent = pret;
                }
                break;
            case pret:
                PORTA = 0x1;
                if(boutonsClick & D3){
                    etatPresent = click1;
                }
                break;
            case click1:
                PORTA = 0x1;
                if(boutonsClick & D3){
                    etatPresent = click2;
                }
                break;
            case click2:
                PORTA = 0x1;
                if(boutonsClick & D3){
                    etatPresent = click3;
                }
                break;
            case click3:
                PORTA = 0x2;
                _delay_ms(1000.0);
                etatPresent = eteint;
                break;
        }
        DEBUG_PRINT("Valeur : %d\n", etatPresent);
        boutonsAvant = boutonsConf;
    }
    return 0;
}