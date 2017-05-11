#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "env.h"
#include "eval.h"
#include "print.h"

void print_atom(_object *obj){
	if(!obj) return;
	switch(obj->type){
		case t_integer:
			printf("%ld",obj->data.fixnum.value);
			break;
		case t_float:
			printf("%g",obj->data.dotted.value);
			break;
		case t_symbol:
			printf("%s",obj->data.symbol.value);
			break;
		case t_string_qq:
			printf("\"%s\"",obj->data.string.value);
			break;
		case t_string_q:
			printf("'%s'",obj->data.string.value);
			break;
		case t_boolean:
			obj->data.boolean.value? printf("#t"):printf("#f");
			break;
		default:
			//printf("u?");
			break;
	}
}
void crlf(void){
	printf("\n");
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
void print_obj(_object *obj){
	if(IS_ATOM(obj)){
		print_atom(obj);
	}
	else if(is_pair(obj)){
		if(is_list(obj)){
			print_list(obj);
		}
		else if(is_hash(obj)){
			print_hash(obj);
		}
		else if(is_func(obj)){
			print_func(obj);
		}
		else{
			printf("u?\n");
		}
	}
	else{
		printf("u?\n");
	}
}
void print_list(_object *obj){
	object_t *x;
	x=obj;
	printf("#<list>");
	print_ast(x);
}
void print_hash(_object *obj){
	printf("#<hash>");
	print_ast(obj);
}
void print_func(_object *obj){
	if(is_primitive_proc(obj)){
		printf("#<fn-c function>");
	}
	else if(is_compound_proc(obj)){
		printf("#<fn function>");
	}
	else{
		printf("#<function>");
	}
}
