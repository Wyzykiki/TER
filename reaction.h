#ifndef REACTION_H
#define REACTION_H

#include <stdio.h>
#include <stdlib.h>


class EspeceMoleculaire;

class Reaction {
    private:
        EspeceMoleculaire* reactifs;
        EspeceMoleculaire* produits;
        float proba;

    public:
        Reaction () : proba(0.0) {};

        Reaction (EspeceMoleculaire*, EspeceMoleculaire*, float);

        EspeceMoleculaire* getReactifs () { return reactifs; }
        void setReactifs (EspeceMoleculaire* em) { reactifs = em; }
        EspeceMoleculaire* getProduits () { return produits; }
        void setProduits (EspeceMoleculaire* em) { produits = em; }
        float getProba () { return proba; }
        void setProba (float f) { proba = f; }
};

#include "especemoleculaire.h"

#endif