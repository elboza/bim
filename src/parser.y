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
#include "env.h"
#include "special_vars.h"

/* prototypes */
int yylex(void);
extern char *yytext;
extern int yylineno;
void yyerror(struct _object **ast,char *s);
%}

%union {
	int int_val;
	float float_val;
	char *s_val;
	struct _object *obj;
};

/*%glr-parser*/

%token <int_val> INTEGER
%token <float_val> FLOAT
%token <s_val> WORD STRING STRING2
%token QUIT PRN TT NIL BEGIN_LISP_SYM END_LISP_SYM LAST_EVAL_VAL IF WHILE ELSE TYPE LET FUNC APPLY BIND_SYM MAPPL_SYM NAMESPACE_SYM 
%type <obj> object bim_expr_list bim_expr expr bexpr LISP LISP_SEXPR LISP_LIST LISP_ITEM number string fn symbol boolean blockcode MAYBEELSE comma_list func_application functions LAMBDA_BODY LAMBDA_PARAM LAMBDA_PARAMS_CLJ lambda_CLJ lambda_single math_expr 
%type <obj> BIND_APPL
%type <obj> list listitems_orempty listitems list_sel_item listpicker
%type <obj> hash hashitems hashitem hashitems_orempty hashpicker hashpicker_str hashpicker_list hashpicker_list_dot hashpicker_list_bracket
%type <obj> namespace
//%left EQ
%right APPLY
%nonassoc THEN
%nonassoc ELSE
%left AND OR
%left '<' '>' EQ NEQ LE GE
%precedence NOT
%left '+' '-'
%left '*' '/' '%' REMINDER
%left '&' '|' rot_l rot_r shift_l shift_r b_xor
%right POW
%precedence NEG
//%left ','

%start bim
%parse-param{struct _object **ast}
%%

bim:
	bim_expr_list {*ast=$1;YYACCEPT;}

object:	
	bexpr {$$=$1;}
	|expr {$$=$1;}
	|string {$$=$1;}
	|functions {$$=$1;}
	|list {$$=$1;}
	|hash {$$=$1;}
	|BEGIN_LISP_SYM LISP END_LISP_SYM {$$=cons(new_atom_s("__progn__"),$2);}

bim_expr_list: 
	bim_expr ';' bim_expr_list {$$=cons($1,$3);} 
	| bim_expr {$$=cons($1,new_empty_list());}
	| {$$=new_empty_list();}

bim_expr: 
	fn {$$=$1;} 
	| object {$$=$1;}

fn: 	
	QUIT	{quit_shell=1;$$=NULL;YYACCEPT;}
	|LET namespace '=' object	{$$=cons(new_atom_s("__assign__"),cons($2,cons($4,new_empty_list())));}
	|namespace '=' object {$$=cons(new_atom_s("__assign__"),cons($1,cons($3,new_empty_list())));}
	|LET namespace ':' object	{$$=cons(new_atom_s("__assign__"),cons($2,cons($4,new_empty_list())));}
	|namespace ':' object		{$$=cons(new_atom_s("__assign__"),cons($1,cons($3,new_empty_list())));}
	|LET list_sel_item '=' object {$$=cons(new_atom_s("__set_list__"),cons($2,cons($4,new_empty_list())));}
	|list_sel_item '=' object {$$=cons(new_atom_s("__set_list__"),cons($1,cons($3,new_empty_list())));}
	|LET hashpicker_list '=' object {$$=cons(new_atom_s("__set_hash__"),cons($2,cons($4,new_empty_list())));}
	|hashpicker_list '=' object {$$=cons(new_atom_s("__set_hash__"),cons($1,cons($3,new_empty_list())));}
	|PRN comma_list			{$$=cons(new_atom_s("__prn__"),$2);}
	|TYPE bim_expr				{$$=cons(new_atom_s("__type__"),cons($2,new_empty_list()));}
	|IF '(' bexpr ')' bim_expr MAYBEELSE {$$=cons(new_atom_s("__if__"),cons($3,cons($5,cons($6,new_empty_list()))));}
	|WHILE '(' bexpr ')' bim_expr {$$=cons(new_atom_s("__while__"),cons($3,cons($5,new_empty_list())));}
	|blockcode {$$=$1;}

blockcode: 
	'{' bim_expr_list '}' {$$=cons(new_atom_s("__progn__"),$2);}

