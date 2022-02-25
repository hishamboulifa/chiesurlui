/*****************************
TP7 - Démonstration librairie
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/
#define F_CPU 8000000

#include "moteur.h"
#include "del.h"
#include "uart.h"
//#include "minuterie.h"
#include "bouton.h"

#include <util/delay.h>
#include <avr/interrupt.h>

char salut[] = "Salut!\n";

volatile uint8_t minuterieExpiree = 0;

ISR(TIMER1_COMPA_vect)
{
    minuterieExpiree = 1;
}

int main(){
    //cli();
    DDRA = 0xff;
    DDRD = 0xFF;
    DDRC = 0x00;

    Moteur moteur;
    uint8_t vitesse = 0;

    DEL dels [3] = {};

    dels[0].setPort(&PORTA,PORTA0);
    dels[1].setPort(&PORTA,PORTA1);
    dels[2].setPort(&PORTA,PORTA2);

    dels[0].allumer();

    Bouton bouton;
    bouton.setPort(&PINC,PORTC2);
    
    initUART();
    transmissionPhrase(salut, sizeof(salut));

    // minuterie incompatible avec minuterie
    //cli();
    //Minuterie minuterie;
    //sei();

    //minuterie.partirMinuterie(2);

    uint8_t i = 0;
    for(;;){
        if(i == 0){
            moteur.ajustementMoteur(vitesse,Moteur::antihoraire);
            dels[0].inverser();
            dels[1].inverser();
            vitesse+= 0xF;
        }
        
        
        
        //if(minuterieExpiree)
        //    dels[1].allumer();
        if(bouton.click())
            dels[2].inverser();
        _delay_ms(2);
        i++;
    }
    return 0;
}