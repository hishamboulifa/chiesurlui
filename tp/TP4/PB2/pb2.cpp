/// \Travail : probleme 2 du TP2
/// \auteurs : Laurent Langlois (2075635), Thomas Duperron (2016459)
/// \Section # : 5
/// \Equipe # : 204
/// \correcteur : Eya-Tom Augustin Sangam, Tristan Rioux

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

ISR(INT0_vect)
{
    boutonPoussoir = 1;
}

ISR(TIMER1_COMPA_vect){
    minuterieExpiree = 1;
}

void partirMinuterie ( uint16_t duree ) {

minuterieExpiree = 0;

// mode CTC du timer 1 avec horloge divisée par 1024

// interruption après la durée spécifiée

TCNT1 = 0x0000 ;

OCR1A = duree;

TCCR1A = 0 ;

TCCR1B = 0b00001101 ;

TCCR1C = 0;

TIMSK1 = 0b00000010 ;

}

void initialisation(void)
{
    cli();
    DDRA = 0xff;
    DDRD = 0x00;
    EIMSK |= (1 << INT0);
    EICRA |= 0b01;
    sei();
}

int main()
{
    initialisation();
    _delay_ms(10000);
    PORTA=2;
    _delay_ms(100);
    PORTA=0;
    partirMinuterie(0x1E84);
    boutonPoussoir = 0;
    do
    {
        
    } while (minuterieExpiree == 0 && boutonPoussoir == 0);
    cli();
    if (boutonPoussoir == 1)
    {
        PORTA = 1;
    }else if (minuterieExpiree == 1)
    {
        PORTA = 2;
    }
    
    
    return 0;
}
