#include "reaction.h"

Reaction::Reaction (EspeceMoleculaire* r, EspeceMoleculaire* p, float f){
    reactifs = (EspeceMoleculaire*) malloc(sizeof(r));
    *reactifs = *r;
    produits = (EspeceMoleculaire*) malloc(sizeof(p));
    *produits = *p;
    proba = f;
}