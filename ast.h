/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef AST_H
#define AST_H

enum __type{T_ATOM,T_LIST,T_CONS,T_EMPTY_LIST,T_BOTTOM,t_lambda,t_func,t_compound_proc,t_integer,t_float,t_symbol,t_string_qq,t_string_q,t_boolean};

typedef struct _object{
	int ltype;
	int type;

	union{
		struct {
			int value;
		}boolean;
		struct{
			long value;
		}fixnum;
		struct{
			float value;
		}dotted;
		struct {
			char *value;
		} string;
		struct {
			char *value;
		}symbol;
		struct {
			char value;
		}character;
		struct {
			struct _object *(*fn)(struct _object *arguments);
			//struct _object *(*fn)(struct _object *, struct _object*);
		}primitive_proc;
		struct {
			struct _object *parameters;
			struct _object *body;
			struct _object *env;
		}compound_proc;
		struct {
			struct _object *car;
			struct _object *cdr;
		}pair;
		struct {
			//FILE *stream;
		}input_port;
		struct {
			//FILE *stream;
		}output_port;
	}data;
} _object,object_t;

#define IS_ATOM(x)    ((x)->ltype==T_ATOM)
#define IS_LIST(x)    ((x)->ltype==T_LIST)
#define IS_PAIR(x)    ((x)->ltype==T_LIST)
#define IS_INTEGER(x) ((x)->type==t_integer)
#define IS_FLOAT(x)   ((x)->type==t_float)
#define IS_SYMBOL(x)  ((x)->type==t_symbol)
#define IS_STRING_QQ(x) ((x)->type==t_string_qq)
#define IS_STRING_Q(x)  ((x)->type==t_string_q)
#define IS_STRING(x)  ((x)->type==t_string_qq || (x)->type==t_string_q)
#define IS_NUMBER(x) ((x)->type==t_integer || (x)->type==t_float)
#define IS_BOOLEAN(x) ((x)->type==t_boolean)
#define IS_CONS(x)    ((x)->ltype==T_CONS)
#define IS_EMPTY(x)   ((x)->ltype==T_EMPTY_LIST)
#define IS_BOTTOM(x)  ((x)->ltype==T_BOTTOM)

_object* new_object(void);
_object* new_atom_i(int ival);
_object* new_atom_f(float fval);
_object* new_atom_s(char *s);
_object* new_atom_str_QQ(char *s);
_object* new_atom_str_Q(char *s);
_object* new_atom_b(int ival);
_object* new_atom_bottom(void);
_object* cons(_object *first,_object *last);
_object* new_fn(_object *(*fn)(_object *arguments));
_object* new_compound_fn(_object *parameters,_object *body,_object *env);
void print_atom(_object *obj);
void crlf(void);
void del_atom(_object *obj);
void del_cascade(_object *obj);
void print_ast(_object *ast);
void print_debug_ast(_object *ast);
_object *car(_object *pair);
_object *cdr(_object *pair);
_object *new_empty_list(void);
int is_the_empty_list(_object *obj);
int is_false(object_t *obj);
int is_true(object_t *obj);


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

//object_t *quote_symbol;
object_t *define_symbol;
object_t *set_symbol;
object_t *ok_symbol;
object_t *if_symbol;
object_t *lambda_symbol;
object_t *begin_symbol;
object_t *cond_symbol;
object_t *else_symbol;
//object_t *let_symbol;
object_t *and_symbol;
object_t *or_symbol;
//object_t *quasiquote_symbol;
//object_t *unquote_symbol;
//object_t *unquote_splicing_symbol;
//object_t *letstar_symbol;
//object_t *letrec_symbol;
//object_t *defmacro_symbol;
//object_t *macroexpand_symbol;
object_t *eval_symbol;


#endif
