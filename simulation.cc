#include <iostream>
#include "simulation.h"

using namespace std;

extern file_vars parse(char*);

int main() {
    file_vars init = parse("file.sim");
    const int diametre = init.diametre;
	map<char*, EspeceMoleculaire> especes = init.especes;

    cout << diametre << endl;
    for (auto& x: especes) {
        cout << x.first  << endl;
    }
    return 0;
}