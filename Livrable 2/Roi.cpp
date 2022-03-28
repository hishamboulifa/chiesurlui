

#include "Roi.h"

bool Roi::estUneDestinationValide(const Position& arrivee) const
{
	return abs(position_.i - arrivee.i) < 2 && abs(position_.j - arrivee.j) < 2 && position_!=arrivee;
}

std::unordered_set<Position, PositionHasher> Roi::obtenirChemin(const Position& arrivee) const
{
	return {};
}