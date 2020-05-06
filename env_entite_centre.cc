#include "env_entite_centre.h"
#include <stdio.h>

Env_entite_centre::Env_entite_centre(EspeceMoleculaire* ems[], int size, int diam) : Simulation(ems, size){
    diametre = diam;

    nb_mols = 0;
    for(int i = 0 ; i < size ; i++) {
        nb_mols += ems[i]->getNbCopies();
    }

    grid = new Molecule***[diam];
    for(int i = 0 ; i < diam ; i++){
        grid[i] = new Molecule**[diam];
        for(int j = 0 ; j < diam ; j++){
            grid[i][j] = new Molecule*[diam];
            for(int k = 0 ; k < diam ; k++) {
                grid[i][j][k] = 0;
            }
        }
    }
 
    srand(time(NULL));

    for(int i = 0; i < size ; i++){
        for(int j = 0 ; j < ems[i]->getNbCopies() ; j++){
            int x, y, z;
            do {
            x = std::rand()/((RAND_MAX + 1u)/diam);
            y = std::rand()/((RAND_MAX + 1u)/diam);
            z = std::rand()/((RAND_MAX + 1u)/diam);
            } while(grid[x][y][z] != 0);
            molecules.push_back(new Molecule(ems[i], x, y, z));
            grid[x][y][z] = molecules.back();
        }
    }

    shuffle (molecules.begin(), molecules.end(), std::default_random_engine((unsigned) time(NULL)));
}

Env_entite_centre::~Env_entite_centre() {
    
}


