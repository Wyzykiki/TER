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