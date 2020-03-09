#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include "especemoleculaire.h"

struct File_vars {
	int diametre;
	EspeceMoleculaire** especes;
	int size;

	File_vars(int diametre_, EspeceMoleculaire** especes_, int size_) {
		diametre = diametre_;
		especes = especes_;
		size = size_;
	}

	File_vars(const File_vars &other) {
		diametre = other.diametre;
		especes = other.especes;
		size = other.size;
	}

	~File_vars() {
		free(especes);
	}
};

#endif