#ifndef POPULATIONCENTERED_H
#define POPULATIONCENTERED_H

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "reaction.h"

class populationCentered
{
private:
	/** Tableau de toutes les réactions possibles. */ 
	Reaction** reactions;
	int nbReactions;

	float volume;

	/** TODO: */
	// EspeceMoleculaire** especes;
	// int nbEspeces;

	/** Determine le sens de parcours des réactions. */
	bool flipFlop;

	/** Calcule le nombre de fois où @param r est effectué et mets à jour les quantités de réactifs et de produits.
	 *  Pour une réaction de type s -> xxx
	 */
	void monoMolecule(Reaction* r);
	/** Calcule le nombre de fois où @param r est effectué et mets à jour les quantités de réactifs et de produits.
	 *  Pour une réaction de type s + t -> xxx
	 */
	void biMolecule(Reaction* r);

public:

	populationCentered(Reaction* reactions[], int nbReactions);
	~populationCentered();

	/** Examine chaque réaction de la simulation */
	void epoch();

};


#endif