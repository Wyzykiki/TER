#include "env_entite_centre.h"
#include <stdio.h>

Env_entite_centre::Env_entite_centre(EspeceMoleculaire* ems[], int size, float diam, int stepMax) : Simulation(ems, size){
    diametre = diam * 100;
    radius = diametre/2;        // le rayon nous permettra de centrer les molecules autours de (0,0,0) pour l'affichage

    this->stepMax = stepMax;

    nb_mols = 0;
    for(int i = 0 ; i < size ; i++) {
        nb_mols += ems[i]->getNbCopies();
    }

    // initialisation de l'espace avec des pointeurs nuls puisqu'il n'existe pas encore de molecule
    grid = new Molecule***[diametre];
    for(int i = 0 ; i < diametre ; i++){
        grid[i] = new Molecule**[diametre];
        for(int j = 0 ; j < diametre ; j++){
            grid[i][j] = new Molecule*[diametre];
            for(int k = 0 ; k < diametre ; k++) {
                grid[i][j][k] = 0;
            }
        }
    }
 
    srand(time(NULL));

    // on cree toutes les molecules avec des positions aleatoires dans l'environnement
    for(int i = 0; i < size ; i++){
        for(int j = 0 ; j < ems[i]->getNbCopies() ; j++){
            int x, y, z;
            do {
            x = std::rand()/((RAND_MAX + 1u)/diametre);
            y = std::rand()/((RAND_MAX + 1u)/diametre);
            z = std::rand()/((RAND_MAX + 1u)/diametre);
            } while(grid[x][y][z] != 0);
            molecules.push_back(new Molecule(ems[i], x - radius, y - radius, z - radius));
            grid[x][y][z] = molecules.back();
        }
    }

    // on melange toutes les molecules pour ne pas qu'il y ait un deroulement espece par espece mais que ce soit heterogene
    shuffle (molecules.begin(), molecules.end(), std::default_random_engine((unsigned) time(NULL)));
}

Env_entite_centre::~Env_entite_centre() {
    
}


