#include <iostream>
#include "simulation.h"

using namespace std;

extern file_vars parse(char*);


int myhash(char* str){
    int result = 0;
    for(int i = 0 ; i < strlen(str) ; i++){
        result += str[i]*i;
    }
    return result%NB_MAX_ESP;
}


EspeceMoleculaire* find(char* name, EspeceMoleculaire** ems) {
    int hashEM = myhash(name);
    if(strcmp(ems[hashEM]->getNom(), name) == 0){
        return ems[hashEM];
    } else {
        bool hasFound = false;
        int hashPlusDecalage = hashEM+SHIFT;
        while(!hasFound && hashEM != hashPlusDecalage){
            hasFound = strcmp(ems[hashPlusDecalage]->getNom(), name) == 0;
            hashPlusDecalage+=SHIFT;
        }
        if(!hasFound){
            throw EspeceMoleculaireNotFound(name);
        }
        return ems[hashPlusDecalage-SHIFT];
    }
}


void add(EspeceMoleculaire* emAdd, EspeceMoleculaire*** ptr_ems) {
    int hashEM = myhash(emAdd);
    bool hasFound = false;
    int hashPlusDecalage = hashEM;
    while(!hasFound && hashEM != hashPlusDecalage){
        hasFound = ptr_ems[hashPlusDecalage] == 0;
        if(hasFound){
            ptr_ems[hashPlusDecalage] = emAdd;
            hasFound = false;
        }
        hashPlusDecalage+=SHIFT;
    }
}




int main() {
    file_vars init = parse("file.sim");
    const int diametre = init.diametre;
	EspeceMoleculaire* especes[NB_MAX_ESP] = init.especes;

    cout << diametre << endl;
    for (auto& x: especes) {
        cout << x.getNom()  << endl;
    }
    return 0;
}