void Env_entite_centre::run(){
    exportCSV();
    srand(time(NULL));

    while (nEpoch<300000){
        for(int i = 0 ; i < nb_mols ; i++) {
            // for (int k=0; k<nb_mols; k++)
            //     std::cout << "OOF " << molecules.at(k)->getEM()->getNom() << std::endl;
            //on calcule un deplacement aleatoire
            
            /*float tauxX = std::rand();
            float tauxY = std::rand();
            float tauxZ = std::rand();

            tauxX = tauxX / (tauxX + tauxY + tauxZ);
            tauxY = tauxY / (tauxX + tauxY + tauxZ);
            tauxZ = tauxZ / (tauxX + tauxY + tauxZ);

            int moveX = molecules.at(i).getEM()->getVitesse() * tauxX;
            int moveY = molecules.at(i).getEM()->getVitesse() * tauxY;
            int moveZ = molecules.at(i).getEM()->getVitesse() * tauxZ;*/

            // la molecule ne se deplace que sur un axe durant une periode, on choisi donc son axe et calcule se nouvelle position
            int axe = 1 + std::rand()/((RAND_MAX + 1u)/3);
            // On choisi si elle "avance" ou "recule" sur cet axe : 0 elle recule, 1 elle avance
            int dir = std::rand()/((RAND_MAX + 1u)/2);

            // On récupère la position de la molécule
            int newPosX = molecules.at(i)->getX();
            int newPosY = molecules.at(i)->getY();
            int newPosZ = molecules.at(i)->getZ();

            bool hasEncountered = false;

            // Comme la grille est entiere, on calcule la probabilite de se deplacer d'une case de plus basee sur la decimale de la vitesse
            long ent = (long) molecules.at(i)->getEM()->getVitesse();
            int dig = (int)((molecules.at(i)->getEM()->getVitesse()-ent)*10 + 0.5);
            //bool willMoveDigit = std::rand()/((RAND_MAX + 1u)/10) <= dig;

            // On calcule la nouvelle position de la molecule après déplacement
            if(axe == 1) newPosX += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) /*+ ((willMoveDigit) ? 1 : 0)*/;
            else if(axe == 2) newPosY += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) /*+ ((willMoveDigit) ? 1 : 0)*/;
            else if(axe == 3) newPosZ += molecules.at(i)->getEM()->getVitesse()*dir + molecules.at(i)->getEM()->getVitesse()*(dir-1) /*+ ((willMoveDigit) ? 1 : 0)*/;

            
            //on verifie que la molecule ne sorte pas de la zone en se deplacant
            if(newPosX >= 0 && newPosX < diametre && newPosY >= 0 && newPosY < diametre && newPosZ >= 0 && newPosZ < diametre) {
                //si collision avec une autre dans la grille elle arrête de se deplacer et retiens avec qui il y a eu collision
                Molecule* mol_encountered = new Molecule(*molecules.at(i));

                if(axe == 1){
                    int a = molecules.at(i)->getX() + ((dir==0)?-1:1);
                    while(((dir == 0) ? a >= newPosX : a <= newPosX) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[a][newPosY][newPosZ] != 0);
                        if(hasEncountered) mol_encountered = (grid[a][newPosY][newPosZ]);
                        if(dir == 0) a--;
                        else a++;
                    }
                } else if(axe == 2){
                    int b = molecules.at(i)->getY() + ((dir==0)?-1:1);
                    while(((dir == 0) ? b >= newPosY : b <= newPosY) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[newPosX][b][newPosZ] != 0);
                        if(hasEncountered) mol_encountered = (grid[newPosX][b][newPosZ]);
                        if(dir == 0) b--;
                        else b++;
                    }
                } else if (axe == 3){
                    int c = molecules.at(i)->getZ() + ((dir==0)?-1:1);
                    while(((dir == 0) ? c >= newPosZ : c <= newPosZ) && !hasEncountered){
                        hasEncountered = hasEncountered || (grid[newPosX][newPosY][c] != 0);
                        if(hasEncountered) mol_encountered = (grid[newPosX][newPosY][c]);
                        if(dir == 0) c--;
                        else c++;
                    }
                }
                //chance de reagir en fonction de molecule croisee et proba
                if(hasEncountered){
                    int reac = molecules.at(i)->getEM()->getPosActualReaction(mol_encountered->getEM());
                    if(reac != -1){
                        bool willReact = ((float) (std::rand()/((RAND_MAX + 1u)/1000))/1000) <= molecules.at(i)->getEM()->getReaction(reac)->getProba();
                        if(willReact){
                            if(molecules.at(i)->getEM()->getReaction(reac)->getNbProduits() == 1) {
                                std::vector<Molecule*>::iterator posMolEnc = std::find_if(molecules.begin(), molecules.end(), [&mol_encountered](Molecule *mol){return mol_encountered->getX() == mol->getX() && mol_encountered->getY() == mol->getY() && mol_encountered->getZ() == mol->getZ();});
                                std::cout << "react 1" << std::endl;
                                if((*posMolEnc)->getEM()->getNbCopies() == 0){
                                    std::cout << "em(enc) : " << (*posMolEnc)->getEM()->getNom() << ", copies : " <<  (*posMolEnc)->getEM()->getNbCopies() << std::endl;
                                    for (int a = 0 ; a < molecules.size() ; a++) {
                                        if(strcmp(molecules.at(a)->getEM()->getNom(), (*posMolEnc)->getEM()->getNom()) == 0 && molecules.at(a)->getX() == (*posMolEnc)->getX() && molecules.at(a)->getY() == (*posMolEnc)->getY()&& molecules.at(a)->getZ() == (*posMolEnc)->getZ()){
                                            std::cout << "trouvé" << std::endl;
                                        }
                                    }
                                }

                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()-1);
                                *posMolEnc = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[0], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ());
                                grid[mol_encountered->getX()][mol_encountered->getY()][mol_encountered->getZ()] = (*posMolEnc);
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()+1);

                                grid[molecules.at(i)->getX()][molecules.at(i)->getY()][molecules.at(i)->getZ()] = 0;
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                                /*if(molecules.at(i)->getEM()->getNbCopies() < 0){
                                    std::cout << "em(mol) : " << molecules.at(i)->getEM()->getNom() << ", copies : " <<  molecules.at(i)->getEM()->getNbCopies() << std::endl;
                                    for (int a = 0 ; a < diametre ; a++) {
                                        for (int b = 0 ; b < diametre ; b++) {
                                            for (int c = 0 ; c < diametre ; c++) {
                                                if(grid[a][b][c] != 0 && strcmp(grid[a][b][c]->getEM()->getNom(), molecules.at(i)->getEM()->getNom()) == 0 && a == molecules.at(i)->getX() && b == molecules.at(i)->getY() && c == molecules.at(i)->getZ()){
                                                    std::cout << "trouvé" << std::endl;
                                                }
                                            }
                                        }
                                    }
                                }*/
                                molecules.erase(std::remove(molecules.begin(), molecules.end(), molecules.at(i)), molecules.end());
                            }
                            /*else {
                                grid[molecules.at(i)->getX()][molecules.at(i)->getY()][molecules.at(i)->getZ()] = 0;
                                std::vector<Molecule*>::iterator posMolEnc = std::find_if(molecules.begin(), molecules.end(), [&mol_encountered](Molecule *mol){return mol_encountered->getX() == mol->getX() && mol_encountered->getY() == mol->getY() && mol_encountered->getZ() == mol->getZ();});
                                std::cout << "reac 2" << std::endl;
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()-1);
                                *posMolEnc = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[0], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ());
                                (*posMolEnc)->getEM()->setNbCopies((*posMolEnc)->getEM()->getNbCopies()+1);
                                grid[(*posMolEnc)->getX()][(*posMolEnc)->getY()][(*posMolEnc)->getZ()] = *posMolEnc;
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                                if(axe == 1) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX()+dir+(dir-1), mol_encountered->getY(), mol_encountered->getZ());
                                    grid[mol_encountered->getX()+dir+(dir-1)][mol_encountered->getY()][mol_encountered->getZ()] = molecules.at(i);
                                }
                                else if(axe == 2) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX(), mol_encountered->getY()+dir+(dir-1), mol_encountered->getZ());
                                    grid[mol_encountered->getX()][mol_encountered->getY()+dir+(dir-1)][mol_encountered->getZ()] = molecules.at(i);
                                }
                                else if(axe == 3) {
                                    molecules.at(i) = new Molecule(molecules.at(i)->getEM()->getReaction(reac)->getProduits()[1], mol_encountered->getX(), mol_encountered->getY(), mol_encountered->getZ()+dir+(dir-1));
                                    grid[mol_encountered->getX()][mol_encountered->getY()][mol_encountered->getZ()+dir+(dir-1)] = molecules.at(i);
                                }
                                molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()+1);
                            }*/
                        }
                    }
                }
                //si c'est bon elle se deplace
                else if(grid[newPosX][newPosY][newPosZ] == 0){
                    grid[molecules.at(i)->getX()][molecules.at(i)->getY()][molecules.at(i)->getZ()] = 0;
                    molecules.at(i)->setPos(newPosX, newPosY, newPosZ);
                    grid[newPosX][newPosY][newPosZ] = molecules.at(i);
                }
            }
            // Si n'a pas réagi avec une autre espèce, a toujours la possibilité de réagir tout seul
            if(!hasEncountered){
                std::vector<Reaction*> reaction_assoc;
                reaction_assoc = *(molecules.at(i)->getEM()->getReactions());
                shuffle (reaction_assoc.begin(), reaction_assoc.end(), std::default_random_engine((unsigned) time(NULL)));

                int x = 0;
                bool hasReact = false;

                while (x < reaction_assoc.size() && !hasReact) {
                    if(reaction_assoc.at(x)->getNbReactifs() == 1) {
                        bool willReact = ((float) (std::rand()/((RAND_MAX + 1u)/1000))/1000) <= reaction_assoc.at(x)->getProba();
                        if(willReact) {
                            std::cout << "react solo" << std::endl;
                            if(reaction_assoc.at(x)->getNbProduits() == 2) {
                                int a = molecules.at(i)->getX();
                                int b = molecules.at(i)->getY();
                                int c = molecules.at(i)->getZ();

                                if(axe == 1) a -= (dir + (dir-1));
                                else if(axe == 2) b -= (dir + (dir-1));
                                else if(axe == 3) c -= (dir + (dir-1));

                                molecules.push_back(new Molecule(reaction_assoc.at(x)->getProduits()[1], a, b, c));
                                molecules.back()->getEM()->setNbCopies(molecules.back()->getEM()->getNbCopies()+1);
                                grid[a][b][c] = molecules.back();
                            }
                            molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()-1);
                            molecules.at(i) = new Molecule(reaction_assoc.at(x)->getProduits()[0], molecules.at(i)->getX(), molecules.at(i)->getY(), molecules.at(i)->getZ());
                            molecules.at(i)->getEM()->setNbCopies(molecules.at(i)->getEM()->getNbCopies()+1);
                            grid[molecules.at(i)->getX()][molecules.at(i)->getY()][molecules.at(i)->getZ()] = molecules.at(i);
                            hasReact = true;
                        }
                    }
                    x++;
                }
            }
            nb_mols = molecules.size();
        }

        nEpoch++;
        exportCSV();
    }
}