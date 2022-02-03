#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void delayus(uint16_t);

int main()
{
    DDRA = 0xff;
    uint16_t b = 1000;
    uint8_t periode = 150;
    uint8_t delay = 50;
    for (;;) // boucle sans fin
    {
        PORTA = 2;
        _delay_ms(150);
        for (uint16_t a = 900; a > 0; a-=delay)
        {
            for (uint8_t i = 0; i < periode; i++)
            {
                PORTA = 2;
                delayus(a);
                PORTA = 0;
                delayus(b-a);
            }
            
        }
        PORTA = 0;
        _delay_ms(150);
        PORTA = 1;
        _delay_ms(150);
        for (uint16_t a = 900; a > 0; a-=delay)
        {
            for (uint8_t i = 0; i < periode; i++)
            {
                PORTA = 1;
                delayus(a);
                PORTA = 0;
                delayus(b-a);
            }
            
        }
        PORTA = 0;
        _delay_ms(150);
    }

    return 0;
}

void delayus(uint16_t us)
{
    while (0 < us)
    {
        _delay_us(50);
        us-=50;
    }
}