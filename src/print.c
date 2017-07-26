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
			//print_string(obj->data.string.value);
			break;
		case t_string_q:
			printf("'%s'",obj->data.string.value);
			//print_string(obj->data.string.value);
			break;
		case t_boolean:
			obj->data.boolean.value? printf("#t"):printf("#f");
			break;
		default:
			//printf("u?");
			break;
	}
}
void display_atom(_object *obj){
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
			//printf("\"%s\"",obj->data.string.value);
			print_string(obj->data.string.value);
			break;
		case t_string_q:
			//printf("'%s'",obj->data.string.value);
			print_string(obj->data.string.value);
			break;
		case t_boolean:
			obj->data.boolean.value? printf("#t"):printf("#f");
			break;
		default:
			//printf("u?");
			break;
	}
}
void print_string(char*str){
	FILE *out=stdout;
	while (*str != '\0') {
				switch (*str) {
					case '\n':
						//fprintf(out, "\\n");
						//break;
					case '\\':
						//fprintf(out, "\\\\");
						if(*(str+1)=='n'){
							fprintf(out,"\n");
							str++;
							break;
						}
						putc(*str, out);
						break;
					case '"':
						//fprintf(out, "\\\"");
						//break;
					default:
						putc(*str, out);
				}
				str++;
			}
			//fputc('"',out);
			//break;

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
	else if(IS_EMPTY(obj)){
		printf("()");
	}
	else{
		fprintf(stderr,"u?\n");
	}
}
void display_obj(_object *obj){
	if(IS_ATOM(obj)){
		display_atom(obj);
	}
	else if(is_pair(obj)){
		if(is_list(obj)){
			display_list(obj);
		}
		else if(is_hash(obj)){
			display_hash(obj);
		}
		else if(is_func(obj)){
			print_func(obj);
		}
		else{
			fprintf(stderr,"u?\n");
		}
	}
	else{
		fprintf(stderr,"u?\n");
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
void display_list(_object *obj){
	object_t *x;
	x=obj;
	printf("#<list>");
	print_ast(x);
}
void display_hash(_object *obj){
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
object_t *type_proc(object_t *obj){
	//printf("type...\n");
	obj=car(obj);
	if(!obj) {fprintf(stderr,"error object type...\n");return bottom;}
	if(IS_ATOM(obj)){
		switch(obj->type){
		case t_integer:
			return new_atom_s("integer");
			break;
		case t_float:
			return new_atom_s("float");
			break;
		case t_symbol:
			return new_atom_s("symbol");
			break;
		case t_string_qq:
			return new_atom_s("string");
			break;
		case t_string_q:
			return new_atom_s("string");
			break;
		case t_boolean:
			return new_atom_s("boolean");
			break;
		default:
			return bottom;
			break;
	}
	}
	else if(is_pair(obj)){
		if(is_list(obj)){
			return new_atom_s("list");
		}
		else if(is_hash(obj)){
			return new_atom_s("hash");
		}
		else if(is_func(obj)){
			return new_atom_s("function");
		}
		else{
			fprintf(stderr,"u?\n");
			return bottom;
		}
	}
	else{
		fprintf(stderr,"u?\n");
		return bottom;
	}
	return bottom;
}
object_t *prn_proc(object_t *obj){
	//printf("prn...\n");
	object_t *x;
	x=obj;
	while(x && !IS_EMPTY(x)){
		display_obj(car(x));
		x=cdr(x);
	}
	return ok_symbol;
}
void clean_print_obj(_object *obj){
	if(!obj) return;
	if(IS_BOTTOM(obj)) return;
	if(IS_ATOM(obj) && obj->type==t_symbol && (strcmp(obj->data.symbol.value,"__ok__")==0)) return;
	print_obj(obj);
}