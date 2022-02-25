/*****************************
Projet final : Clavier3x3
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#include "clavier3x3.h"

//configurer les ports
void Clavier3x3::initialiser(){
    DDRC |= 0b111 << DDC5;
    PORTC |= 0b111 << PORTC5;
    DDRC &= ~(0b111 << DDC2);
}


uint8_t Clavier3x3::lire(){
    // lire la pin et déterminer la colonnes
    uint8_t lecture = (PINC & (0b111 << PINC2)) >> PINC2;
    if(!lecture){
        return 0;
    }
    uint8_t position = 4;
    while(lecture){
        lecture >>= 1;
        position--;
    }

    // inverser les entrées et les sorties
    DDRC |= 0b111 << DDC2;
    PORTC |= 0b111 << PORTC2;
    PORTC &= ~(0b111 << PORTC5);
    DDRC &= ~(0b111 << DDC5);

    _delay_ms(10.0); // debouncing


    // lire la pin et déterminer la case exacte
    lecture = (PINC >> PINC5) & 0b111;

    DDRC |= 0b111 << DDC5;
    PORTC |= 0b111 << PORTC5;
    PORTC &= ~(0b111 << PORTC2);
    DDRC &= ~(0b111 << DDC2);

    if(!lecture){
        return 0;
    }
    // calculer le numéro de la case
    lecture >>= 1;
    while(lecture){
        lecture >>= 1;
        position+=3;
    }

    return position;
}