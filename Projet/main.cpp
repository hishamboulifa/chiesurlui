

//#include "Calc.hpp"
//#include "CalcWindow.hpp"

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
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette ent�te si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.
#endif

#include "Partie.h"

void initialiserBibliothequeCours([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	#ifdef BIBLIOTHEQUE_COURS_INCLUS
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par d�faut.

	// cdbg.setTee(&clog);  // D�commenter cette ligne pour que cdbg affiche sur la console en plus de la "Sortie" du d�bogueur.
	
	bibliotheque_cours::executerGoogleTest(argc, argv); // Attention de ne rien afficher avant cette ligne, sinon l'Explorateur de tests va tenter de lire votre affichage comme un r�sultat de test.
	#endif
	//NOTE: C'est normal que la couverture de code dans l'Explorateur de tests de Visual Studio ne couvre pas la fin de cette fonction ni la fin du main apr�s l'appel � cette fonction puisqu'il ex�cute uniquement les tests Google Test dans l'appel ci-dessus.
}

int main(int argc, char *argv[])
{
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	QApplication app(argc, argv);
	initialiserBibliothequeCours(argc, argv);

	//CalcWindow calcWindow;
	//calcWindow.show();


	// S�rie de tests pr�liminaires pour v�rifier qu�on peut bien bouger les pi�ces seulement pour des mouvements valides

	//D�but de la partie
	Partie partie;
	partie.afficher();

	// Cr�ation d'un roi blanc en C5
	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	// D�placement du roi blanc vers D6
	partie.deplacerPiece({ 4, 2 }, { 5, 3 });
	partie.afficher();

	// Tentative de mouvement depuis la case C5 laiss�e vide
	partie.deplacerPiece({ 4, 2 }, { 6, 3 });
	partie.afficher();

	// Tentative de d�placement du roi blanc de plus d'une case
	partie.deplacerPiece({ 5, 3 }, { 6, 5 });
	partie.afficher();

	// Cr�ation d'un roi blanc en C5
	partie.ajouterPiece(std::make_unique<Roi>(4, 2, true));
	partie.afficher();

	// Tentative de prise du second roi blanc par le premier
	partie.deplacerPiece({ 5, 3 }, { 4, 2 });
	partie.afficher();

	// Cr�ation d'une tour noire en D1
	partie.ajouterPiece(std::make_unique<Tour>(0, 3, false));
	partie.afficher();

	// D�placement de la tour noire vers D5
	partie.deplacerPiece({ 0, 3 }, { 4, 3 });
	partie.afficher();

	// Tentative de d�placement de la tour noire par dessus le roi blanc se trouvant en D6
	partie.deplacerPiece({ 4, 3 }, { 6, 3 });
	partie.afficher();

	// Prise du roi blanc se trouvant en D6 par la tour noire
	partie.deplacerPiece({ 4, 3 }, { 5, 3 });
	partie.afficher();

	// Cr�ation d'un cavalier blanc en E4
	partie.ajouterPiece(std::make_unique<Cavalier>(3, 4, true));
	partie.afficher();

	// Tentative de d�placement du cavalier le long d'une ligne
	partie.deplacerPiece({ 3, 4 }, { 3, 7 });
	partie.afficher();

	// Prise de la tour noire par le cavalier blanc
	partie.deplacerPiece({ 3, 4 }, { 5, 3 });
	partie.afficher();

	// D�placement du cavalier blanc de D6 vers B5
	partie.deplacerPiece({ 5, 3 }, { 4, 1 });
	partie.afficher();


	return app.exec();
}
