/*****************************
TP9
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "debug.h"
#include "bimoteur.h"
#include "matrice.h"
#include "instructions.h"
#include "delcouleur.h"
#include "uart.h"

const uint8_t directions[] = {
        0b001101,
        0b010011,
        0b100101,
        0b010110
};

int main()
{
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRB = 0xFF;

    DELCouleur delCouleur;
    delCouleur.setPort(&PORTB,PORTB0,PORTB1);

    Matrice affichage;
    affichage.setPort(&PORTA, PORTA0, 4);

    Matrice directionAffichage;
    directionAffichage.setPort(&PORTC, PORTC0, 3);
    directionAffichage.setEtat(directions[0]);

    BiMoteur biMoteur;

    initUART();

    bool debut = false;
    uint8_t position = 0;
    uint8_t instruction = 0;
    uint8_t operande = 0;
    uint8_t direction = 0;
    uint8_t adresseBoucle = 0;
    uint8_t nbIteration;            //variable boucle
    uint8_t nbIterationComplete;    //variable boucle

    delCouleur.allumerrouge();

    // Enregistrement du programme
    transmissionUART(0);
    while(instruction != FIN){
        instruction = receptionUART();
        eeprom_write_byte((uint8_t*)position,instruction);
        position++;
        operande = receptionUART();
        eeprom_write_byte((uint8_t*)position,operande);
        position++;
    }
    delCouleur.allumervert();
    _delay_ms(200);
    delCouleur.eteindre();
    _delay_ms(200);
    delCouleur.allumervert();
    _delay_ms(200);
    delCouleur.eteindre();

    position = 0;
    instruction = 0;
    operande = 0;

    while (instruction != FIN)
    {
        instruction = eeprom_read_byte((const uint8_t *)position);
        position++;
        operande = eeprom_read_byte((const uint8_t *)position);
        position++;
        if (debut == false)
        {
            if (instruction == DBT)
                {
                    debut = true;
                }
        }
        else
        {
            switch (instruction)
            {
            case MON:
                affichage.setEtat(operande);
                break;
            case MOF:
                affichage.setEtat(0);
                break;
            case ATT: 
                for (uint16_t i = 0; i < operande; i++) {
                        _delay_ms(25);
                }
                break;
            case DAL:
                if (operande <= 127)
                {
                    delCouleur.allumervert();
                }
                else
                {
                    delCouleur.allumerrouge();
                }
                break;
            case DET:
                delCouleur.eteindre();
                break;
            case MAR: //moteur.ajustementMoteur(0, 0);       
                biMoteur.ajustementGauche(0,BiMoteur::avancer);
                biMoteur.ajustementDroite(0,BiMoteur::avancer);         
                break;
            case MAV: //vitesse (vers l'avant) = operande
                biMoteur.ajustementGauche(operande,BiMoteur::avancer);
                biMoteur.ajustementDroite(operande,BiMoteur::avancer);
                break;
            case MRE: //vitesse (vers l'arriere) = operande
                biMoteur.ajustementGauche(operande,BiMoteur::reculer);
                biMoteur.ajustementDroite(operande,BiMoteur::reculer);
                break;
            case TRD:   
                biMoteur.ajustementGauche(0,BiMoteur::avancer);
                biMoteur.ajustementDroite(127,BiMoteur::avancer);
                direction = (direction + 1) % 4;
                directionAffichage.setEtat(directions[direction]);
                _delay_ms(1000);
                biMoteur.ajustementGauche(0,BiMoteur::avancer);
                biMoteur.ajustementDroite(0,BiMoteur::avancer);
                break;
            case TRG:   
                biMoteur.ajustementGauche(127,BiMoteur::avancer);
                biMoteur.ajustementDroite(0,BiMoteur::avancer);
                direction = (direction + 3) % 4;
                directionAffichage.setEtat(directions[direction]);
                _delay_ms(1000);
                biMoteur.ajustementGauche(0,BiMoteur::avancer);
                biMoteur.ajustementDroite(0,BiMoteur::avancer);
                break;
            case DBC:
                adresseBoucle = position;         //enregistre adresse debut de boucle
                nbIteration = operande + 1;     //enregistre nb d'iteration à faire
                nbIterationComplete = 0;    //enregistre nb d'iteration complete
                break;
            case FBC:
                nbIterationComplete ++;                     //incremente nb iteration complete
                if(nbIterationComplete < nbIteration) {
                    position = adresseBoucle;                     //revient à l'adresse debut de boucle si 
                }                                           //on n'a pas fait le nb d'iterations necessaires
                break;
            case FIN:
                break;
            default:
                break;
            }
        }
    }
    delCouleur.eteindre();
    biMoteur.ajustementGauche(0,BiMoteur::avancer);
    biMoteur.ajustementDroite(0,BiMoteur::avancer); 
    return 0;
}