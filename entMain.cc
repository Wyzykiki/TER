#include <stdio.h>
#include "env_entite_centre.h"
#include "simulation.h"

extern File_vars parse(char*);


int main(int argc, char** argv) {
    if (argc == 2 && (strstr(argv[1], ".sim") != NULL || strstr(argv[1], ".txt") != NULL)) {
        File_vars init(parse(argv[1]));

        std::cout << "Starting with file " << argv[1] << " ..." << std::endl;

        Env_entite_centre env(init.especes, init.size, init.diametre, 10000);
        env.run();

        std::cout << "Simulation ended. You can find results in results.csv." << std::endl;
    } else {
        std::cerr << "Usage : ./entCentre [file_name].sim" << std::endl;
    }

    return 0;
}