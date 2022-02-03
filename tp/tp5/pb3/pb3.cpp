#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

void initialisationUART(void)
{

    // 2400 bauds. Nous vous donnons la valeur des deux
    // premier registres pour vous éviter des complications

    UBRR0H = 0;
    UBRR0L = 0xCF;

    // permettre la réception et la transmission par le UART0

    UCSR0A = 0;

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    //modifie  receive,transfer enabled

    // Format des trames: 8 bits, 1 stop bits, none parity

    UCSR0C |= (1 << UCSZ11) | (1 << UCSZ10); //char size
}

void transmissionUART(uint8_t donnee)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
    /* Copy 9th bit to TXB8 */
    UCSR0B &= ~(1 << TXB80);

    if (donnee & 0x0100)
    {
        UCSR0B |= (1 << TXB80);
    }
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

int main()
{
    DDRD = 0xff;

    initialisationUART();

    DDRA = 0xff;
    char poly[] = "POLYTECHNIQUE MONTREAL";
    char read[23] = {};
    eeprom_write_block(poly, 0x00, sizeof(poly) / sizeof(poly[0]));
    eeprom_read_block(read, 0x00, sizeof(poly) / sizeof(poly[0]));
    for (size_t i = 0; i < sizeof(read); i++)
    {
        transmissionUART(read[i]);
    }
    

    return 0;
}