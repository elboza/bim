#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "env.h"
#include "eval.h"
#include "print.h"


object_t *make_symbol(char *value){
	return new_atom_s(value);
}
int is_pair(object_t *obj){
	return (IS_PAIR(obj)||IS_CONS(obj));
}
void set_car(object_t *obj, object_t *value){
	obj->data.pair.car = value;
}
void set_cdr(object_t *obj, object_t *value){
	obj->data.pair.cdr = value;
}
object_t *error_proc(object_t *args){
	printf("error !\n");
	return new_atom_bottom();
	//return bottom;
}
void set_debug_var(int b){
	object_t *dbg,*bval;
	(b==1)?(bval=true):(bval=false);
	dbg=cons(new_atom_s("__assign__"),cons(new_atom_s("__debug__"),cons(bval,new_empty_list())));
	eval(dbg,the_global_environment);
	//del_cascade(dbg);
}
int is_debug_var(void){
	object_t *dbg,*ret;
	//int x;
	dbg=new_atom_s("__debug__");
	ret=eval(dbg,the_global_environment);
	return is_true(ret);
	//del_cascade(ret);
	//del_cascade(dbg);
	//return x;
}
int is_list(object_t *obj){
	if(is_pair(obj)){
		if(is_tagged_list(obj,list_symbol)){
			return 1;
		}
	}
	return 0;
}
int is_hash(object_t *obj){
	if(is_pair(obj)){
		if(is_tagged_list(obj,hash_symbol)){
			return 1;
		}
	}
	return 0;
}
int is_func(object_t *obj){
	if(is_pair(obj)){
		if(is_primitive_proc(obj) || is_compound_proc(obj)){
			return 1;
		}
	}
	return 0;
}
char are_number_args(object_t *arguments){
	object_t *ptr;
	int argno=1;
	ptr=arguments;
	while(!is_the_empty_list(ptr)){
		if(!IS_NUMBER(ptr)) return argno;
		ptr=cdr(ptr);
		argno++;
	}
	return 0;
}
char are_float_args(object_t *arguments){
	object_t *ptr;
	int argno=1;
	ptr=arguments;
	while(!is_the_empty_list(ptr)){
		//if(car(ptr)->type== T_FLOAT) return 1;
		if(IS_FLOAT(car(ptr))) return 1;
		ptr=cdr(ptr);
		argno++;
	}
	return 0;
}
object_t *add_proc(object_t *arguments) {
	long result = 0;
	float fresult=0;
	
	if(are_float_args(arguments)){
		while (!is_the_empty_list(arguments)) {
			if(IS_FLOAT(car(arguments))){
				fresult += (car(arguments))->data.dotted.value;
			}
			else{
				fresult += (float)(car(arguments))->data.fixnum.value;
			}
			arguments = cdr(arguments);
		}
		return new_atom_f(fresult);
	}
	else{
		while (!is_the_empty_list(arguments)) {
			result += (car(arguments))->data.fixnum.value;
			arguments = cdr(arguments);
		}
		return new_atom_i(result);
	}
}
object_t *sub_proc(object_t *arguments) {
	long result;
	float fresult=0;
	
	if(are_float_args(arguments)){
		if(IS_FLOAT(car(arguments))){
			fresult = (car(arguments))->data.dotted.value;
		}
		else{
			fresult = (float)(car(arguments))->data.fixnum.value;
		}
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			if(IS_FLOAT(car(arguments))){
				fresult -= (car(arguments))->data.dotted.value;
			}
			else{
				fresult -= (float)(car(arguments))->data.fixnum.value;
			}
			//arguments = cdr(arguments);
		}
		return new_atom_f(fresult);
	}
	else{
		result = (car(arguments))->data.fixnum.value;
		while (!is_the_empty_list(arguments = cdr(arguments))) {
			result -= (car(arguments))->data.fixnum.value;
			//arguments = cdr(arguments);
		}
		return new_atom_i(result);
	}
}
object_t *neg_proc(object_t *arguments){
	long result;
	float fresult=0;
	if(are_float_args(arguments)){
		fresult = -1 * (car(arguments))->data.dotted.value ;
		return new_atom_f(fresult);
	}else{
		result = -1 * (car(arguments))->data.fixnum.value;
	}
	return new_atom_i(result);
}
object_t *div_proc(object_t *arguments) {
	if(are_float_args(arguments)){
		return new_atom_f(
			(float)((car(arguments) )->data.dotted.value)/(float)
			((cadr(arguments))->data.dotted.value));
	}
	else{
		return new_atom_f(
			(float)((car(arguments) )->data.fixnum.value)/(float)
			((cadr(arguments))->data.fixnum.value));
	}
}
object_t *mod_proc(object_t *arguments) {
	if(are_float_args(arguments)){
//		return new_atom_f((float)
//			((car(arguments) )->data.dotted.value)%
//			(float)((cadr(arguments))->data.dotted.value));
		fprintf(stderr,"float arguments not allowed.\n");
		return bottom;
	}
	else{
		return new_atom_i(
			((car(arguments) )->data.fixnum.value)%
			((cadr(arguments))->data.fixnum.value));
	}
}
object_t *mul_proc(object_t *arguments) {
	long result = 1;
	float fresult=1;
	
	if(are_float_args(arguments)){
		while (!is_the_empty_list(arguments)) {
			if(IS_FLOAT(car(arguments))){
				fresult *= (car(arguments))->data.dotted.value;
			}
			else{
				fresult *= (float)(car(arguments))->data.fixnum.value;
			}
			arguments = cdr(arguments);
		}
		return new_atom_f(fresult);
	}
	else{
		while (!is_the_empty_list(arguments)) {
			result *= (car(arguments))->data.fixnum.value;
			arguments = cdr(arguments);
		}
		return new_atom_i(result);
	}
}
object_t *make_primitive_proc(object_t *(*fn)(struct _object *arguments)){
	return new_fn(fn);
}
int is_primitive_proc(object_t *obj) {
	return obj->type == t_func;
}
object_t *is_procedure_proc(object_t *arguments) {
	object_t *obj;
	
	obj = car(arguments);
	return (is_primitive_proc(obj) ||
	is_compound_proc(obj)) ?
	true :
	false;
}
object_t *make_compound_proc(object_t *parameters, object_t *body, object_t *env) {
	object_t *obj;
	
	obj = new_compound_fn(parameters,body,env);

	return obj;
}
   
