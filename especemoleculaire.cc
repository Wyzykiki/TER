#include "especemoleculaire.h"

void EspeceMoleculaire::addReaction (Reaction* r) { reac_assoc.push_back(r); }

Reaction* EspeceMoleculaire::getReaction (int i) { return reac_assoc.at(i); }

int EspeceMoleculaire::getPosActualReaction (EspeceMoleculaire* em){
    int i = 0;
    
    while (i < reac_assoc.size()) {
        if(strcmp(this->getNom(), reac_assoc.at(i)->getReactifs()[0]->getNom()) == 0 && (reac_assoc.at(i)->getNbReactifs() == 2)){
            if(strcmp(reac_assoc.at(i)->getReactifs()[1]->getNom(), em->getNom()) == 0){
                return i;
            }
        } else if(reac_assoc.at(i)->getNbReactifs() == 2 && strcmp(this->getNom(), reac_assoc.at(i)->getReactifs()[1]->getNom()) == 0) {
            if(strcmp(reac_assoc.at(i)->getReactifs()[0]->getNom(), em->getNom()) == 0){
                return i;
            }
        }
        i++;
    }
    return -1;
}

bool EspeceMoleculaire::isDepleted() {
    return this->nbCopies == 0;
}

bool EspeceMoleculaire::operator==(const EspeceMoleculaire& em) {
    return nom == em.nom;
}

EspeceMoleculaire::EspeceMoleculaire(const EspeceMoleculaire &em){
    nom = em.nom;
    vitesse = em.vitesse;
    taille = em.taille;
    nbCopies = em.nbCopies;
}
