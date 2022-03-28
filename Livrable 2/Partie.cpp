

#include "Partie.h"

using iter::range;

void Partie::ajouterPiece(std::unique_ptr<Piece> piece)
{
	Position pos = piece->lirePosition();

	std::cout << "\nCr�ation d'un.e " << piece->lireNom() << (piece->estBlanc() ? " blanc.he " : " noir.e ") << "en " << nomsColonnes_[pos.j]  << nomsLignes_[pos.i] << std::endl;

	echiquier_.insert({ pos, std::move(piece) });
}

bool Partie::estEnEchec(bool estBlanc) const
{
	bool estEnEchec = false;

	bool roiTrouv� = false;
	Position positionRoi = { -1, -1 };
	for (auto&& it : echiquier_)
	{
		if (dynamic_cast<Roi*>(it.second.get()) != nullptr && it.second->estBlanc() == estBlanc)
		{
			roiTrouv� = true;
			positionRoi = it.second->lirePosition();
			break;
		}
	}

	if (roiTrouv�)
	{
		for (auto&& it : echiquier_)
		{
			if(estUnMouvementValide(it.first, positionRoi) && it.second->estBlanc() != estBlanc)
			{
				estEnEchec = true;
				break;
			}
		}
	}
	return estEnEchec;
}

bool Partie::estUnMouvementValide(const Position& depart, const Position& arrivee) const
{
	bool estUnMouvementValide = true;
	
	if (echiquier_.at(depart)->estUneDestinationValide(arrivee)) 
	{
		std::unordered_set<Position, PositionHasher> chemin = echiquier_.at(depart)->obtenirChemin(arrivee);

		for (auto&& position : chemin)
		{
			if (echiquier_.find(position) != echiquier_.end())
			{
				estUnMouvementValide = false;
				break;
			}
		}

		if (echiquier_.find(arrivee) != echiquier_.end() && echiquier_.at(depart)->estBlanc() == echiquier_.at(arrivee)->estBlanc())
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

void Partie::deplacerPiece(const Position& depart, const Position& arrivee)
{
	if (echiquier_.find(depart) != echiquier_.end())
	{
		if (echiquier_[depart]->estBlanc() == estTourBlanc)
		{
			if (estUnMouvementValide(depart, arrivee))
			{
				bool piecePrise = echiquier_.find(arrivee) != echiquier_.end();
				std::unique_ptr<Piece> detruite = nullptr;
				if (piecePrise)
				{
					detruite = move(echiquier_[arrivee]);
				}

				echiquier_[arrivee] = move(echiquier_[depart]);
				echiquier_.erase(depart);
				echiquier_[arrivee]->ecrirePosition(arrivee);

				if (estEnEchec(estTourBlanc))
				{
					std::cout << "\nEchec !" << std::endl;

					echiquier_[depart] = move(echiquier_[arrivee]);
					echiquier_.erase(arrivee);
					echiquier_[depart]->ecrirePosition(depart);
					if (piecePrise)
					{
						echiquier_[arrivee] = move(detruite);
					}
					afficher();

					emit mouvementInvalide(arrivee.i, arrivee.j); 
				}
				else
				{
					estTourBlanc = !estTourBlanc;
					emit pieceDeplacee();

					std::cout << "\nDeplacement du " << echiquier_[arrivee]->lireNom() << (echiquier_[arrivee]->estBlanc() ? " blanc.he " : " noir.e ") << "vers " << nomsColonnes_[arrivee.j] << nomsLignes_[arrivee.i] << std::endl;
				}
			}
			else
			{
				std::cout << "\nMouvement invalide !" << std::endl;
				emit mouvementInvalide(arrivee.i, arrivee.j);
			}
		}
		else
		{
			std::cout << "\nCe n'est pas votre tour !" << std::endl;
			emit mouvementInvalide(arrivee.i, arrivee.j);
		}
	}
	else
	{
		std::cout << "\nAucune piece selectionnee !" << std::endl;
		emit mouvementInvalide(arrivee.i, arrivee.j);
	}
}


void Partie::afficher() const
{
	std::cout << "\nTOUR DES " << (estTourBlanc ? "BLANCS" : "NOIRS") << std::endl;
	for (auto&& it : echiquier_)
	{
		std::cout << it.second->lireNom() << (it.second->estBlanc() ? " blanc.he " : " noir.e ") << " " << nomsColonnes_[it.first.j] << nomsLignes_[it.first.i] << std::endl;
	}
}

std::string Partie::lireContenuCase(int i, int j) const
{
	Position position = { i, j };
	std::string nomPiece = "";
	
	if (echiquier_.find(position) != echiquier_.end())
	{
		nomPiece = echiquier_.at(position)->lireNom() + (echiquier_.at(position)->estBlanc() ? "blanc" : "noir");
	}
	return nomPiece;
}


void Partie::caseSelectionnee(const Position& selection)
{
	if (selection_ == nullptr)
	{
		selection_ = new Position(selection);
	}
	else
	{
		deplacerPiece(*selection_, selection);
		delete(selection_); 
		selection_ = nullptr;
	}
}