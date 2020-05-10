#include <iostream>
#include "populationCentered.h"

using namespace std;

extern File_vars parse(char*);


int main(int argc, char** argv) {
	if (argc == 2 && (strstr(argv[1], ".sim") != NULL || strstr(argv[1], ".txt") != NULL)) {
		File_vars init(parse(argv[1]));

		std::cout << "Starting with file " << argv[1] << " ..." << std::endl;

		populationCentered sim = populationCentered(init.especes, init.size, init.reactions, init.nbReactions, init.volume);
		sim.run();

        std::cout << "Simulation ended. You can find results in results.csv." << std::endl;
    } else {
        std::cerr << "Usage : ./entCentre [file_name].sim" << std::endl;
    }
	return 0;
}