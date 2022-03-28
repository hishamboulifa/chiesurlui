

#pragma once

#include "Partie.h"

#pragma warning(push, 0) 
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLabel>
#pragma pop()

class FenetreJeu : public QMainWindow {
	Q_OBJECT

public:
	FenetreJeu(QWidget* parent = nullptr);
	~FenetreJeu() override = default;

	void ajouterPiece(std::unique_ptr<Piece> piece);

public slots:

	void selectionnerCase(); 
	void mettreAJourAffichage(); 
	void retroactionErreur(int i, int j); 

private:

	QPushButton* nouveauBouton(const QString& text);

	Partie partie_;  

	QPushButton* grille_[8][8]; 
	

	int i_;
	int j_;
};


