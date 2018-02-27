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
	//if(IS_BOOLEAN(ret) && ret->data.boolean.value==1) return 1;
	//return 0;
	return is_true2(ret);
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
	if(IS_FLOAT(car(arguments)) && IS_FLOAT(cadr(arguments))){
		return new_atom_f(
			(float)((car(arguments) )->data.dotted.value)/(float)
			((cadr(arguments))->data.dotted.value));
	}
	if(IS_FLOAT(car(arguments)) && !IS_FLOAT(cadr(arguments))){
		return new_atom_f(
			(float)((car(arguments) )->data.dotted.value)/(float)
			((cadr(arguments))->data.fixnum.value));
	}
	if(!IS_FLOAT(car(arguments)) && IS_FLOAT(cadr(arguments))){
		return new_atom_f(
			(float)((car(arguments) )->data.fixnum.value)/(float)
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
object_t *is_number_equal_proc(object_t *arguments) {
	long value;
	float f1,f2;
	if(are_float_args(arguments)){
		if(IS_FLOAT(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			if(IS_FLOAT(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 != f2) {
				return false;
			}
		}
		return true;
	}
	else{
		value = (car(arguments))->data.fixnum.value;
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			if (value != ((car(arguments))->data.fixnum.value)) {
				return false;
			}
		}
		return true;
	}
}
object_t *is_not_number_equal_proc(object_t *arguments){
	return not_proc(cons(is_number_equal_proc(arguments),the_empty_list));
}
object_t *not_proc(object_t *arguments){
	object_t *x;
	x=car(arguments);
	if(IS_BOOLEAN(x)){
		if(is_true2(x)) return false;
		return true;
	}
	fprintf(stderr,"argument is not boolean\n");
	return bottom;
}
object_t *is_less_than_proc(object_t *arguments) {
	long previous;
	long next;
	float f1,f2;
	
	if(are_float_args(arguments)){
		if(IS_FLOAT(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			if(IS_FLOAT(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 < f2) {
				f1=f2;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		previous = (car(arguments))->data.fixnum.value;
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			next = (car(arguments))->data.fixnum.value;
			if (previous < next) {
				previous = next;
			}
			else {
				return false;
			}
		}
		return true;
	}
}

object_t *is_greater_than_proc(object_t *arguments) {
	long previous;
	long next;
	float f1,f2;
	
	if(are_float_args(arguments)){
		if(IS_FLOAT(car(arguments))) {f1=((car(arguments))->data.dotted.value);} else {f1=(float)((car(arguments))->data.fixnum.value);}
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			if(IS_FLOAT(car(arguments))){
				f2=((car(arguments))->data.dotted.value);
			}
			else{
				f2=(float)((car(arguments))->data.fixnum.value);
			}
			if (f1 > f2) {
				f1=f2;
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		previous = (car(arguments))->data.fixnum.value;
		while (!IS_EMPTY(arguments = cdr(arguments))) {
			next = (car(arguments))->data.fixnum.value;
			if (previous > next) {
				previous = next;
			}
			else {
				return false;
			}
		}
		return true;
	}
}
object_t *is_less_and_eq_than_proc(object_t *arguments){
	return not_proc(cons(is_greater_than_proc(arguments),the_empty_list));
}
object_t *is_greater_and_eq_than_proc(object_t *arguments){
	return not_proc(cons(is_less_than_proc(arguments),the_empty_list));
}
int get_list_index(int index,object_t *list){
	if(!is_list(list)) return -1;
	int count=count_list(list);
	if(index<0) index=count+index; //formally count-index
	if(index<0 || index>count-1){
		fprintf(stderr,"list: out of bounds\n");
		return -1;
	}
	return index;
}
int get_string_index(int index,object_t *x){
	if(!IS_STRING(x)) return -1;
	int count=count_str(x);
	if(index<0) index=count+index; //formally count-index
	if(index<0 || index>count-1){
		fprintf(stderr,"string: out of bounds\n");
		return -1;
	}
	return index;
}
int is_range_index(object_t *obj){
	if(is_pair(obj)){
		if(is_tagged_list(obj,list_range_symbol)){
			return 1;
		}
	}
	return 0;
}
object_t *get_list_range(object_t *index,object_t *list){
	//printf("list_range\n");
	int a=cadr(index)->data.fixnum.value;
	int b=caddr(index)->data.fixnum.value;
	a=get_list_index(a,list);
	b=get_list_index(b,list);
	if(a<b) return get_list_slice(a,b,list);
	return get_list_slice(b,a,list);
}
object_t *get_string_range(object_t *index,object_t *x){
	int a=cadr(index)->data.fixnum.value;
	int b=caddr(index)->data.fixnum.value;
	a=get_string_index(a,x);
	b=get_string_index(b,x);
	if(a<b) return get_string_slice(a,b,x);
	return get_string_slice(b,a,x);
}
object_t *get_list_slice(int a,int b,object_t *list){
	object_t *new_list=cons(new_atom_s("__list__"),new_empty_list());
	int count=0;
	list=cdr(list);
	while(list && !IS_EMPTY(list) && count<=b){
		if(count>=a){
			add_node_last(new_list,car(list));
			//list=cdr(list);
			//count++;
		}
		list=cdr(list);
		count++;
	}
	return new_list;
}
object_t *get_string_slice(int a,int b,object_t *x){
	char *s,*p=(char*)(x->data.string.value+a);
	int len=count_str(x);
	s=(char*)malloc(len+5);
	if(!s){fprintf(stderr,"error allocating memory...\n"); return new_atom_bottom();}
	if(!strncpy(s,p,b-a+1)){fprintf(stderr,"error getting string slice...\n"); return new_atom_bottom();}
	return new_atom_s(s);
}
int count_list(object_t *x){
	//object_t *x=car(arguments);
	//if istagged list "__list__" || "__hash__"
	if(!is_list(x) && !is_hash(x)){
		fprintf(stderr,"the argument is not a list or a hash\n");
		return -1;
	}
	x=cdr(x);
	int n=0;
	while(!is_the_empty_list(x)){
		x=cdr(x);
		n++;
	}
	return n;
}
int count_str(object_t *x){
	if(!IS_STRING(x)){ fprintf(stderr,"argment is not a string.\n"); return -1;}
	if(x->data.string.value){
		return strlen(x->data.string.value);
	}
	fprintf(stderr,"invalid string...\n");
	return -1;
}
object_t *count_proc(object_t *arguments){
	object_t *x=car(arguments);
	int n;
	if(IS_STRING(x)){
		n=count_str(x);
		return new_atom_i(n);
	}
	n=count_list(x);
	if(n==-1) return new_atom_bottom();
	return new_atom_i(n);
}
object_t *get_string_at(object_t *arguments){
	object_t *x=cadr(arguments);
	object_t *index=car(arguments);
	char str[10];
	if(!IS_STRING(x)){ fprintf(stderr,"argument is not a string.\n"); return new_atom_bottom();}
	if(is_range_index(index)) return get_string_range(index,x);
	int index_val=get_string_index(index->data.fixnum.value,x);
	if(index_val==-1) return new_atom_bottom();
	str[0]=x->data.string.value[index_val];
	str[1]='\0';
	return new_atom_s((char*)&str);
}
object_t *get_list_proc(object_t *arguments){
	object_t *index,*list;
	if(IS_STRING(cadr(arguments))) return get_string_at(arguments);
	if(!is_list(cadr(arguments))) return new_atom_bottom();
	index=car(arguments);
	list=cadr(arguments);
	list=cdr(list);
	int n=0;
	if(is_range_index(index)) return get_list_range(index,cadr(arguments));
	int index_val=get_list_index(index->data.fixnum.value,cadr(arguments));
	if(index_val==-1) return new_atom_bottom();
	if(list==NULL) return new_atom_bottom();
	if(is_the_empty_list(list)) return new_atom_bottom();
	while(n++<index_val){
		list=cdr(list);
	}
	return car(list);
}
object_t *get_hash_proc(object_t *arguments){
	object_t *key,*list,*obj;
	key=car(arguments);
	list=cdadr(arguments);
	if(!is_hash(cadr(arguments))) return new_atom_bottom();
	while(list && !is_the_empty_list(list)){
		obj=caar(list);
		if(strcmp(obj->data.string.value,key->data.string.value)==0) return cadar(list);
		list=cdr(list);
	}
	return new_atom_bottom();
}
int subst_node_tree(object_t *exnode,object_t *newnode){
	exnode->ancestor->data.pair.car=newnode;
	newnode->ancestor=exnode->ancestor;
	return 0;
}
int append_node_tree(object_t *exnode,object_t *newnode){
	exnode->ancestor->data.pair.cdr=newnode;
	newnode->ancestor=exnode->ancestor;
	return 0;
}
int add_node_last(object_t *lastnode,object_t *newnode){
	object_t *list;
	list=cdr(lastnode);
	//if(!is_hash(cadr(arguments))) return new_atom_bottom();
	while(list && !is_the_empty_list(list)){
		list=cdr(list);
	}
	return append_node_tree(list,cons(newnode,new_empty_list()));
}
object_t *make_hash_node(object_t *key,object_t *val){
	return cons(key,cons(val,new_empty_list()));
}
object_t *set_list_proc(object_t *arguments){
	object_t *var,*val,*obj;
	//printf("set list\n");
	//index=car(arguments);
	var=cadr(arguments);
	val=caddr(arguments);
	if(!is_list(var)){
		//printf("not a list!!\n");
		return bottom;
	}
	obj=get_list_proc(arguments);
	if(subst_node_tree(obj,val)!=0) return bottom;
	return ok_symbol;
}
object_t *set_hash_proc(object_t *arguments){
	object_t *index,*var,*val,*obj;
	//printf("set hash\n");
	index=car(arguments);
	var=cadr(arguments);
	val=caddr(arguments);
	if(!is_hash(var)){
		//printf("not a hash!!\n");
		return bottom;
	}
	obj=get_hash_proc(arguments);
	if(IS_BOTTOM(obj)){add_node_last(var,make_hash_node(index,val));return ok_symbol;}
	if(subst_node_tree(obj,val)!=0) return bottom;
	return ok_symbol;
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
 
	while (!IS_EMPTY(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		while (!IS_EMPTY(vars)) {
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
	add_procedure("__get_list__", get_list_proc);
	add_procedure("__get_hash__", get_hash_proc);
	add_procedure("__set_list__", set_list_proc);
	add_procedure("__set_hash__", set_hash_proc);
	add_procedure("__type__", type_proc);
	add_procedure("count", count_proc);
	add_procedure("type", type_proc);
	add_procedure("__prn__", prn_proc);
	add_procedure("print", prn_proc);
	add_procedure("__eq__",is_number_equal_proc);
	add_procedure("__neq__",is_not_number_equal_proc);
	add_procedure("__not__",not_proc);
	add_procedure("__lt__",is_less_than_proc);
	add_procedure("__gt__",is_greater_than_proc);
	add_procedure("__le__",is_less_and_eq_than_proc);
	add_procedure("__ge__",is_greater_and_eq_than_proc);

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
	list_range_symbol = make_symbol("__list_range__");
	ok_symbol = make_symbol("__ok__");
	if_symbol = make_symbol("__if__");
	or_symbol = make_symbol("__or__");
	and_symbol = make_symbol("__and__");
	while_symbol = make_symbol("__while__");

	the_empty_environment = the_empty_list;
	the_global_environment = make_environment();
	
}
