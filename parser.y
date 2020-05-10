%{
    #include <iostream>
    #include <cmath>
    #include "reaction.h"
    #include "simulation.h"
    
    using namespace std;


    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int yylineno;
 
    void yyerror(const char *s);

    const float PI = 3.141592653589793;
    const float NB_AVOG = 6.02214076;

    map<int, EspeceMoleculaire*> especes;
    float diametre;
    float volume;
    Reaction** tmpReactions = new Reaction*[100] ;
    int nbReactions = 0;

    /** Compute an int from a c-string
     *  @result is the sum of caracter's ASCII codes multiplied by their positions
     */
    int strToASCII(char * str);

%}

%union {
    int ival;
    float fval;
    char* text;
}


%token <text>IDENT
%token <ival>INT
%token <fval>FLOAT
%token COMMA SEMI LP RP EQ ARROW LB RB PLUS
%token ESPECE DIAM TAILLE VITESSE INIT

%type <fval> prob
%type <text> sr

%start start
%%

start: head body {}
    ;

head: espece diametre {}
    | diametre espece {}
    ;

espece: ESPECE listesp SEMI {}
    ;
    
listesp: IDENT { char* id = $1; especes[strToASCII(id)] = new EspeceMoleculaire(id); }
    | IDENT COMMA listesp { char* id = $1; especes[strToASCII(id)] = new EspeceMoleculaire(id); /*on ajoute la nouvelle espece moleculaire a la liste */}
    ;

diametre: DIAM EQ FLOAT SEMI { diametre = $3; volume = 1./6. * PI * pow(diametre, 3.); }
    ;

body: instr body {}
    | instr {}
    ;

instr: reaction {}
    | taille {}
    | vitesse {}
    | init {}
    ;

reaction: IDENT sr ARROW IDENT sr prob SEMI {
        char *reactif1 = $1;
        char *reactif2 = $2;
        char *produit1 = $4;
        char *produit2 = $5;
        float proba = $6;

        EspeceMoleculaire **reactifs;
        EspeceMoleculaire **produits;
        int size_reac(1);
        int size_prod(1);

        if (reactif2 == "") {
            reactifs = (EspeceMoleculaire**) malloc(sizeof(EspeceMoleculaire*));
            reactifs[0] = especes[strToASCII(reactif1)];
        } else {
            reactifs = (EspeceMoleculaire**) malloc(2*(sizeof(EspeceMoleculaire*)));
            reactifs[0] = especes[strToASCII(reactif1)];
            reactifs[1] = especes[strToASCII(reactif2)];
            size_reac = 2;
        }
        if (produit2 == "") {
            produits = (EspeceMoleculaire**) malloc(sizeof(EspeceMoleculaire*));
            produits[0] = especes[strToASCII(produit1)];
        } else {
            produits = (EspeceMoleculaire**) malloc(2*(sizeof(EspeceMoleculaire*)));
            produits[0] = especes[strToASCII(produit1)];
            produits[1] = especes[strToASCII(produit2)];
            size_prod = 2;
        }
        
        Reaction* r = new Reaction(reactifs, produits, size_reac, size_prod, proba);

        tmpReactions[nbReactions] = r;
        nbReactions++;
    }
    ;

prob: LB FLOAT RB { $$ = (float) $2; }
    ;

sr: /* empty */ { $$ = (char*) ""; }
    | PLUS IDENT { $$ = $2; }
    ;

taille: TAILLE LP IDENT RP EQ INT SEMI { char* id = $3; especes[strToASCII(id)]->setTaille($6); }
    ;

vitesse: VITESSE LP IDENT RP EQ FLOAT SEMI { char* id = $3; especes[strToASCII(id)]->setVitesse($6); }
    ;

init: INIT LP IDENT RP EQ INT SEMI { char* id = $3; especes[strToASCII(id)]->setNbCopies($6); }
	| INIT LP IDENT RP EQ INT IDENT SEMI { 
        char* id = $3;
        int concentration = $6;
        char* unite = $7;
        if(strToASCII(unite) == strToASCII("uM")){
            int nbMol = concentration * volume * NB_AVOG * 100;
            especes[strToASCII(id)]->setNbCopies(nbMol);
        } else if (strToASCII(unite) == strToASCII("mM")) {
            int nbMol = concentration * volume * NB_AVOG * 100 * 1000;
            especes[strToASCII(id)]->setNbCopies(nbMol);
        }
	}
	| INIT LP IDENT RP EQ FLOAT IDENT SEMI { 
        char* id = $3;
        float concentration = $6;
        char* unite = $7;
        if(strToASCII(unite) == strToASCII("uM")){
            int nbMol = concentration * volume * NB_AVOG * 100;
            especes[strToASCII(id)]->setNbCopies(nbMol);
        } else if (strToASCII(unite) == strToASCII("mM")) {
            int nbMol = concentration * volume * NB_AVOG * 100 * 1000;
            especes[strToASCII(id)]->setNbCopies(nbMol);
        }
	}
    ;

%%

int strToASCII(char * str) {
        int result = 0;
        for(int i = 0 ; i < strlen(str) ; i++){
            result += str[i]*(i+1);
        }
        return result;
}

/** Verifie la somme des propensions des reactions d'une espece moleculaire */
void checkReaction(EspeceMoleculaire** especes, int n) {
    for (int i = 0; i < n; i++) {        
        vector<Reaction*>* reactions = especes[i]->getReactions();
        float total = 0.0;
        for (int j=0; j<reactions->size(); j++)
            total += (*reactions)[j]->getProba();
        if (total > 1.0) {
            throw IncorrectProbabilityRates(especes[i]->getNom());
        }
    }
}

/** Attribue une couleur à chaque espèce */
void initColors(EspeceMoleculaire** especes, int n) {
    int batches = (n-1)/7 + 1;

    for (int i=0; i<n; i++) {
        int color = 255 * (batches - i/7) / batches;

        int nthInBatch = i%7 + 1;
        int r = nthInBatch & 0b100;
        int g = nthInBatch & 0b010;
        int b = nthInBatch & 0b001;
        if (r)
            especes[i]->setR(color);
        if (g)
            especes[i]->setG(color);
        if (b)
            especes[i]->setB(color);
    }
}

/** Extrait les donnees utiles du fichier fournie et creer les objets necessaires pour la simulation */
File_vars parse(char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        cout << "Can't open " << filename << endl;
        exit(-1);
    }
    yyin = file;
    yyparse();

    int esp_size = especes.size();
    EspeceMoleculaire** especes_arr = (EspeceMoleculaire**) malloc(esp_size * sizeof(EspeceMoleculaire*));
    
    int i = 0;
    for (auto& tpl : especes) {
        especes_arr[i] = tpl.second;
        i++;
    }

    try {
        checkReaction(especes_arr, esp_size);
    } catch (exception& e) {
        cerr<<e.what()<<endl;
        exit(-1);
    }

    initColors(especes_arr, esp_size);

    Reaction** reactions = new Reaction*[nbReactions];
    for (int i=0; i<nbReactions; i++) {
        reactions[i] = tmpReactions[i];
    }

    /** Création de la structure avec les données utiles pour la simulation. */
    File_vars globals(diametre, volume, especes_arr, esp_size, reactions, nbReactions);
    return globals;
}

/** Fonction appelee lors d'une erreur de syntaxe */
void yyerror(const char *s) {
    cout << "Line " << yylineno << " : " << s << endl;
    exit(-1);
}