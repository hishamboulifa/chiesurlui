#include "Partie.h"

using iter::range;

// Ajoute une pi�ce sur l'�chiquier.
void Partie::ajouterPiece(std::unique_ptr<Piece> piece)
{
	Position pos = piece->lirePosition();

	std::cout << "\nCr�ation d'un.e " << piece->lireNom() << (piece->estBlanc() ? " blanc.he " : " noir.e ") << "en " << nomsColonnes_[pos.j]  << nomsLignes_[pos.i] << std::endl;

	echiquier_.insert({ pos, std::move(piece) });
}

// V�rifie si le d�placement de la pi�ce se trouvant sur la case de d�part jusqu'� la case d'arriv�e est licite.
bool Partie::estUnMouvementValide(const Position& depart, const Position& arrivee)
{
	bool estUnMouvementValide = true;
	
	if (echiquier_[depart]->estUneDestinationValide(arrivee)) // Si le type de pi�ce peut permettre le d�placement.
	{
		std::unordered_set<Position, PositionHasher> chemin = echiquier_[depart]->obtenirChemin(arrivee);

		// On v�rifie que toutes les cases � traverser sont libres.
		for (auto&& position : chemin)
		{
			if (echiquier_.find(position) != echiquier_.end())
			{
				estUnMouvementValide = false;
				break;
			}
		}

		// Si la case de destination est occup�e par une pi�ce du m�me camp, le mouvement est invalide.
		if (echiquier_.find(arrivee) != echiquier_.end() && echiquier_[depart]->estBlanc() == echiquier_[arrivee]->estBlanc())
		{
			estUnMouvementValide = false;
		}
	}
	else
	{
		estUnMouvementValide = false;
	}
	
	return estUnMouvementValide;
}

// Joue un coup si le d�placement demand� est licite.
void Partie::deplacerPiece(const Position& depart, const Position& arrivee)
{
	if (echiquier_.find(depart) != echiquier_.end())
	{
		if (estUnMouvementValide(depart, arrivee))
		{
			std::cout << "\nD�placement du " << echiquier_[depart]->lireNom() << (echiquier_[depart]->estBlanc() ? " blanc.he " : " noir.e ") << "vers " << nomsColonnes_[arrivee.j] << nomsLignes_[arrivee.i] << std::endl;

			echiquier_[arrivee] = move(echiquier_[depart]);
			echiquier_[arrivee]->ecrirePosition(arrivee);
			echiquier_.erase(depart);
		}
		else
		{
			std::cout << "\nMouvement invalide !" << std::endl;
		}
	}
	else
	{
		std::cout << "\nAucune pi�ce s�lectionn�e !" << std::endl;
	}
}


void Partie::afficher() const
{
	std::cout << "\nEtat de la partie : " << std::endl;
	for (auto&& it : echiquier_)
	{
		std::cout << it.second->lireNom() << (it.second->estBlanc() ? " blanc.he " : " noir.e ") << " " << nomsColonnes_[it.first.j] << nomsLignes_[it.first.i] << std::endl;
	}
}