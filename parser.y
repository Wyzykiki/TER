%{
    #include <iostream>
    #include <map>
    #include "especemoleculaire.h"
    #include "reaction.h"
    using namespace std;


    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
 
    void yyerror(const char *s);

    map<char*, EspeceMoleculaire> especes;
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

head: espece diametre   {  }
    | diametre espece   {  }
    ;

espece: ESPECE listesp SEMI {  }
    ;
    
listesp: IDENT { char* id = $1; especes[id] = EspeceMoleculaire(id);/*on ajoute la nouvelle espece moleculaire a la liste */}
    | IDENT COMMA listesp { char* id = $1; especes[id] = EspeceMoleculaire(id); }
    ;

diametre: DIAM EQ INT SEMI { const int DIAMETRE = $3; }
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
        char *reactif1 = (char*) $1;
        char *reactif2 = (char*) $2;
        char *produit1 = (char*) $4;
        char *produit2 = (char*) $5;
        float proba = $6;

        EspeceMoleculaire *reactifs, *produits;
        int size_reac = 1;
        if (reactif2 == "") {
            reactifs = (EspeceMoleculaire*) malloc(sizeof(EspeceMoleculaire));
            reactifs[0] = especes.find($1)->second;
        } else {
            reactifs = (EspeceMoleculaire*) malloc((sizeof(EspeceMoleculaire)));
            reactifs[0] = especes.find($1)->second;
            reactifs[1] = especes.find(reactif2)->second;
            size_reac = 2;
        }
        if (produit2 == "") {
            produits = (EspeceMoleculaire*) malloc(sizeof(EspeceMoleculaire));
            produits[0] = especes.find(produit1)->second;
        } else {
            produits = (EspeceMoleculaire*) malloc(sizeof(EspeceMoleculaire));
            produits[0] = especes.find(produit1)->second;
            produits[1] = especes.find(produit2)->second;
        }

        Reaction r(reactifs, produits, proba);

        // for (int i = 0; i< size_reac; i++) {
        //     reactifs[i].addReaction(r);
        // }
    }
    ;

prob: LB FLOAT RB { $$ = (float) $2; }
    ;

sr: /* empty */ { $$ = (char*) ""; }
    | PLUS IDENT { $$ = $2; }
    ;

taille: TAILLE LP IDENT RP EQ INT SEMI { (especes.find($3)->second).setTaille($6); }
    ;

vitesse: VITESSE LP IDENT RP EQ FLOAT SEMI { (especes.find($3)->second).setVitesse($6); }
    ;

init: INIT LP IDENT RP EQ INT SEMI { (especes.find($3)->second).setNbCopies($6); }
    ;

%%

void parse(char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        cout << "Can't open " << filename << endl;
        exit(-1);
    }
    yyin = file;
    yyparse();
}

void yyerror(const char *s) {
    cout << "Error : " << s << endl;
    exit(-1);
}