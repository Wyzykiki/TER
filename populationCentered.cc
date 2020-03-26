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
	for (int i=0; i<5000; i++) {
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


	float propension = (alpha * (r1Amount + r2Amount) * r->getProba()) / volume;
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

int main() {
	EspeceMoleculaire* e1 = new EspeceMoleculaire("E");
	EspeceMoleculaire* e2 = new EspeceMoleculaire("s");
	EspeceMoleculaire* e3 = new EspeceMoleculaire("Es");
	EspeceMoleculaire* e4 = new EspeceMoleculaire("p");

	EspeceMoleculaire* especes[4];
	especes[0] = e1;
	especes[1] = e2;
	especes[2] = e3;
	especes[3] = e4;


	e1->setNbCopies(3000);
	e2->setNbCopies(200);

	EspeceMoleculaire* reac[2];
	reac[0] = e1;
	reac[1] = e2;

	EspeceMoleculaire* prod[1];
	prod[0] = e3;


	Reaction* r1 = new Reaction(reac, prod, 2, 1, 0.3);
	
	EspeceMoleculaire* reac2[1];
	reac2[0] = e3;

	EspeceMoleculaire* prod2[2];
	prod2[0] = e1;
	prod2[1] = e2;

	Reaction* r2 = new Reaction(reac2, prod2, 1, 2, 0.002);


	EspeceMoleculaire* reac3[1];
	reac3[0] = e3;

	EspeceMoleculaire* prod3[2];
	prod3[0] = e1;
	prod3[1] = e4;

	Reaction* r3 = new Reaction(reac3, prod3, 1, 2, 0.005);

	Reaction* reacs[3];

	reacs[0] = r1;
	reacs[1] = r2;
	reacs[2] = r3;

 	float volume = 0.000523598;

	populationCentered sim = populationCentered(especes, 4, reacs, 3, volume);

	sim.run();


	return 0;
}