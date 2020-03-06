#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>
#include "especemoleculaire.h"

#ifndef NB_MAX_ESP
#define NB_MAX_ESP 30
#endif

#ifndef SHIFT
#define SHIFT 7
#endif

struct file_vars {
	int diametre;
	EspeceMoleculaire* especes[NB_MAX_ESP];
};


class EspeceMoleculaireNotFound : public std::exception {
    private:
        std::string esp_name;
    public:
        EspeceMoleculaireNotFound(char* name) { esp_name = name; }

        virtual const char* what() const throw() {
            return ("The molecular species named " + esp_name + " does not exist.").c_str();
        }
};


int myhash(char*);
EspeceMoleculaire* find(char*, EspeceMoleculaire**);
void add(EspeceMoleculaire*, EspeceMoleculaire***);


#endif