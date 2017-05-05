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
%token <s_val> WORD STRING STRING2
%token QUIT IF WHILE LET PRN TT NIL TYPE
%type <obj> number object sexpr fn sexprlist symbol expr op boolean string func_application func_args blockcode LAMBDA_BODY LAMBDA_PARAMS lambda list listitems hash hashitems hashitem listpicker hashpicker bexpr MAYBEELSE
//<int_val> expr
//%left EQ
%left '+' '-'
%left '*' '/'
//%right '-'

%start lisp
%parse-param{struct _object **ast}
%%

lisp:	sexprlist {*ast=$1;YYACCEPT;}

object:	symbol {$$=$1;}
		|boolean {$$=$1;}
		|expr {$$=$1;}
		|string {$$=$1;}
		|lambda {$$=$1;}
		|list {$$=$1;}
		|hash {$$=$1;}

sexprlist: sexpr ';' sexprlist {$$=cons($1,$3);} | sexpr {$$=cons($1,the_empty_list());}

sexpr: fn {$$=$1;} | object {$$=$1;}

fn: 	QUIT							{quit_shell=1;$$=NULL;YYACCEPT;}
		|LET symbol '=' object						{$$=cons(new_atom_s("assign"),cons($2,cons($4,the_empty_list())));}
		|symbol '=' object {$$=cons(new_atom_s("assign"),cons($1,cons($3,the_empty_list())));}
		|LET symbol ':' object						{$$=cons(new_atom_s("assign"),cons($2,cons($4,the_empty_list())));}
		|symbol ':' object						{$$=cons(new_atom_s("assign"),cons($1,cons($3,the_empty_list())));}
		|PRN expr					{$$=cons(new_atom_s("prn"),cons($2,the_empty_list()));}
		|PRN WORD						{$$=cons(new_atom_s("prn"),cons(new_atom_s($2),the_empty_list()));}
		|PRN STRING						{$$=cons(new_atom_s("prn"),cons(new_atom_str($2),the_empty_list()));}
		|TYPE INTEGER					{printf("integer\n");$$=NULL;}
		|TYPE FLOAT						{printf("float\n");$$=NULL;}
		|TYPE WORD						{printf("word\n");$$=NULL;}
		|TYPE STRING						{printf("string\n");$$=NULL;}
		|TYPE boolean					{printf("boolean\n");$$=NULL;}
		|TYPE symbol					{printf("symbol\n");$$=NULL;}
		|IF '(' bexpr ')' sexpr MAYBEELSE {$$=cons(new_atom_s("if"),cons($3,cons($5,cons($6,the_empty_list()))));}
		|blockcode {$$=$1;}
		| {$$=the_empty_list();}

blockcode: '{' sexprlist '}' {$$=cons(new_atom_s("progn"),$2);}

expr:	expr op expr	{$$=cons($2,cons($1,cons($3,the_empty_list())));}
	|'(' expr ')'	{$$=$2;}
	| number		{$$=$1;}
	| '-' expr		{$$=cons(new_atom_s("neg"),cons($2,the_empty_list()));}
	| func_application {$$=$1;}
	| symbol {$$=$1;}
	| symbol '[' listpicker ']' {$$=cons(new_atom_s("get_list"),cons($3,cons($1,the_empty_list())));}
	| symbol '[' hashpicker ']' {$$=cons(new_atom_s("get_hash"),cons($3,cons($1,the_empty_list())));}

op: '+' {$$=new_atom_s("add");}|'-' {$$=new_atom_s("sub");}|'*' {$$=new_atom_s("mul");}|'/' {$$=new_atom_s("div");}

bexpr: WORD {$$=cons(new_atom_s("bexpr"),the_empty_list());}

MAYBEELSE: sexpr {$$=$1;}

number:	INTEGER							{$$=new_atom_i($1);}
		|FLOAT							{$$=new_atom_f($1);}

lambda:
	'\\' LAMBDA_PARAMS '.' LAMBDA_BODY {$$=cons(new_atom_s("lambda"),cons($2,cons($4,the_empty_list())));}

LAMBDA_PARAMS:
	symbol {$$=cons($1,the_empty_list());}
	|symbol ':' symbol {$$=cons($1,cons($3,the_empty_list()));}
	|symbol ',' LAMBDA_PARAMS {$$=cons($1,$3);}

LAMBDA_BODY:
	sexpr {$$=$1;}

func_application: WORD '(' func_args ')' {/*$$=cons(new_atom_s("apply"),cons(new_atom_s($1),$3));*/$$=cons(new_atom_s($1),$3);}

func_args: expr {$$=cons($1,the_empty_list());}
	|expr ',' func_args {$$=cons($1,$3);}
	|{$$=the_empty_list();}

list: '[' listitems ']' {$$=cons(new_atom_s("list"),$2);}

listitems: object {$$=cons($1,the_empty_list());}
	| object ',' listitems {$$=cons($1,$3);}

hash: '{' hashitems '}' {$$=cons(new_atom_s("hash"),$2);}

hashitem: string ':' object {$$=cons($1,cons($3,the_empty_list()));}

hashitems: hashitem {$$=cons($1,the_empty_list());}
	|hashitem ',' hashitems {$$=cons($1,$3);}

listpicker:
	expr {$$=$1;}
	|expr ':' expr {$$=cons(new_atom_s("list_range"),cons($1,cons($3,the_empty_list())));}
	| ':' expr {$$=cons(new_atom_s("list_range"),cons(new_atom_i(0),cons($2,the_empty_list())));}
	| expr ':' {$$=cons(new_atom_s("list_range"),cons($1,cons(new_atom_i(-1),the_empty_list())));}

hashpicker:
	string {$$=$1;}

symbol: WORD						{$$=new_atom_s($1);}

string: STRING {$$=new_atom_str($1);}
	|STRING2 {$$=new_atom_str2($1);}

boolean:	TT							{$$=new_atom_b(1);}
		|NIL							{$$=new_atom_b(0);}

%%

void yyerror(struct _object **ast,char *s) {
    //fprintf(stdout, "%s\n", s);
    fprintf(stdout,"%s on line %d - %s\n", s, yylineno, yytext);
}
