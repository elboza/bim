#include<stdlib.h>
#include "ast.h"
#include "environment.h"


static _object* fn_let(_object *a,_object *b){
	_object *obj;
	obj=new_atom_i(42);
	return obj;
}
_object* init_env(void){
	_object *env;
	
	#define FUNCTION_SYMBOL(name, func_ptr) \
		(cons(new_atom_s((name)), cons(new_fn((func_ptr)), NULL)))
	
	env=cons(FUNCTION_SYMBOL("let",&fn_let),NULL);
	
#undef FUNCTION_SYMBOL
	return env;
}
