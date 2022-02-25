/*****************************
Projet final : SeptSegment
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#define F_CPU 8000000

#include <util/delay.h>
#include "septSegment.h"

void SeptSegment::afficher1(uint8_t af){
    PORTB &= ~ (1 << PORTB7);
    PORTB |= (1 << PORTB5);
    PORTA = (af << 4);
    _delay_ms(10);
    PORTB &= ~ (1 << PORTB5);
}
void SeptSegment::afficher2(uint8_t af){
    PORTB &= ~ (1 << PORTB7);
    PORTB |= (1 << PORTB6);
    PORTA = (af << 4);
    _delay_ms(10);
    PORTB &= ~ (1 << PORTB6);
}
void SeptSegment::afficher3(uint8_t af){
    PORTD &= ~ (1 << PORTD7);
    PORTC |= (1 << PORTC0);
    PORTA = (af << 4);
    _delay_ms(10);
    PORTC &= ~ (1 << PORTC0);
}
void SeptSegment::afficher4(uint8_t af){
    PORTD &= ~ (1 << PORTD7);
    PORTC |= (1 << PORTC1);
    PORTA = (af << 4);
    _delay_ms(10);
    PORTC &= ~ (1 << PORTC1);
}
void SeptSegment::desactiver(){
    PORTB |= (1 << PORTB7);
    PORTD |= (1 << PORTD7);
}