#pragma once

#include "Piece.h"

class Tour : public Piece {
public:
	Tour(int i, int j, bool estBlanc) : Piece(i, j, estBlanc) {}

	bool estUneDestinationValide(const Position& arrivee) const override;
	std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const override;

	const std::string& lireNom() const override { return nom_; }

private:
	std::string nom_ = "Tour"; // Utile pour l'affichage textuel de l'�tat de la partie.
};