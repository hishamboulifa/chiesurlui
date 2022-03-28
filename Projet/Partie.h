#pragma once

#include <unordered_map>
#include <memory>

#include "Roi.h"
#include "Tour.h"
#include "Cavalier.h"

// Classe repr�sentant l'�tat de la partie.
class Partie {
public:
	Partie(){};

	void ajouterPiece(std::unique_ptr<Piece> piece);
	bool estUnMouvementValide(const Position& depart, const Position& arrivee);
	void deplacerPiece(const Position& depart, const Position& arrivee);

	void afficher() const;

private :
	// Le plateau de jeu est repr�sent� par une map o� les cl�s sont les positions et les pi�ces les valeurs.
	std::unordered_map<Position, std::unique_ptr<Piece>, PositionHasher> echiquier_;

	// Utile pour l'affichage textuel de l'�tat de la partie.
	std::string nomsColonnes_[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	std::string nomsLignes_[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };
};