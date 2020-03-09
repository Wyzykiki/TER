#include "especemoleculaire.h"

void EspeceMoleculaire::addReaction (Reaction* r) { reac_assoc.push_back(r); }

Reaction* EspeceMoleculaire::getReaction (int i) { return reac_assoc.at(i); }

int EspeceMoleculaire::getPosActualReaction (EspeceMoleculaire em){
    int i = 0;
    while (i < reac_assoc.size()) {
        if(strcmp(reac_assoc.at(i)->getReactifs()[2]->getNom(), em.getNom()) == 0){
            return i;
        }
        i++;
    }
    return -1;
}
