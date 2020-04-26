#include <stdio.h>

#include "especemoleculaire.h"

class Molecule {
    private:
        int x;
        int y;
        int z;
        EspeceMoleculaire* em;

    public:
        Molecule();
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
        EspeceMoleculaire* getEM() { return em; }

        Molecule& operator=(const Molecule &);
        bool operator==(const Molecule &);

};