
#pragma once

#pragma warning(push, 0) 
#include <QObject>
#pragma pop()
#include <functional>

#include <unordered_map>
#include <memory>

#include "Roi.h"
#include "Tour.h"
#include "Cavalier.h"

class Partie : public QObject {
	Q_OBJECT
public:
	Partie(){};

	
	void ajouterPiece(std::unique_ptr<Piece> piece);
	void deplacerPiece(const Position& depart, const Position& arrivee);

	bool estEnEchec(bool estBlanc) const;
	bool estUnMouvementValide(const Position& depart, const Position& arrivee) const;

	void afficher() const;
	std::string lireContenuCase(int i, int j) const;

public slots:

	void caseSelectionnee(const Position& selection);

signals:

	void pieceDeplacee(); 
	void mouvementInvalide(int i, int j); 

private :
	
	std::unordered_map<Position, std::unique_ptr<Piece>, PositionHasher> echiquier_;

	bool estTourBlanc = true; 

	Position* selection_ = nullptr;

	std::string nomsColonnes_[8] = { "A", "B", "C", "D", "E", "F", "G", "H" };
	std::string nomsLignes_[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
};