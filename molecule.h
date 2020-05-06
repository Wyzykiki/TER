#ifndef MOLECULE_H
#define MOLECULE_H

#include <stdio.h>

#include "especemoleculaire.h"

class Molecule {
    private:
        int x;
        int y;
        int z;
        EspeceMoleculaire* em;

    public:
        Molecule() : x(0), y(0), z(0) {}
        Molecule(EspeceMoleculaire* espece) : x(0), y(0), z(0) {
            em = espece;
        }
        Molecule(EspeceMoleculaire*, int, int, int);
        Molecule(const Molecule &);

        ~Molecule();

        int getX() { return x; }
        void setX(int i) { x = i; }
        int getY() { return y; }
        void setY(int i) { y = i; }
        int getZ() { return z; }
        void setZ(int i) { z = i; }
        void setPos(int, int, int);

        Molecule& operator=(const Molecule &);
        bool operator==(const Molecule &);

        EspeceMoleculaire* getEM() { return em; }
        void setEspece(EspeceMoleculaire* em) { this->em = em; }
};

#endif