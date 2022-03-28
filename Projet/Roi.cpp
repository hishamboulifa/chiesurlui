#include "Roi.h"

bool Roi::estUneDestinationValide(const Position& arrivee) const
{
	// Le roi peut se d�placer d'une case dans n'importe quelle direction :
	return abs(position_.i - arrivee.i) < 2 && abs(position_.j - arrivee.j) < 2 && position_!=arrivee;
}

std::unordered_set<Position, PositionHasher> Roi::obtenirChemin(const Position& arrivee) const
{
	// Le roi ne traverse aucune case avant d'arriver � sa destination.

	//Remarque : le param�tre arrivee n'�tant pas utilis�, un avertissement est g�n�r�.
	return {};
}