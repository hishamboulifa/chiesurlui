#include "Tour.h"

using iter::range;

bool Tour::estUneDestinationValide(const Position& arrivee) const
{
	// La tour peut se d�placer le long d'une ligne ou d'une colonne :
	return (position_.i == arrivee.i || position_.j == arrivee.j) && position_ != arrivee;
}

std::unordered_set<Position, PositionHasher> Tour::obtenirChemin(const Position& arrivee) const
{
	std::unordered_set<Position, PositionHasher> chemin = {};

	int sensD�placementColonne = position_.i - arrivee.i > 0 ? -1 : 1;
	int sensD�placementLigne = position_.j - arrivee.j > 0 ? -1 : 1;

	// Si la tour se d�place le long d'une colonne :
	for (int d : range(1, abs(position_.i - arrivee.i)))
	{
		chemin.insert({ position_.i + d*sensD�placementColonne, position_.j });
	}

	// Si la tour se d�place le long d'une ligne :
	for (int d : range(1, abs(position_.j - arrivee.j)))
	{
		chemin.insert({ position_.i, position_.j + d*sensD�placementLigne });
	}

	return chemin;
}