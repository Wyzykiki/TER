#include "reaction.h"

Reaction::Reaction (EspeceMoleculaire** r, EspeceMoleculaire** p, int nbReac, int nbProd, float f){
    reactifs = r;
    produits = p;
    nbReactifs = nbReac;
    nbProduits = nbProd;
    proba = f;

    for (int i = 0; i<nbReac; i++)
        reactifs[i]->addReaction(this);
}

int Reaction::getNbReactifs() {
    return this->nbReactifs;
}

int Reaction::getNbProduits() {
    return this->nbProduits;
}

std::ostream& operator<< (std::ostream &stream, const Reaction &r) {
    stream<<r.reactifs[0]->getNom();
    if (r.nbReactifs == 2)
        stream<<"+"<<r.reactifs[1]->getNom();
    stream<<"->"<<r.produits[0]->getNom();
    if (r.nbProduits == 2)
        stream<<"+"<<r.produits[1]->getNom();
    stream<<" proba: "<<r.proba;    
    return stream;
}

Reaction::Reaction(const Reaction& r) {
    reactifs = r.reactifs;
    produits = r.produits;
    nbReactifs = r.nbReactifs;
    nbProduits = r.nbProduits;
    proba = r.proba;
}

Reaction& Reaction::operator=(const Reaction& r) {
    if(this != &r) {
        reactifs = r.reactifs;
        produits = r.produits;
        nbReactifs = r.nbReactifs;
        nbProduits = r.nbProduits;
        proba = r.proba;
    }
    return *this;
}