#pragma once

#include <cstdlib>
#include <unordered_set>
#include <string>

#include "Position.h"

// Classe virtuelle repr�sentant une pi�ce du jeu d'�checs.
class Piece {
public :
	Piece(int i, int j, bool estBlanc) : position_({ i, j }), estBlanc_(estBlanc) {}
	Piece(const Position& position, bool estBlanc) : position_(position), estBlanc_(estBlanc) {}

	virtual ~Piece() = default;

	// Accesseurs :
	Position lirePosition() const { return position_; }
	void ecrirePosition(const Position& position) { position_ = position; }
	virtual const std::string& lireNom() const = 0;

	bool estBlanc() const { return estBlanc_; }

	// Indique si le d�placement de la pi�ce peut lui permettre de se rendre sur la position vis�e (sans tenir compte des autres pi�ces).
	virtual bool estUneDestinationValide(const Position& arrivee) const = 0;

	// Indique les positions que la pi�ce devrait traverser pour atteindre la destination (excluant les cases de d�part et d'arriv�e).
	virtual std::unordered_set<Position, PositionHasher> obtenirChemin(const Position& arrivee) const = 0;

protected :
	Position position_; // Position de la pi�ce sur l'�chiquier.
	bool estBlanc_; // Indique le camp auquel appartient la pi�ce.
};