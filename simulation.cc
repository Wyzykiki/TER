#include "simulation.h"

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
