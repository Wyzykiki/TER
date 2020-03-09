#include <iostream>
#include "simulation.h"

using namespace std;

extern File_vars parse(char*);

int main() {
    File_vars init(parse("file.sim"));

    //TODO: Creer tableau de reactions et les molecules
    cout << init.diametre << endl;


    for (int i = 0; i<init.size; i++) {
        cout<<init.especes[i]<<">>>"<<init.especes[i]->getNom()<<" taille: "<<init.especes[i]->getTaille()<<" vitesse: "<<init.especes[i]->getVitesse()<<" init: "<<init.especes[i]->getNbCopies()<< endl;
        vector<Reaction*>* reactions = init.especes[i]->getReactions();
        int nbReaction = reactions->size();
        for (int j = 0; j<nbReaction; j++) {
            cout << "Reac "<<j<<" : "<<*(*reactions)[j]<<endl;
        }
    }
    return 0;
}