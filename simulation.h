#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include "especemoleculaire.h"

struct file_vars {
	int diametre;
	std::map<char*, EspeceMoleculaire> especes;
};

#endif