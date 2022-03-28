#include "Cavalier.h"

using iter::range;

bool Cavalier::estUneDestinationValide(const Position& arrivee) const
{
	// Le cavalier peut se d�placer en L :
	return (abs(position_.i - arrivee.i) == 2 && abs(position_.j - arrivee.j) == 1) || (abs(position_.i - arrivee.i) == 1 && abs(position_.j - arrivee.j) == 2);
}

std::unordered_set<Position, PositionHasher> Cavalier::obtenirChemin(const Position& arrivee) const
{
	// Le cavalier ne traverse aucune case pour arriver � sa destination.

	//Remarque : le param�tre arrivee n'�tant pas utilis�, un avertissement est g�n�r�.
	return {};
}