#define F_CPU 8000000

#include <avr/io.h> 
#include "bimoteur.h" 

int main(){
    DDRD = 0xFF;

    BiMoteur biMoteur;
    biMoteur.ajustementGauche(127,BiMoteur::avancer);
    biMoteur.ajustementDroite(127,BiMoteur::avancer);


    for(;;){}

    return 0;
}