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
        std::vector<Reaction*> reac_assoc;

    public:
        EspeceMoleculaire() {}
        EspeceMoleculaire(char* n) : vitesse(1.0), taille(10), nbCopies(0){
            int length = strlen(n);
            nom = new char[length+1];
            strcpy(nom,n);
        }

        ~EspeceMoleculaire() { delete nom; }

        char* getNom() { return nom; }
        float getVitesse() { return vitesse; }
        void setVitesse(float v) { vitesse = v; }
        int getTaille() { return taille; }
        void setTaille(int t) { taille = t; }
        int getNbCopies() { return nbCopies; }
        void setNbCopies(float f) { nbCopies = 6.02214076e23*f; }
        
        void addReaction (Reaction* r);
        std::vector<Reaction*>* getReactions () { return &reac_assoc; }
        Reaction* getReaction (int i);
        
        /* Retourne la position de la reaction dans le vector associant l'espece moleculaire courante et une espece moleculaire donnee */
        int getPosActualReaction (EspeceMoleculaire em);

};

class IncorrectProbabilityRates : public std::exception {
    private:
        std::string esp_name;
    public:
        IncorrectProbabilityRates(const char* esp_name_) { 
            esp_name = esp_name_; 
            esp_name = ("The sum of the reactions probabilities is greater than 1 for " + esp_name);
        }

        const char* what() const throw() {
            return esp_name.c_str();
        }
};

#include "reaction.h"

#endif