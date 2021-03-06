#ifndef ESPECEMOLECULAIRE_H
#define ESPECEMOLECULAIRE_H

#include <stdio.h>
#include <iostream>
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
        
        /** Pour l'affichage */
        int color[3] = {0, 0, 0};

    public:
        EspeceMoleculaire() {}
        EspeceMoleculaire(char* n) : vitesse(1.0), taille(10), nbCopies(0){
            int length = strlen(n);
            nom = new char[length+1];
            strcpy(nom,n);
        }
        EspeceMoleculaire(const EspeceMoleculaire&);

        ~EspeceMoleculaire() { delete nom; }

        char* getNom() { return nom; }
        float getVitesse() { return vitesse; }
        void setVitesse(float v) { vitesse = v; }
        int getTaille() { return taille; }
        void setTaille(int t) { taille = t; }
        int getNbCopies() { return nbCopies; }
        void setNbCopies(int n) { nbCopies = n; }
        
        void addReaction (Reaction* r);
        std::vector<Reaction*>* getReactions () { return &reac_assoc; }
        Reaction* getReaction (int i);
        
        /* Retourne la position de la reaction dans le vector associant l'espece moleculaire courante et une espece moleculaire donnee */
        int getPosActualReaction (EspeceMoleculaire* em);

        bool operator==(const EspeceMoleculaire&);

        bool isDepleted();

        /** color */

        int getR() {
            return this->color[0];
        }
        int getG() {
            return this->color[1];
        }
        int getB() {
            return this->color[2];
        }
        void setR(int r) {
            this->color[0] = r;
        }
        void setG(int g) {
            this->color[1] = g;
        }
        void setB(int b) {
            this->color[2] = b;
        }
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