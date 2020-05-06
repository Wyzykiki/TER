#include "molecule.h"

Molecule::Molecule(EspeceMoleculaire* espece, int a, int b, int c) {
    em = espece;
    x = a;
    y = b;
    z = c;
}

Molecule::Molecule(const Molecule& mol){
    x = mol.x;
    y = mol.y;
    z = mol.z;
    em = mol.em;
}

Molecule& Molecule::operator=(const Molecule &mol){
    if(this != &mol){
        x = mol.x;
        y = mol.y;
        z = mol.z;
        em = mol.em;
    }
    return *this;
}

bool Molecule::operator==(const Molecule &mol){
    return x == mol.x && y == mol.y && z == mol.z && em == mol.em;
}

Molecule::~Molecule(){
    delete em;
}


void Molecule::setPos(int a, int b, int c){
    x = a;
    y = b;
    z = c;
}