math_expr: number		{$$=$1;}
	| namespace {$$=$1;}
	/*|expr op expr	{$$=cons($2,cons($1,cons($3,new_empty_list())));}*/
	|expr '+' expr	{$$=cons(new_atom_s("__add__"),cons($1,cons($3,new_empty_list())));}
	|expr '-' expr	{$$=cons(new_atom_s("__sub__"),cons($1,cons($3,new_empty_list())));}
	|expr '*' expr	{$$=cons(new_atom_s("__mul__"),cons($1,cons($3,new_empty_list())));}
	|expr '/' expr	{$$=cons(new_atom_s("__div__"),cons($1,cons($3,new_empty_list())));}
	|expr '%' expr	{$$=cons(new_atom_s("__mod__"),cons($1,cons($3,new_empty_list())));}
	|expr REMINDER expr	{$$=cons(new_atom_s("__reminder__"),cons($1,cons($3,new_empty_list())));}
	|expr '&' expr	{$$=cons(new_atom_s("__b_and__"),cons($1,cons($3,new_empty_list())));}
	|expr '|' expr	{$$=cons(new_atom_s("__b_or__"),cons($1,cons($3,new_empty_list())));}
	|expr b_xor expr	{$$=cons(new_atom_s("__b_xor__"),cons($1,cons($3,new_empty_list())));}
	|expr rot_l expr	{$$=cons(new_atom_s("__rot_l__"),cons($1,cons($3,new_empty_list())));}
	|expr rot_r expr	{$$=cons(new_atom_s("__rot_r__"),cons($1,cons($3,new_empty_list())));}
	|expr shift_l expr	{$$=cons(new_atom_s("__shift_l__"),cons($1,cons($3,new_empty_list())));}
	|expr shift_r expr	{$$=cons(new_atom_s("__shift_r__"),cons($1,cons($3,new_empty_list())));}
	|expr POW expr	{$$=cons(new_atom_s("__pow__"),cons($1,cons($3,new_empty_list())));}
	|'(' expr ')'	{$$=$2;}
	| '-' expr	%prec NEG	{$$=cons(new_atom_s("__neg__"),cons($2,new_empty_list()));}

expr: math_expr {$$=$1;}
	| func_application {$$=$1;}


/*op: '+' {$$=new_atom_s("add");}|'-' {$$=new_atom_s("sub");}|'*' {$$=new_atom_s("mul");}|'/' {$$=new_atom_s("div");}*/

bexpr: 
	boolean {$$=$1;}
	/*|expr {$$=$1;}*/
	|'(' bexpr ')' {$$=$2;}
	|bexpr AND bexpr {$$=cons(new_atom_s("__and__"),cons($1,cons($3,new_empty_list())));}
	|bexpr OR bexpr {$$=cons(new_atom_s("__or__"),cons($1,cons($3,new_empty_list())));}
	|expr '<' expr {$$=cons(new_atom_s("__lt__"),cons($1,cons($3,new_empty_list())));}
	|expr '>' expr {$$=cons(new_atom_s("__gt__"),cons($1,cons($3,new_empty_list())));}
	|expr LE expr {$$=cons(new_atom_s("__le__"),cons($1,cons($3,new_empty_list())));}
	|expr GE expr {$$=cons(new_atom_s("__ge__"),cons($1,cons($3,new_empty_list())));}
	|expr EQ expr {$$=cons(new_atom_s("__eq__"),cons($1,cons($3,new_empty_list())));}
	|expr NEQ expr {$$=cons(new_atom_s("__neq__"),cons($1,cons($3,new_empty_list())));}
	|bexpr EQ bexpr {$$=cons(new_atom_s("__eq__"),cons($1,cons($3,new_empty_list())));}
	|bexpr NEQ bexpr {$$=cons(new_atom_s("__neq__"),cons($1,cons($3,new_empty_list())));}
	|'!' bexpr  %prec NOT {$$=cons(new_atom_s("__not__"),cons($2,new_empty_list()));}

func_application: 
	namespace '(' comma_list ')' {$$=cons($1,$3);}
	| MAPPL_SYM BIND_APPL {$$=$2;}

BIND_APPL:
	object {$$=$1;}
	| BIND_APPL APPLY '(' object ')' {$$=cons($1,cons($4,new_empty_list()));}
	| BIND_APPL '.' hashpicker {$$=cons(new_atom_s("__get_hash__"),cons($3,cons($1,new_empty_list())));}
	| BIND_APPL '[' listpicker ']' {$$=cons(new_atom_s("__get_list__"),cons($3,cons($1,new_empty_list())));}
	| BIND_APPL BIND_SYM symbol {$$=cons($3,cons($1,new_empty_list()));}
	| BIND_APPL BIND_SYM functions {$$=cons($3,cons($1,new_empty_list()));}
	| BIND_APPL ',' object {$$=cons($1,cons($3,new_empty_list()));}

MAYBEELSE: 
	ELSE bim_expr %prec ELSE{$$=$2;}
	| %prec THEN{$$=new_empty_list();}

number:	
	INTEGER {$$=new_atom_i($1);}
	|FLOAT {$$=new_atom_f($1);}

