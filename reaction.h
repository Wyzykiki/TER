#ifndef REACTION_H
#define REACTION_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class EspeceMoleculaire;

class Reaction {
    private:
        EspeceMoleculaire** reactifs;
        EspeceMoleculaire** produits;
        int nbReactifs;
        int nbProduits;
        float proba;

    public:
        Reaction () : proba(0.0) {};
        Reaction (EspeceMoleculaire**, EspeceMoleculaire**, int, int, float);

        ~Reaction () { delete reactifs; delete produits;}

        EspeceMoleculaire** getReactifs () { return reactifs; }
        void setReactifs (EspeceMoleculaire** em) { reactifs = em; }
        EspeceMoleculaire** getProduits () { return produits; }
        void setProduits (EspeceMoleculaire** em) { produits = em; }
        float getProba () { return proba; }
        void setProba (float f) { proba = f; }
        friend std::ostream& operator<< (std::ostream &stream, const Reaction& r);
};

#include "especemoleculaire.h"

#endif