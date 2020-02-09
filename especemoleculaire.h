#ifndef ESPECEMOLECULAIRE_H
#define ESPECEMOLECULAIRE_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <exception>


class Reaction;

class EspeceMoleculaire {
    private:
        char* nom;
        float vitesse;
        int taille;
        int nbCopies;
        // std::vector<Reaction> reac_assoc;

    public:
        EspeceMoleculaire() {}
        EspeceMoleculaire(char* n) : vitesse(1.0), taille(10), nbCopies(0){
            int length = strlen(n);
            nom = new char[length+1];
            strcpy(nom,n);
        }

        char* getNom() { return nom; }
        float getVitesse() { return vitesse; }
        void setVitesse(float v) { vitesse = v; }
        int getTaille() { return taille; }
        void setTaille(int t) { taille = t; }
        int getNbCopies() { return nbCopies; }
        void setNbCopies(float f) { nbCopies = 6.02214076e23*f; }
        
        //FIXME: ?
        // void addReaction (Reaction r) { reac_assoc.push_back(r); }
        std::vector<Reaction> getReaction () {}

};

class IncorrectProbabilityRates : public std::exception {
    private:
        std::string esp_name;
    public:
        IncorrectProbabilityRates(const char* esp_name_) { esp_name = esp_name_; }

        virtual const char* what() const throw() {
            return ("The sum of the reactions probabilities is greater than 1 for " + esp_name).c_str();
        }
};

#include "reaction.h"

#endif