functions:
	'\\' comma_list '.' LAMBDA_BODY {$$=cons(new_atom_s("__lambda__"),cons($2,cons($4,new_empty_list())));}
	|lambda_CLJ {$$=$1;}

lambda_CLJ:
	'~' '\\' LAMBDA_PARAMS_CLJ {$$=$3;}

lambda_single:
	LAMBDA_PARAM '.' LAMBDA_BODY {$$=cons(new_atom_s("__lambda__"),cons($1,cons($3,new_empty_list())));}

LAMBDA_PARAM:
	symbol {$$=cons($1,new_empty_list());}

LAMBDA_PARAMS_CLJ:
	lambda_single {$$=$1;} 
	|LAMBDA_PARAM ',' LAMBDA_PARAMS_CLJ {$$=cons(new_atom_s("__lambda__"),cons($1,cons($3,new_empty_list())));}
	|LAMBDA_PARAM {$$=$1;}

LAMBDA_BODY:
	bim_expr {$$=$1;}

list: 
	'[' listitems_orempty ']' {$$=cons_dl(new_atom_s("__list__"),$2);}

listitems: 
	object {$$=cons_dl($1,new_empty_list());}
	| object ',' listitems {$$=cons_dl($1,$3);}

listitems_orempty:
	/*empty*/ {$$=new_empty_list();}
	|listitems {$$=$1;}

listpicker:
	expr {$$=$1;}
	|expr ':' expr {$$=cons(new_atom_s("__list_range__"),cons($1,cons($3,new_empty_list())));}
	| ':' expr {$$=cons(new_atom_s("__list_range__"),cons(new_atom_i(0),cons($2,new_empty_list())));}
	| expr ':' {$$=cons(new_atom_s("__list_range__"),cons($1,cons(new_atom_i(-1),new_empty_list())));}

list_sel_item:
	list_sel_item '['  listpicker ']' {$$=cons(new_atom_s("__get_list__"),cons($3,cons($1,new_empty_list())));}
	| namespace {$$=$1;}
	| func_application {$$=$1;}

hash: 
	'{' hashitems_orempty '}' {$$=cons_dl(new_atom_s("__hash__"),$2);}

hashitem: 
	string ':' object {$$=cons_dl($1,cons_dl($3,new_empty_list()));}
	| WORD ':' object {$$=cons_dl(new_atom_str_Q($1),cons_dl($3,new_empty_list()));}

hashitems: 
	hashitem {$$=cons_dl($1,new_empty_list());}
	|hashitem ',' hashitems {$$=cons_dl($1,$3);}

hashitems_orempty:
	/*empty*/ {$$=new_empty_list();}
	|hashitems {$$=$1;}

hashpicker:
	WORD {$$=new_atom_str_Q($1);}

hashpicker_str:
	string {$$=$1;}

hashpicker_list_dot:
	hashpicker_list_dot '.' hashpicker {$$=cons(new_atom_s("__get_hash__"),cons($3,cons($1,new_empty_list())));}
	| namespace {$$=$1;}
	| func_application {$$=$1;}

hashpicker_list_bracket:
	hashpicker_list_bracket '~' '['  hashpicker_str ']' {$$=cons(new_atom_s("__get_hash__"),cons($4,cons($1,new_empty_list())));}
	| namespace {$$=$1;}
	| func_application {$$=$1;}

hashpicker_list:
	hashpicker_list_dot {$$=$1;}
	|hashpicker_list_bracket {$$=$1;}


LISP:
	{$$=new_empty_list();}
	| LISP_SEXPR LISP {$$=cons($1,$2);}
	

LISP_SEXPR:
	LISP_ITEM {$$=$1;}
	|LISP_LIST {$$=$1;}

LISP_LIST:
	'(' LISP ')' {$$=$2;}

LISP_ITEM:
	namespace {$$=$1;}
	|number {$$=$1;}
	|string {$$=$1;}

namespace:
	symbol {$$=$1;}
	| namespace NAMESPACE_SYM symbol {$$=cons(new_atom_s("__namespace__"),cons($1,cons($3,new_empty_list())));}								 

comma_list: 
	bim_expr {$$=cons($1,new_empty_list());}
	| bim_expr ',' comma_list {$$=cons($1,$3);}

symbol: 
	WORD {$$=new_atom_s($1);}
	| LAST_EVAL_VAL {$$=new_atom_s("$!");}
	|'^' WORD {$$=cons(new_atom_s("__global__"),cons(new_atom_s($2),new_empty_list()));}

string: 
	STRING {$$=new_atom_str_QQ($1);}

boolean:	
	TT {$$=new_atom_b(1);}
	|NIL {$$=new_atom_b(0);}

%%

void yyerror(struct _object **ast,char *s) {
    //fprintf(stdout, "%s\n", s);
    fprintf(stdout,"%s on line %d - %s\n", s, yylineno, yytext);
}