int is_compound_proc(object_t *obj) {
	return obj->type == t_compound_proc;
}
int is_equal_variable(_object *var1,_object *var2){
	if(var1->type==t_symbol && var2->type==t_symbol){
		if(strcmp(var1->data.symbol.value,var2->data.symbol.value)==0) return 1;
	}
	return 0;
}
object_t *eval_proc(object_t *arguments) {
	fprintf(stderr, "illegal state: The body of the eval "
	"primitive procedure should not execute.\n");
	exit(1);
}
object_t *apply_proc(object_t *arguments) {
	fprintf(stderr, "illegal state: The body of the apply "
	"primitive procedure should not execute.\n");
	exit(1);
}
object_t *enclosing_environment(object_t *env) {
	return cdr(env);
}

object_t *first_frame(object_t *env) {
	return car(env);
}

object_t *make_frame(object_t *variables, object_t *values) {
	return cons(variables, values);
}

object_t *frame_variables(object_t *frame) {
	return car(frame);
}

object_t *frame_values(object_t *frame) {
	return cdr(frame);
}

void add_binding_to_frame(object_t *var, object_t *val, object_t *frame) {
	set_car(frame, cons(var, car(frame)));
	set_cdr(frame, cons(val, cdr(frame)));
}

object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env) {
	return cons(make_frame(vars, vals), base_env);
}
 
object_t *lookup_variable_value(object_t *var, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!is_the_empty_list(vars)) {
			if (is_equal_variable(var, car(vars))) {
				return car(vals);
			}
			if(is_equal_variable(var,cdr(vars))){
				//printf("found &rest param \n");//,cdr(vars)->data.string.value);
				return cdr(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
			if(!vars) break;
			if(!is_pair(vars)) break;
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "unbound variable, %s\n", var->data.symbol.value);
	//exit(1);
	return bottom;
}
 
void set_variable_value(object_t *var, object_t *val, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;
 
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!is_the_empty_list(vars)) {
			if (is_equal_variable(var, car(vars))) {
				set_car(vals, val);
				return;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "unbound variable, %s\n", var->data.symbol.value);
	//exit(1);
}
 
void define_variable(object_t *var, object_t *val, object_t *env) {
	object_t *frame;
	object_t *vars;
	object_t *vals;

	frame = first_frame(env);    
	vars = frame_variables(frame);
	vals = frame_values(frame);

	while (!is_the_empty_list(vars)) {
		if (is_equal_variable(var, car(vars))) {
			set_car(vals, val);
			return;
		}
		vars = cdr(vars);
		vals = cdr(vals);
	}
	add_binding_to_frame(var, val, frame);
}
object_t *setup_environment(void) {
	object_t *initial_env;
 
	initial_env = extend_environment(
	the_empty_list,
	the_empty_list,
	the_empty_environment);
	return initial_env;
}
void populate_environment(object_t *env) {
 
	#define add_procedure(scheme_name, c_name)              \
	define_variable(make_symbol(scheme_name),           \
	make_primitive_proc(c_name),        \
	env);
 
	add_procedure("__error__", error_proc);
	add_procedure("__add__", add_proc);
	add_procedure("__sub__", sub_proc);
	add_procedure("__neg__", neg_proc);
	add_procedure("__div__", div_proc);
	add_procedure("__mod__", mod_proc);
	add_procedure("__mul__", mul_proc);
	//add_procedure("global", global_proc);

	/*#define FUNCTION_SYMBOL(name, func_ptr) \
		(cons(new_atom_s((name)), cons(new_fn((func_ptr)), NULL)))
	
	//env=cons(FUNCTION_SYMBOL("let",&fn_let),NULL);
	
	//#undef FUNCTION_SYMBOL */
}
object_t *make_environment(void) {
	object_t *env;
 
	env = setup_environment();
	populate_environment(env);
	return env;
}
void init_env(void){

	the_empty_list = new_empty_list();
	false = new_atom_b(0);
	true = new_atom_b(1);
	bottom = new_atom_bottom();
	symbol_table = the_empty_list;

	//make_symbol...
	assign_symbol = make_symbol("__assign__");
	lambda_symbol = make_symbol("__lambda__");
	begin_symbol = make_symbol("__progn__");
	global_symbol = make_symbol("__global__");
	list_symbol = make_symbol("__list__");
	hash_symbol = make_symbol("__hash__");
	ok_symbol = make_symbol("__ok__");

	the_empty_environment = the_empty_list;
	the_global_environment = make_environment();
	
}
