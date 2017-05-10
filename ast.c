/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "env.h"

_object* new_object(void){
	_object *obj;
	obj=(_object*)malloc(sizeof(_object));
	if(!obj) return NULL;
	//obj->sval=NULL;
	obj->data.pair.car=NULL;
	obj->data.pair.cdr=NULL;
	return obj;
}
_object* new_atom_i(int ival){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_integer;
	obj->data.fixnum.value=(long)ival;
	return obj;
}
_object* new_atom_b(int ival){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_boolean;
	obj->data.boolean.value=ival;
	return obj;
}
_object* new_atom_f(float fval){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_float;
	obj->data.dotted.value=fval;
	return obj;
}
_object* new_atom_s(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_symbol;
	obj->data.symbol.value=strdup(s);
	return obj;
}
_object* new_atom_str_QQ(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_string_qq;
	obj->data.string.value=strdup(s);
	return obj;
}
_object* new_atom_str_Q(char *s){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_ATOM;
	obj->type=t_string_q;
	obj->data.string.value=strdup(s);
	return obj;
}
_object* cons(_object *first,_object *last){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_CONS;
	obj->data.pair.car=first;
	obj->data.pair.cdr=last;
	return obj;
}
_object* new_fn(_object *(*fn)(_object *arguments)){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_LIST;
	obj->type=t_func;
	obj->data.primitive_proc.fn=fn;
	return obj;
}
_object* new_compound_fn(_object *parameters,_object *body,_object *env){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_LIST;
	obj->type=t_compound_proc;
	obj->data.compound_proc.parameters=parameters;
	obj->data.compound_proc.body=body;
	obj->data.compound_proc.env=env;
	return obj;
}
_object *new_empty_list(void){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_EMPTY_LIST;
	obj->data.pair.car=NULL;
	obj->data.pair.cdr=NULL;
	return obj;
}
int is_the_empty_list(_object *obj){
	return (IS_EMPTY(obj));
}
_object* new_atom_bottom(void){
	_object *obj;
	obj=new_object();
	if(!obj) return NULL;
	obj->ltype=T_BOTTOM;
	obj->data.pair.car=NULL;
	obj->data.pair.cdr=NULL;
	return obj;
}

void del_atom(_object *obj){
	if(obj==NULL) return;
	//if(is_the_empty_list(obj)) return;
	//if(IS_BOTTOM(obj)) return;
	if(IS_STRING(obj)){
		if(obj->data.string.value) free(obj->data.string.value);
	}
	if(IS_SYMBOL(obj)){
		if(obj->data.symbol.value) free(obj->data.symbol.value);
	}
	free(obj);
}
void del_cascade(_object *obj){
	if(IS_PAIR(obj)){
		del_cascade(car(obj));
		del_cascade(cdr(obj));
	}
	del_atom(obj);
}

_object *car(_object *pair) {
	return pair->data.pair.car;
}
_object *cdr(_object *pair) {
	return pair->data.pair.cdr;
}
int is_false(object_t *obj) {
	return obj == false;
}
int is_true(object_t *obj) {
	return !is_false(obj);
}