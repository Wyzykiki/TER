#ifndef POPULATIONCENTERED_H
#define POPULATIONCENTERED_H

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "reaction.h"
#include "simulation.h"

class populationCentered : public Simulation
{
private:
	/** Tableau de toutes les réactions possibles. */ 
	Reaction** reactions;
	int nbReactions;

	/** Volume de la simulation */
	float volume;

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

	populationCentered(EspeceMoleculaire* especes[], int nbEspeces, Reaction* reactions[], int nbReactions, float volume);
	~populationCentered();

	/** Examine chaque réaction de la simulation */
	void epoch();
	
	/** Lance la simulation */
	void run();
};


#endif