/*****************************
Librairie
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

class SeptSegment
{
public:
    void afficher1(uint8_t af);
    void afficher2(uint8_t af);
    void afficher3(uint8_t af);
    void afficher4(uint8_t af);
    void desactiver();
};
