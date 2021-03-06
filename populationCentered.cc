#include "populationCentered.h"

populationCentered::populationCentered(EspeceMoleculaire* especes_[], int nbEspeces, Reaction* reactions_[], int nbReactions, float volume)
: Simulation(especes_, nbEspeces) {
		
	this->reactions = new Reaction*[nbReactions];
	for (int i=0; i<nbReactions; i++) {
		this->reactions[i] = reactions_[i];
	}
	this->nbReactions = nbReactions;

	this->volume = volume;
	this->flipFlop = true;

	srand(time(NULL));
}

populationCentered::~populationCentered() {
	delete[] reactions;
}

void populationCentered::run() {
	for (int i=0; i<300000; i++) {
		this->exportCSV();
		this->epoch();
	}
}

void populationCentered::monoMolecule(Reaction* r) {

	EspeceMoleculaire** reactifs = r->getReactifs();
	EspeceMoleculaire** produits = r->getProduits();

	int r1Amount = reactifs[0]->getNbCopies();

	float propension = r1Amount * r->getProba();

	int activeReactions = (int) propension;


	float random = (float) rand()/RAND_MAX;

	if (random < (propension - activeReactions))
		activeReactions++;

	reactifs[0]->setNbCopies(r1Amount - activeReactions);

	for (int i=0; i< r->getNbProduits(); i++)
		produits[i]->setNbCopies(produits[i]->getNbCopies() + activeReactions);	

}

void populationCentered::biMolecule(Reaction* r) {
	const float alpha = 7.4e-7;

	EspeceMoleculaire** reactifs = r->getReactifs();
	EspeceMoleculaire** produits = r->getProduits();
	
	int r1Amount = reactifs[0]->getNbCopies();
	int r2Amount = reactifs[1]->getNbCopies();


	float propension = (alpha * r1Amount * r2Amount * r->getProba()) / volume;
	int activeReactions = (int) propension;


	float random = (float) rand()/RAND_MAX;

	if (random < (propension - activeReactions))
		activeReactions++;


	if (reactifs[0] == reactifs[1]) {

		activeReactions = activeReactions/2;

		if (activeReactions*2 > r1Amount)
			activeReactions = r1Amount/2;

		reactifs[0]->setNbCopies(r1Amount - 2*activeReactions);

	} else {
		if (activeReactions > r1Amount || activeReactions > r2Amount)
			activeReactions = std::min(r1Amount,r2Amount);

		reactifs[0]->setNbCopies(r1Amount - activeReactions);
		reactifs[1]->setNbCopies(r2Amount - activeReactions);
	}


	for (int i=0; i< r->getNbProduits(); i++)
		produits[i]->setNbCopies(produits[i]->getNbCopies() + activeReactions);

}

void populationCentered::epoch() {
	int i, end, step;

	if (this->flipFlop) {
		i = 0;
		end = nbReactions;
		step = 1;
	} else {
		i = nbReactions-1;
		end = -1;
		step = -1;
	}

	while (i != end) {
		Reaction* r = this->reactions[i];

		int nbReactifs = r->getNbReactifs();
		EspeceMoleculaire** reactifs = r->getReactifs();
		
		/** S'il y a assez de réctifs pour faire la réaction */
		bool isPossible = true;

		for (int j=0; j<nbReactifs; j++) {
			if (reactifs[j]->isDepleted())
				isPossible = false;
		}

		if (isPossible) {
			if (nbReactifs == 1)
				monoMolecule(r);
			else
				biMolecule(r);
		}

		i+=step;
	}

	this->nEpoch++;
	this->flipFlop = !this->flipFlop;
}
