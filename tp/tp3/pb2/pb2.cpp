#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>

void delayms(uint16_t);
void delayus(uint16_t);


int main()
{
    DDRB = 0b00000011; // DDB0 et DDB1 a 1
    uint8_t b = 16;
    uint16_t b2 = 2500;
    uint8_t dureeMs = 120;
    uint16_t dureeUs = 800;
    for (;;) // boucle sans fin
    {
        for (uint16_t a = 16; a > 0; a-=4)
        {
            for (uint8_t i = 0; i < dureeMs; i++)
            {
                PORTB = 1;
                delayms(a);
                PORTB = 0;
                delayms(b-a);
            }
            
        }
        PORTB = 0;
        _delay_ms(2000);

        for (uint16_t a = 2500; a > 0; a-=625)
        {
            for (uint16_t i = 0; i < dureeUs; i++)
            {
                PORTB = 1;
                delayus(a);
                PORTB = 0;
                delayus(b2-a);
            }
            
        }
        PORTB = 0;
        _delay_ms(2000);
    }
    return 0;
}

void delayms(uint16_t ms)
{
    while (0 < ms)
    {
        _delay_ms(1);
        ms-=1;
    }
}

void delayus(uint16_t us)
{
    while (0 < us)
    {
        _delay_us(625);
        us-=625;
    }
}

// 60 hz = 0.016666- s = 16.666- ms
// 400 hz = 0.0025 s = 2500 us 