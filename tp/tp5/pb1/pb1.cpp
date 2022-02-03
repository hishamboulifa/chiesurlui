#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

int main()
{
    bool identique= false;
    DDRA = 0xff;
    char poly[] = "POLYTECHNIQUE MONTREAL";
    char testread[23] = {};
    eeprom_write_block(poly, 0x00, sizeof(poly) / sizeof(poly[0]));
    eeprom_read_block(testread, 0x00, sizeof(poly) / sizeof(poly[0]));
    for (size_t i = 0; i < sizeof(poly); i++)
    {
        if (poly[i] == testread[i])
        {
            identique = true;
        }else
        {
            identique = false;
            break;
        }
        
    }
    
    if (identique)
    {
        PORTA = 1;
    }
    else 
    {
        PORTA = 2;
    }

    return 0;
}