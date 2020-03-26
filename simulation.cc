#include <iostream>
#include "simulation.h"

using namespace std;

extern File_vars parse(char*);

Simulation::Simulation(EspeceMoleculaire* especes_[], int nbEspeces) {
    this->especes = new EspeceMoleculaire*[nbEspeces];
	for (int i=0; i<nbEspeces; i++) {
		this->especes[i] = especes_[i];
	}
	this->nbEspeces = nbEspeces;

    this->nEpoch = 0;

	this->initCSV();
}

Simulation::~Simulation() {
    delete[] especes;
}

void Simulation::initCSV() {
	std::fstream file("results.csv", std::fstream::out);
	file<<"Date(ms)";
	for (int i=0; i<this->nbEspeces; i++) {
		file<<","<<this->especes[i]->getNom();
	}
	file<<std::endl;
	file.close();
}

void Simulation::exportCSV() {
	if (this->nEpoch%10==0) {
		std::fstream file("results.csv", std::fstream::app);
		file<<(this->nEpoch*0.1);
		for (int i=0; i<this->nbEspeces; i++) {
			file<<","<<this->especes[i]->getNbCopies();
		}
		file<<std::endl;
		file.close();
	}
}

void Simulation::run() {}


int main() {
    File_vars init(parse("file.sim"));

    //TODO: Creer tableau de reactions et les molecules
    cout << init.diametre << endl;


    for (int i = 0; i<init.size; i++) {
        cout<<init.especes[i]<<">>>"<<init.especes[i]->getNom()<<" taille: "<<init.especes[i]->getTaille()<<" vitesse: "<<init.especes[i]->getVitesse()<<" init: "<<init.especes[i]->getNbCopies()<< endl;
        vector<Reaction*>* reactions = init.especes[i]->getReactions();
        int nbReaction = reactions->size();
        for (int j = 0; j<nbReaction; j++) {
            cout << "Reac "<<j<<" : "<<*(*reactions)[j]<<endl;
        }
    }
    return 0;
}