void Env_entite_centre::run(){
    

    // std::cout<<"alo "<<molecules[0]->getX()<<" "<<molecules[0]->getY()<<" "<<molecules[0]->getZ()<<std::endl;
    int step=0;

    exportCSV();
    srand(time(NULL));

    while (step<this->stepMax){
        step++;
        for(int i = 0 ; i < nb_mols ; i++) {
            // on calcule un deplacement aleatoire
            // la molecule ne se deplace que sur un axe durant une periode, on choisi donc son axe et calcule se nouvelle position
            int axe = 1 + std::rand()/((RAND_MAX + 1u)/3);
            // On choisi si elle "avance" ou "recule" sur cet axe : 0 elle recule, 1 elle avance
            int dir = std::rand()/((RAND_MAX + 1u)/2);

            // On récupère la position de la molécule
            int newPosX = molecules.at(i)->getX() + radius;
            int newPosY = molecules.at(i)->getY() + radius;
            int newPosZ = molecules.at(i)->getZ() + radius;

            bool hasEncountered = false;

            // Comme la grille est entiere, on calcule la probabilite de se deplacer d'une case de plus basee sur la decimale de la vitesse
            long ent = (long) molecules.at(i)->getEM()->getVitesse();
            int dig = (int)((molecules.at(i)->getEM()->getVitesse()-ent)*10 + 0.5);
            bool willMoveDigit = std::rand()/((RAND_MAX + 1u)/10) <= dig;

            // On calcule la nouvelle position de la molecule après déplacement
            if(axe == 1) newPosX += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) + ((willMoveDigit) ? 1 : 0);
            else if(axe == 2) newPosY += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) + ((willMoveDigit) ? 1 : 0);
            else if(axe == 3) newPosZ += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) + ((willMoveDigit) ? 1 : 0);

            
            //on verifie que la molecule ne sorte pas de la zone en se deplacant
            if(newPosX >= 0 && newPosX < diametre && newPosY >= 0 && newPosY < diametre && newPosZ >= 0 && newPosZ < diametre) {
                //si collision avec une autre dans la grille elle arrête de se deplacer et retiens avec qui il y a eu collision
                Molecule* mol_encountered = new Molecule(*molecules.at(i));

                if(axe == 1){
                    int a = molecules.at(i)->getX() + radius + ((dir==0)?-1:1);
                    while(((dir == 0) ? a >= newPosX : a <= newPosX) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[a][newPosY][newPosZ] != 0);
                        if(hasEncountered) mol_encountered = (grid[a][newPosY][newPosZ]);
                        if(dir == 0) a--;
                        else a++;
                    }
                } else if(axe == 2){
                    int b = molecules.at(i)->getY() + radius + ((dir==0)?-1:1);
                    while(((dir == 0) ? b >= newPosY : b <= newPosY) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[newPosX][b][newPosZ] != 0);
                        if(hasEncountered) mol_encountered = (grid[newPosX][b][newPosZ]);
                        if(dir == 0) b--;
                        else b++;
                    }
                } else if (axe == 3){
                    int c = molecules.at(i)->getZ() + radius + ((dir==0)?-1:1);
                    while(((dir == 0) ? c >= newPosZ : c <= newPosZ) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[newPosX][newPosY][c] != 0);
                        if(hasEncountered) mol_encountered = (grid[newPosX][newPosY][c]);
                        if(dir == 0) c--;
                        else c++;
                    }
                }
                //chance de reagir en fonction de molecule croisee et proba
                if(hasEncountered){
                    int reac = molecules.at(i)->getEM()->getPosActualReaction(mol_encountered->getEM());    // regarde si il existe une reaction entre les especes des deux molecules
                    if(reac != -1){
                        bool willReact = ((float) (std::rand()/((RAND_MAX + 1u)/1000))/1000) <= molecules.at(i)->getEM()->getReaction(reac)->getProba();    // calcule si la reaction va avoir lieu en fonction de la probabilite
                        if(willReact){
                            // reaction se fait en ne donnant qu'un produit
                            if(molecules.at(i)->getEM()->getReaction(reac)->getNbProduits() == 1) {
                                std::vector<Molecule*>::iterator posMolEnc = std::find_if(molecules.begin(), molecules.end(), [&mol_encountered](Molecule *mol){return mol_encountered->getX() == mol->getX() && mol_encountered->getY() == mol->getY() && mol_encountered->getZ() == mol->getZ();});

                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()-1);
                                *posMolEnc = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[0], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ());
                                grid[mol_encountered->getX() + radius][mol_encountered->getY() + radius][mol_encountered->getZ() + radius] = (*posMolEnc);
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()+1);

                                grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] = 0;
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                                molecules.erase(std::remove(molecules.begin(), molecules.end(), molecules.at(i)), molecules.end());
                            }
                            // reaction se fait en donnant deux produits
                            else {
                                grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] = 0;
                                std::vector<Molecule*>::iterator posMolEnc = std::find_if(molecules.begin(), molecules.end(), [&mol_encountered](Molecule *mol){return mol_encountered->getX() == mol->getX() && mol_encountered->getY() == mol->getY() && mol_encountered->getZ() == mol->getZ();});
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()-1);
                                *posMolEnc = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[0], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ());
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()+1);
                                grid[(*posMolEnc)->getX() + radius][(*posMolEnc)->getY() + radius][(*posMolEnc)->getZ() + radius] = *posMolEnc;
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                                if(axe == 1) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX()+dir+(dir-1), mol_encountered->getY(), mol_encountered->getZ());
                                    grid[mol_encountered->getX()+dir+(dir-1) + radius][mol_encountered->getY() + radius][mol_encountered->getZ() + radius] = molecules.at(i);
                                }
                                else if(axe == 2) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX(), mol_encountered->getY()+dir+(dir-1), mol_encountered->getZ());
                                    grid[mol_encountered->getX() + radius][mol_encountered->getY()+dir+(dir-1) + radius][mol_encountered->getZ() + radius] = molecules.at(i);
                                }
                                else if(axe == 3) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ()+dir+(dir-1));
                                    grid[mol_encountered->getX() + radius][mol_encountered->getY() + radius][mol_encountered->getZ()+dir+(dir-1) + radius] = molecules.at(i);
                                }
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()+1);
                            }
                        }
                    }
                }
                // si elle ne ragit pas en chemin elle se deplace sur la case desiree
                else if(grid[newPosX][newPosY][newPosZ] == 0){
                    grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] = 0;
                    molecules.at(i)->setPos(newPosX - radius, newPosY - radius, newPosZ - radius);
                    grid[newPosX][newPosY][newPosZ] = molecules.at(i);
                }
            }
            // Si n'a pas réagi avec une autre espèce, a toujours la possibilité de réagir tout seul
            if(!hasEncountered){
                std::vector<Reaction*> reaction_assoc;
                reaction_assoc = *(molecules.at(i)->getEM()->getReactions());       // on recupere l'ensemble des reactions dans lesquels la molecule peut intervenir
                shuffle (reaction_assoc.begin(), reaction_assoc.end(), std::default_random_engine((unsigned) time(NULL)));

                int x = 0;
                bool hasReact = false;

                // on regarde si la molecule reagira seule
                while (x < reaction_assoc.size() && !hasReact) {
                    if(reaction_assoc.at(x)->getNbReactifs() == 1) {
                        bool willReact = ((float) (std::rand()/((RAND_MAX + 1u)/1000))/1000) <= reaction_assoc.at(x)->getProba();
                        if(willReact) {
                            // si se transforme en deux produits
                            if(reaction_assoc.at(x)->getNbProduits() == 2) {
                                int a = molecules.at(i)->getX() + radius;
                                int b = molecules.at(i)->getY() + radius;
                                int c = molecules.at(i)->getZ() + radius;

                                if(axe == 1 && a > 0 && a < diametre - 1 && grid[a - (dir + (dir-1))][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] == 0) 
                                    a -= (dir + (dir-1));
                                else if(axe == 1 && a > 0 && a < diametre - 1 && grid[a + (dir + (dir-1))][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] == 0) 
                                    a += (dir + (dir-1));
                                else if(axe == 2 && b > 0 && b < diametre - 1 && grid[molecules.at(i)->getX() + radius][b - (dir + (dir-1))][molecules.at(i)->getZ() + radius] == 0) 
                                    b -= (dir + (dir-1));
                                else if(axe == 2 && b > 0 && b < diametre - 1 && grid[molecules.at(i)->getX() + radius][b + (dir + (dir-1))][molecules.at(i)->getZ() + radius] == 0) 
                                    b += (dir + (dir-1));
                                else if(axe == 3 && c > 0 && c < diametre - 1 && grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][c - (dir + (dir-1))] == 0) 
                                    c -= (dir + (dir-1));
                                else if(axe == 3 && c > 0 && c < diametre - 1 && grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][c + (dir + (dir-1))] == 0) 
                                    c += (dir + (dir-1));

                                molecules.push_back(new Molecule(reaction_assoc.at(x)->getProduits()[1], a - radius, b - radius, c - radius));
                                molecules.back()->getEM()->setNbCopies(molecules.back()->getEM()->getNbCopies()+1);
                                grid[a][b][c] = molecules.back();
                            }
                            molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                            molecules.at(i) = new Molecule(reaction_assoc.at(x)->getProduits()[0], molecules.at(i)->getX(), molecules.at(i)->getY(), molecules.at(i)->getZ());
                            molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()+1);
                            grid[molecules.at(i)->getX() + radius][molecules.at(i)->getY() + radius][molecules.at(i)->getZ() + radius] = molecules.at(i);
                            hasReact = true;
                        }
                    }
                    x++;
                }
            }
            nb_mols = molecules.size();     // on actualise le nombre de molecules total existante
        }

        nEpoch++;
        exportCSV();
    }
}