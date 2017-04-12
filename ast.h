/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef AST_H
#define AST_H

enum __type{T_ATOM,T_LIST,T_CONS,T_EMPTY_LIST,t_lambda,t_func,t_integer,t_float,t_symbol,t_boolean};

typedef struct _object{
	int ltype;
	int type;
	
	int ival;
	float fval;
	char *sval;
	
	struct _object *(*fn)(struct _object *, struct _object*);
	
	struct _object *car;
	struct _object *cdr;
} _object;

#define IS_ATOM(x)    ((x)->ltype==T_ATOM)
#define IS_LIST(x)    ((x)->ltype==T_LIST)
#define IS_INTEGER(x) ((x)->type==t_integer)
#define IS_FLOAT(x)   ((x)->type==t_float)
#define IS_SYMBOL(x)  ((x)->type==t_symbol)
#define IS_BOOLEAN(x) ((x)->type==t_boolean)
#define IS_CONS(x)    ((x)->ltype==T_CONS)
#define IS_EMPTY(x)   ((x)->ltype==T_EMPTY_LIST)

_object* new_object(void);
_object* new_atom_i(int ival);
_object* new_atom_f(float fval);
_object* new_atom_s(char *s);
_object* new_atom_b(int ival);
_object* cons(_object *first,_object *last);
_object* new_fn(_object *(*fn)(_object *, _object*));
void print_atom(_object *obj);
void crlf(void);
void del_atom(_object *obj);
void del_cascade(_object *obj);
void print_ast(_object *ast);
void print_debug_ast(_object *ast);
_object *car(_object *pair);
_object *cdr(_object *pair);
_object *the_empty_list(void);


#define caar(obj)   car(car(obj))
#define cadr(obj)   car(cdr(obj))
#define cdar(obj)   cdr(car(obj))
#define cddr(obj)   cdr(cdr(obj))
#define caaar(obj)  car(car(car(obj)))
#define caadr(obj)  car(car(cdr(obj)))
#define cadar(obj)  car(cdr(car(obj)))
#define caddr(obj)  car(cdr(cdr(obj)))
#define cdaar(obj)  cdr(car(car(obj)))
#define cdadr(obj)  cdr(car(cdr(obj)))
#define cddar(obj)  cdr(cdr(car(obj)))
#define cdddr(obj)  cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))


#endif
