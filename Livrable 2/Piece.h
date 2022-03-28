

#pragma once

#include <cstdlib>
#include <unordered_set>
#include <string>

#include "Position.h"

class Piece {
public :
	Piece(int i, int j, bool estBlanc) : position_({ i, j }), estBlanc_(estBlanc) {}
	Piece(const Position& position, bool estBlanc) : position_(position), estBlanc_(estBlanc) {}

	virtual ~Piece() = default;

	Position lirePosition() const { return position_; }
	void ecrirePosition(const Position& position) { position_ = position; }
	virtual const std::string& lireNom() const = 0;

	bool estBlanc() const { return estBlanc_; }

	virtual bool estUneDestinationValide(const Position& arrivee) const = 0;

	virtual std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const = 0;

protected :
	Position position_; 
	bool estBlanc_; 
};