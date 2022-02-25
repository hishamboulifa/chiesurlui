/*****************************
Projet final : main
Auteurs:
 - Hisham Boulifa    (2085232)
 - Nicolas Deloumeau (2086143)
 - Laurent Langlois  (2075635)
 - Thomas Duperron   (2016459)
*****************************/

#define F_CPU 8000000

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "uart.h"
#include "bimoteur.h"
#include "clavier3x3.h"
#include "septSegment.h"
#include "debug.h"
#include "can.h"

enum Affichage
{
    modeR,
    modeV,
    modeC
};
volatile Affichage affichage = modeR;
enum Convertisseur
{
    I,
    E
};
enum Categorie
{
    OK = 0,
    ATTENTION = 1,
    DANGER = 2
};
char categorieTexte[3][10] = {
    "OK",
    "ATTENTION",
    "DANGER"};
char manoeuvreChar[6] = {'-', '1', '2', '3', '4', '5'};
volatile Convertisseur conv = I;
volatile uint8_t vitesseDetection = 1;
volatile bool boutonappuye = false;
SeptSegment septSegment;
uint16_t puissanceD = 0;
uint16_t puissanceG = 0;
char texteFormate[128];
char manoeuvreTexte[128];
char actionTexte[128];
volatile bool afficherhex = false;
volatile float temps = 0.0f;
volatile char actionClavier = ' ';

void initialisation()
{
    cli();
    //EIMSK |= (1 << INT0) | (1 << INT1);
    //EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (0b111111 << PCINT18);
    EIMSK |= (1 << INT0);
    EICRA |= (0 << ISC00) | (1 << ISC01);

    // Timers
    
    TIMSK0 = (1 << OCIE1A); 
    TCCR0B = (1<<CS00) | (1<<CS02); // freq/1024
    sei();
    TCNT0 = 0;
    OCR1A = 127;
}

ISR(INT0_vect)
{
    _delay_ms(30);
    boutonappuye = true;
    EIFR |= (1 << INTF0);
}

ISR(TIMER0_COMPA_vect)
{
    temps += 0.0326f;
}

ISR(PCINT2_vect)
{
    switch (Clavier3x3::lire())
    {
    case 1:
        vitesseDetection = 1;
        actionClavier = '1';
        break;
    case 2:
        vitesseDetection = 2;
        actionClavier = '2';
        break;
    case 3:
        vitesseDetection = 4;
        actionClavier = '4';
        break;
    case 4:
        affichage = modeR;
        actionClavier = 'R';
        break;
    case 5:
        affichage = modeV;
        actionClavier = 'V';
        break;
    case 6:
        affichage = modeC;
        actionClavier = 'C';
        break;
    case 7:
        conv = I;
        actionClavier = 'I';
        break;
    case 8:
        conv = E;
        actionClavier = 'E';
        break;
    case 9:
        afficherhex = !afficherhex;
        actionClavier = '#';
        break;
    default:
        break;
    }
    PCIFR |= (1 << PCIF2);
}
// fonctions

