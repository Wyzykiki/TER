%{
    #include <iostream>
    #include "reaction.h"
    #include "simulation.h"
    
    using namespace std;


    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    extern int yylineno;
 
    void yyerror(const char *s);

    map<int, EspeceMoleculaire*> especes;
    int diametre;

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
    
listesp: IDENT { char* id = $1; especes[strToASCII(id)] = new EspeceMoleculaire(id);/*on ajoute la nouvelle espece moleculaire a la liste */}
    | IDENT COMMA listesp { char* id = $1; especes[strToASCII(id)] = new EspeceMoleculaire(id); }
    ;

diametre: DIAM EQ INT SEMI { diametre = $3; }
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

    /** Création de la structure avec les données utiles pour la simulation. */
    File_vars globals(diametre, especes_arr, esp_size);
    return globals;
}

/** Fonction appelee lors d'une erreur de syntaxe */
void yyerror(const char *s) {
    cout << "Line " << yylineno << " : " << s << endl;
    exit(-1);
}