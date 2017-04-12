/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
 %{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include"repl.h"
#include "ast.h"

/* prototypes */
int yylex(void);
extern char *yytext;
extern int yylineno;
void yyerror(struct _object **ast,char *s);
//int sym[26];                    /* symbol table */
%}

%union {
	int int_val;
	float float_val;
	char *s_val;
	struct _object *obj;
};

%token <int_val> INTEGER
%token <float_val> FLOAT
%token <s_val>  WORD FILENAME STRING
%token QUIT DEFUN DEFMACRO LET PRN TT NIL TYPE
%type <obj> number object sexpr fn sexprlist symbol expr op boolean
//<int_val> expr
//%left EQ
//%left '+' '-'
//%left '*' '/'

%start lisp
%parse-param{struct _object **ast}
%%

lisp:	sexprlist {*ast=$1;YYACCEPT;}

object:	symbol {$$=$1;}
		|boolean {$$=$1;}
		|expr {$$=$1;}

sexprlist: sexpr ';' sexprlist {$$=cons($1,cons($3,the_empty_list()));} | sexpr {$$=$1;}

sexpr: fn {$$=$1;} | object {$$=$1;}

fn: 	QUIT							{quit_shell=1;$$=NULL;YYACCEPT;}
		|LET							{printf("ciao\n");$$=new_atom_s("pippo");}
		|PRN INTEGER					{printf("%d\n",$2);$$=NULL;}
		|PRN FLOAT						{printf("%f\n",$2);$$=NULL;}
		|PRN WORD						{printf("%s\n",$2);$$=NULL;}
		|TYPE INTEGER					{printf("integer\n");$$=NULL;}
		|TYPE FLOAT						{printf("float\n");$$=NULL;}
		|TYPE WORD						{printf("string\n");$$=NULL;}
		|TYPE boolean					{printf("boolean\n");$$=NULL;}
		|TYPE symbol					{printf("symbol\n");$$=NULL;}

expr:	number op expr	{$$=cons($2,cons($1,cons($3,the_empty_list())));}
	|'(' expr ')' op expr	{$$=NULL;}
	| number		{$$=$1;}
	|	{$$=NULL;}

op: '+' {$$=new_atom_s("add");}|'-' {$$=new_atom_s("sub");}|'*' {$$=new_atom_s("mul");}|'/' {$$=new_atom_s("div");}

number:	INTEGER							{$$=new_atom_i($1);}
		|FLOAT							{$$=new_atom_f($1);}

symbol: WORD						{$$=new_atom_s($1);}

boolean:	TT							{$$=new_atom_b(1);}
		|NIL							{$$=new_atom_b(0);}

%%

void yyerror(struct _object **ast,char *s) {
    //fprintf(stdout, "%s\n", s);
    fprintf(stdout,"%s on line %d - %s\n", s, yylineno, yytext);
}
