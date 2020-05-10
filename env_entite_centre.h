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
        int nb_mols;                            // nombre total de molecules
        int diametre;                           // diametre du environnement
        int radius;                             // rayon de l'environnement
        std::vector<Molecule*> molecules;       // vector contenant  des pointeurs l'ensemble des molecules
        Molecule**** grid;                      // representation de l'espace en 3 dimensions
        EspeceMoleculaire** especes;            // liste de toutes les especes moleculaires presentes dans la simulation

    public :
     Env_entite_centre(EspeceMoleculaire**, int, float);
     ~Env_entite_centre();

     void run();    // deroulement de la simulation
};