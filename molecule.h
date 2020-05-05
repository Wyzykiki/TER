#include "especemoleculaire.h"

class Molecule {
    private:
        int x;
        int y;
        int z;
        EspeceMoleculaire* em;

    public:
        Molecule() : x(0), y(0), z(0) {}
        Molecule(EspeceMoleculaire* espece) : x(0), y(0), z(0) {this->em = espece;}

        ~Molecule() { delete em; }

        int getX() { return x; }
        void setX(int i) { x = i; }
        int getY() { return y; }
        void setY(int i) { y = i; }
        int getZ() { return z; }
        void setZ(int i) { z = i; }
        EspeceMoleculaire* getEspece() { return em; }
        void setEspece(EspeceMoleculaire* em) { this->em = em; }
};