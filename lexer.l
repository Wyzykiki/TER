%{
	#include "parser.hh"
	#include "string.h"
%}

ident           [A-Za-z_][A-Za-z_0-9]*
entier          [0-9]+
flottant        [0-9]+.[0-9]+
commentaire     "//".*"\n"

%option noyywrap

%%

{entier}    {   
                yylval.ival = atoi(yytext);
                return (INT);
            }

{flottant}  {
                yylval.fval = atof(yytext);
                return (FLOAT);
            }

{commentaire}  {}

"," return (COMMA);
";" return (SEMI);
"(" return (LP);
")" return (RP);
"=" return (EQ);
"->" return (ARROW);
"[" return (LB);
"]" return (RB);
"+" return (PLUS);
"taille" return (TAILLE);
"vitesse" return (VITESSE);
"init" return (INIT);
"espece" return (ESPECE);
"diametre" return (DIAM);

{ident} 	{   
				char *valIdent = (char *)calloc(strlen(yytext)+1,sizeof(char));
				strcpy(valIdent, yytext);
				yylval.text = valIdent;
				return (IDENT);
			}

%%