int main()
{
    DDRA = 0b11111101;
    DDRB = 0b11111100;
    DDRC = 0b00000011; // modifié par le claviers
    DDRD = 0b11111011;
    initialisation();

    uint8_t manoeuvre = 0;
    uint16_t puissanceD = 0;
    uint16_t puissanceG = 0;
    char manoeuvre0[] = "Combinaison non evaluee\n";
    char bps[] = "9600 bps\n";

    BiMoteur biMoteur;
    can convertisseur;

    SeptSegment septSegment;

    Clavier3x3::initialiser();

    //Demarrage
    transmissionPhrase(bps, 10);
    septSegment.afficher1(10); //10ms afficher A
    septSegment.afficher2(11); //10ms afficher B
    septSegment.afficher3(12); //10ms afficher C 
    septSegment.afficher4(13); //10ms afficher D
    _delay_ms(1960);
    septSegment.desactiver();
    biMoteur.ajustementDroite(255, BiMoteur::avancer);
    biMoteur.ajustementGauche(255, BiMoteur::avancer);
    _delay_ms(1000);
    biMoteur.ajustementDroite(255, BiMoteur::reculer);
    biMoteur.ajustementGauche(255, BiMoteur::reculer);
    _delay_ms(1000);
    biMoteur.ajustementDroite(0, BiMoteur::avancer);
    biMoteur.ajustementGauche(0, BiMoteur::avancer);

    for (;;)
    {
        biMoteur.ajustementDroite(0, BiMoteur::avancer);
        biMoteur.ajustementGauche(0, BiMoteur::avancer);
        bool afficherDistances = false;
        septSegment.desactiver();
        float distances[3];
        Categorie categories[3];
        float tempdistances[3]; //etat précédent
        Categorie tempCategories[3]; //etat précédent
        for (uint8_t i = 0; i < 3; i++)
        {
            tempdistances[i] = distances[i]; //initialisation des état précedents de distances et catégorie
            tempCategories[i] = categories[i];
        }
        float lectureAnalogue = 0;
        uint8_t lectureExterne = 0;
        for (uint8_t i = 0; i < 3; i++)
        {
            lectureExterne = 0; //reinitialisation de la valeur de sortie du CAN externe pour chaque capteur
            PORTA &= ~(0b11 << PORTA2);
            PORTA |= (i << PORTA2);
            _delay_ms(10);
            if (conv == I)
            {
                PORTA &= ~(1 << PORTA0);
                lectureAnalogue = convertisseur.lecture(PORTA1) * 5.0f / 1024.0f; //Convertir CAN interne en V
            }
            else if (conv == E)
            {
                PORTA |= (1 << PORTA0);
                for (uint8_t i = 0; i < 8; i++)
                {
                    PORTB &= ~(0b111 << PORTB2);
                    PORTB |= (i << PORTB2); //MUX pour CAN bit par bit
                    _delay_ms(10); //delay pour stabiliser les pins
                    uint8_t valeurPin = ((PINB & (1 << PINB1)) >> PINB1);
                    lectureExterne |= (valeurPin << (7-i));//inverser les bit puisque D0 est connecté a D7
                }
                lectureAnalogue = lectureExterne * 5.0f / 256.0f; //Convertir CAN externe en V
            }

            distances[i] = 29.1956f / powf(lectureAnalogue, 1.14184f) - 0.2063f;
            if (distances[i] < 20)  //affectation des catégories par rapport a la valeur de distance
            {
                categories[i] = DANGER;
            }
            else if (distances[i] < 50)
            {
                categories[i] = ATTENTION;
            }
            else
            {
                categories[i] = OK;
            }
        }
        if (categories[0] == OK && categories[1] == ATTENTION && categories[2] == ATTENTION)    //affectation de la valeur de manoeuvre par rapport aux categories
        {
            manoeuvre=1;
        }else if (categories[0] == ATTENTION && categories[1] == ATTENTION && categories[2] == OK)
        {
            manoeuvre=2;
        }else if (categories[0] == DANGER && categories[1] == DANGER && categories[2] == DANGER)
        {
            manoeuvre=3;
        }else if (categories[0] == OK && categories[1] == OK && categories[2] == DANGER)
        {
            manoeuvre=4;
        }else if (categories[0] == DANGER && categories[1] == OK && categories[2] == OK)
        {
            manoeuvre=5;
        }else
        {
            manoeuvre=0; //si combinaison non reconnue
        }
        
        switch (affichage)
        {
        case modeR:
            afficherDistances=true;
            break;
        
        case modeV:
            for (uint8_t i = 0; i < 3; i++)
            {
                if (distances[i] != tempdistances[i])//verification par rapport a l'état précédent
                {
                    afficherDistances = true;
                    break;
                }
            }
            break;
        
        case modeC:
            for (uint8_t i = 0; i < 3; i++)
            {
                if (categories[i] != tempCategories[i])//verification par rapport a l'état précédent
                {
                    afficherDistances = true;
                    break;
                }
            }
            break;
        
        default:
            break;
        }
        if (afficherDistances)
        {
            uint8_t longueur = sprintf(texteFormate, "%02d:%02d.%02d - G%d.%d C%d.%d D%d.%d - %s | %s | %s (%c)\n",
                                       (uint8_t)(temps/60),
                                       (uint8_t)(temps)%60,
                                       (uint8_t)(temps*100)%100,
                                       (uint8_t)(distances[0]), (uint8_t)(distances[0] * 10) % 10,
                                       (uint8_t)(distances[1]), (uint8_t)(distances[1] * 10) % 10,
                                       (uint8_t)(distances[2]), (uint8_t)(distances[2] * 10) % 10,
                                       categorieTexte[categories[0]],
                                       categorieTexte[categories[1]],
                                       categorieTexte[categories[2]],
                                       manoeuvreChar[manoeuvre]);
            transmissionPhrase(texteFormate, longueur);
        }
        switch (vitesseDetection) //delays pour le nombre de vérification par secondes
        {
        case 1:
            _delay_ms(1000);
            break;
        case 2:
            _delay_ms(500);
            break;
        case 4:
            _delay_ms(250);
            break;
        default:
            break;
        }

        if (boutonappuye)
        {

            uint8_t longueurTexte = sprintf(manoeuvreTexte, "Manoeuvre %d\n", manoeuvre); //affichage de la manoeuvre
            transmissionPhrase(manoeuvreTexte, longueurTexte);
            switch (manoeuvre)
            {
            case 0: //Combinaison non reconnue
                transmissionPhrase(manoeuvre0, 27);
                _delay_ms(2000);
                break;
            case 1: //Manoeuvre 1
                puissanceD = 35;
                puissanceG = 35;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::reculer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms 
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                _delay_ms(1460);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(2000);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::reculer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(1500);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(125);
                for (size_t i = 0; i < 12; i++)
                {
                    puissanceD += 5;
                    puissanceG += 5;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                    _delay_ms(85);
                }
                _delay_ms(2000);
                break;
            case 2: //Manoeuvre 2
                puissanceD = 35;
                puissanceG = 35;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::reculer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                _delay_ms(1460);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(2000);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::reculer);
                _delay_ms(1500);
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(125);
                for (size_t i = 0; i < 12; i++)
                {
                    puissanceD += 5;
                    puissanceG += 5;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                    _delay_ms(85);
                }
                _delay_ms(2000);
                break;
            case 3: //Manoeuvre 3
                puissanceD = 50;
                puissanceG = 50;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::reculer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::reculer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                _delay_ms(960);
                puissanceD = 70;
                puissanceG = 70;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::reculer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                _delay_ms(1460);
                puissanceD = 0;
                puissanceG = 0;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                _delay_ms(50);
                for (size_t i = 0; i < 33; i++)
                {
                    puissanceD += 3;
                    puissanceG += 3;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                    _delay_ms(10);
                }
                _delay_ms(500);
                for (size_t i = 0; i < 5; i++)
                {
                    puissanceD -= 5;
                    puissanceG -= 5;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                    _delay_ms(460); //vérifier si ce délai est une fois de trop
                }
                _delay_ms(2000);
                break;
            case 4: //Manoeuvre 4
                puissanceD = 78;
                puissanceG = 78;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                for (size_t i = 0; i < 15; i++)
                {
                    _delay_ms(250);
                    puissanceG -= 2;
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                    }
                }
                _delay_ms(1500);
                for (size_t i = 0; i < 15; i++)
                {
                    _delay_ms(250);
                    puissanceG += 2;
                    biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 10); //10ms
                        septSegment.afficher2(puissanceG % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher1(puissanceG / 16); //10ms
                        septSegment.afficher2(puissanceG % 16); //10ms
                    }
                }
                _delay_ms(2000);
                break;
            case 5: //Manoeuvre 5
                puissanceD = 78;
                puissanceG = 78;
                biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                biMoteur.ajustementGauche(255 * puissanceG / 100, BiMoteur::avancer);
                if (!afficherhex)
                {
                    septSegment.afficher1(puissanceG / 10); //10ms
                    septSegment.afficher2(puissanceG % 10); //10ms
                    septSegment.afficher3(puissanceD / 10); //10ms
                    septSegment.afficher4(puissanceD % 10); //10ms
                }
                else if (afficherhex)
                {
                    septSegment.afficher1(puissanceG / 16); //10ms
                    septSegment.afficher2(puissanceG % 16); //10ms
                    septSegment.afficher3(puissanceD / 16); //10ms
                    septSegment.afficher4(puissanceD % 16); //10ms
                }
                for (size_t i = 0; i < 15; i++)
                {
                    _delay_ms(250);
                    puissanceD -= 2;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                }
                _delay_ms(1500);
                for (size_t i = 0; i < 15; i++)
                {
                    _delay_ms(250);
                    puissanceD += 2;
                    biMoteur.ajustementDroite(255 * puissanceD / 100, BiMoteur::avancer);
                    if (!afficherhex)
                    {
                        septSegment.afficher3(puissanceD / 10); //10ms
                        septSegment.afficher4(puissanceD % 10); //10ms
                    }
                    else if (afficherhex)
                    {
                        septSegment.afficher3(puissanceD / 16); //10ms
                        septSegment.afficher4(puissanceD % 16); //10ms
                    }
                }
                _delay_ms(2000);
                break;

            default:
                break;
            }
            boutonappuye = false;
        }
        if (actionClavier != ' ')
        {
            uint8_t longueurAction = sprintf(actionTexte, "Le bouton %c du clavier a ete appuye\n", actionClavier); //Affichage des boutons appuyés
            transmissionPhrase(actionTexte, longueurAction);
            actionClavier = ' ';
        }
        

    }

    return 0;
}