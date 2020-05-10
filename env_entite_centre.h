#include <stdio.h>
#include <vector>
#include <chrono>
#include <functional>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <algorithm>

#include "molecule.h"
#include "simulation.h"

class Env_entite_centre : public Simulation {
    private :
        int nb_mols;
        int diametre;
        int radius;
        int stepMax;
        std::vector<Molecule*> molecules;
        Molecule**** grid;
        EspeceMoleculaire** especes;

    public :
     Env_entite_centre(EspeceMoleculaire**, int, float, int);
     ~Env_entite_centre();

    std::vector<Molecule*> getMolecules() { return this->molecules; }

     void run();
};