#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include <fstream>
#include "especemoleculaire.h"

class Simulation
{
private:
	/** Tableau des especes */
	EspeceMoleculaire** especes;
	int nbEspeces;
	
	/** Réinitialise le fichier et écrie la première ligne */
	void initCSV();

protected:
	/** Numéro de l'époque */
	int nEpoch;

	/** Ecrie les concentrations des especes toutes les n époques. */
	void exportCSV();

public:
	Simulation(EspeceMoleculaire* especes[], int nbEspeces);
	~Simulation();

	/** A redéfinir */
	void run();
	
};

struct File_vars {
	int diametre;
	float volume;
	EspeceMoleculaire** especes;
	int size;

	File_vars(int diametre_, float volume_, EspeceMoleculaire** especes_, int size_) {
		diametre = diametre_;
		volume = volume_;
		especes = especes_;
		size = size_;
	}

	File_vars(const File_vars &other) {
		diametre = other.diametre;
		volume = other.volume;
		especes = other.especes;
		size = other.size;
	}

	~File_vars() {
		free(especes);
	}
};

#endif