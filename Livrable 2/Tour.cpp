/**
* \file   Tour.cpp
* \author Hisham Boulifa, Ismael Adam Soule
* \date   22 avril 2021
*/

#include "Tour.h"

using iter::range;

bool Tour::estUneDestinationValide(const Position& arrivee) const
{
	return (position_.i == arrivee.i || position_.j == arrivee.j) && position_ != arrivee;
}

std::unordered_set<Position, PositionHasher> Tour::obtenirChemin(const Position& arrivee) const
{
	std::unordered_set<Position, PositionHasher> chemin = {};

	int sensD�placementColonne = position_.i - arrivee.i > 0 ? -1 : 1;
	int sensD�placementLigne = position_.j - arrivee.j > 0 ? -1 : 1;

	for (int d : range(1, abs(position_.i - arrivee.i)))
	{
		chemin.insert({ position_.i + d*sensD�placementColonne, position_.j });
	}

	for (int d : range(1, abs(position_.j - arrivee.j)))
	{
		chemin.insert({ position_.i, position_.j + d*sensD�placementLigne });
	}

	return chemin;
}