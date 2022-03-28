

//#include "Calc.hpp"
//#include "CalcWindow.hpp"
#include "FenetreJeu.h"

#include <QApplication>

#if __has_include("bibliotheque_cours.hpp")
#include "bibliotheque_cours.hpp"
#define BIBLIOTHEQUE_COURS_INCLUS
using bibliotheque_cours::cdbg;
#else
auto& cdbg = clog;
#endif

#if __has_include("verification_allocation.hpp")
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  
#endif

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  
	bibliotheque_cours::executerGoogleTest(argc, argv); 
#endif

}

int main(int argc, char* argv[])
{
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	QApplication app(argc, argv);
	initialiserBibliothequeCours(argc, argv);

	//CalcWindow calcWindow;
	//calcWindow.show();
	

	//DEBUT PARTIE
	Partie partie;
	partie.afficher();

	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	partie.deplacerPiece({ 4, 2 }, { 5, 3 });
	partie.afficher();

	partie.deplacerPiece({ 4, 2 }, { 6, 3 });
	partie.afficher();

	partie.deplacerPiece({ 5, 3 }, { 6, 5 });
	partie.afficher();

	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	partie.deplacerPiece({ 5, 3 }, { 4, 2 });
	partie.afficher();

	partie.ajouterPiece(std::make_unique<Tour>(0, 3, false));
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	partie.deplacerPiece({ 0, 3 }, { 4, 3 });
	partie.afficher();

	partie.deplacerPiece({ 4, 3 }, { 6, 3 });
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	partie.deplacerPiece({ 4, 3 }, { 5, 3 });
	partie.afficher();

	partie.ajouterPiece(std::make_unique<Cavalier>(3, 4, true));
	partie.afficher();

	partie.deplacerPiece({ 3, 4 }, { 3, 7 });
	partie.afficher();

	partie.deplacerPiece({ 3, 4 }, { 5, 3 });
	partie.afficher();
	std::cout << (partie.estEnEchec(true)) << std::endl;

	partie.deplacerPiece({ 5, 3 }, { 4, 1 });
	partie.afficher();
	
	FenetreJeu fenetre;
	fenetre.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	fenetre.ajouterPiece(std::make_unique<Tour>(0, 3, false));
	fenetre.ajouterPiece(std::make_unique<Cavalier>(3, 4, true));
	fenetre.ajouterPiece(std::make_unique<Roi>(1, 6, false));
	fenetre.ajouterPiece(std::make_unique<Tour>(4, 1, true));
	fenetre.ajouterPiece(std::make_unique<Cavalier>(7, 0, false));

	fenetre.show();
	

	return app.exec();
}
