/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ast.h"

_object* new_object(void){
	_object *obj;
	obj=(_object*)malloc(sizeof(_object));
	if(!obj) return NULL;
	obj->sval=NULL;
	obj->car=NULL;
	obj->cdr=NULL;
	return obj;
}
_object* new_atom_i(int ival){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_integer;
	obj->ival=ival;
	return obj;
}
_object* new_atom_b(int ival){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_boolean;
	obj->ival=ival;
	return obj;
}
_object* new_atom_f(float fval){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_float;
	obj->fval=fval;
	return obj;
}
_object* new_atom_s(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_symbol;
	obj->sval=strdup(s);
	return obj;
}
_object* new_atom_str(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_string;
	obj->sval=strdup(s);
	return obj;
}
_object* new_atom_str2(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_string2;
	obj->sval=strdup(s);
	return obj;
}
_object* cons(_object *first,_object *last){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_CONS;
	obj->car=first;
	obj->cdr=last;
	return obj;
}
_object* new_fn(_object *(*fn)(_object *, _object*)){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->type=t_func;
	obj->fn=fn;
	return obj;
}
_object *the_empty_list(void){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_EMPTY_LIST;
	obj->car=NULL;
	obj->cdr=NULL;
	return obj;
}
void print_atom(_object *obj){
	if(!obj) return;
	switch(obj->type){
		case t_integer:
			printf("%d",obj->ival);
			break;
		case t_float:
			printf("%g",obj->fval);
			break;
		case t_symbol:
			printf("%s",obj->sval);
			break;
		case t_string:
			printf("\"%s\"",obj->sval);
			break;
		case t_string2:
			printf("'%s'",obj->sval);
			break;
		case t_boolean:
			obj->ival? printf("T"):printf("NIL");
			break;
		default:
			//printf("u?");
			break;
	}
}
void crlf(void){
	printf("\n");
}
void del_atom(_object *obj){
	if(obj==NULL) return;
	if(obj->sval==NULL) return;
	if(obj->sval) free(obj->sval);
	free(obj);
}
void del_cascade(_object *obj){
	
	del_atom(obj);
}
void write_pair(_object *pair){
	_object *car_obj,*cdr_obj;
	car_obj=car(pair);
	cdr_obj=cdr(pair);

	print_ast(car_obj);
	if(cdr_obj->ltype==T_CONS){
		printf(" ");
		write_pair(cdr_obj);
	}
	else if(cdr_obj->ltype==T_EMPTY_LIST){
		return;
	}
	else{
		printf(" . ");
		print_ast(cdr_obj);
	}
}
void print_ast(_object *ast){
	if(!ast) return;
	if(IS_CONS(ast)){
		printf("(");
		write_pair(ast);
		printf(")");
	}
	if(IS_ATOM(ast)){
		print_atom(ast);
	}
	if(IS_EMPTY(ast)){
		printf("()");
	}
}
void print_debug_ast(_object *ast){
	if(!ast) return;
	if(IS_CONS(ast)){
		printf("(cons ");
		print_debug_ast(car(ast));
		printf(" ");
		print_debug_ast(cdr(ast));
		printf(" )");
	}
	if(IS_ATOM(ast)){
		print_atom(ast);
	}
	if(IS_EMPTY(ast)){
		printf("()");
	}
}
_object *car(_object *pair) {
	return pair->car;
}
_object *cdr(_object *pair) {
	return pair->cdr;
}
