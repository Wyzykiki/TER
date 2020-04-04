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
	virtual void run() = 0;
	
};

struct File_vars {
	float diametre;
	float volume;
	EspeceMoleculaire** especes;
	int size;
	Reaction** reactions;
	int nbReactions;

	File_vars(float diametre_, float volume_, EspeceMoleculaire** especes_, int size_, Reaction** reactions_, int nbReactions_) {
		diametre = diametre_;
		volume = volume_;
		especes = especes_;
		size = size_;
		reactions = reactions_;
		nbReactions = nbReactions_;
	}

	File_vars(const File_vars &other) {
		diametre = other.diametre;
		volume = other.volume;
		especes = other.especes;
		size = other.size;
		reactions = other.reactions;
		nbReactions = other.nbReactions;
	}

	~File_vars() {
		free(especes);
	}
};

#endif