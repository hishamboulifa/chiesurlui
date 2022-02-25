/*****************************
Librairie
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#include <avr/io.h>
#include <util/delay.h>

class Clavier3x3
{
public:
    static void initialiser();
    static uint8_t lire();
};