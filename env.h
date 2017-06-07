#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

object_t *make_symbol(char *value);
int is_pair(object_t *obj);
void set_car(object_t *obj, object_t *value);
void set_cdr(object_t *obj, object_t *value);

object_t *the_empty_list;
object_t *false;
object_t *true;
object_t *bottom;
object_t *symbol_table;
object_t *the_empty_environment;
object_t *the_global_environment;

object_t *error_proc(object_t *args);
void set_debug_var(int b);
int is_debug_var(void);

object_t *make_primitive_proc(object_t *(*fn)(struct _object *arguments));
int is_primitive_proc(object_t *obj);
object_t *is_procedure_proc(object_t *arguments);
object_t *make_compound_proc(object_t *parameters, object_t *body, object_t *env);
int is_compound_proc(object_t *obj);
int is_equal_variable(_object *var1,_object *var2);
object_t *eval_proc(object_t *arguments);
object_t *apply_proc(object_t *arguments);
int is_list(object_t *obj);
int is_hash(object_t *obj);
int is_func(object_t *obj);
char are_number_args(object_t *arguments);
char are_float_args(object_t *arguments);
object_t *add_proc(object_t *arguments);
object_t *sub_proc(object_t *arguments);
object_t *neg_proc(object_t *arguments);
object_t *div_proc(object_t *arguments);
object_t *mod_proc(object_t *arguments);
object_t *mul_proc(object_t *arguments);
object_t *is_number_equal_proc(object_t *arguments);
object_t *is_not_number_equal_proc(object_t *arguments);
object_t *not_proc(object_t *arguments);
object_t *is_less_than_proc(object_t *arguments);
object_t *is_greater_than_proc(object_t *arguments);
object_t *is_less_and_eq_than_proc(object_t *arguments);
object_t *is_greater_and_eq_than_proc(object_t *arguments);
int get_list_index(int index,object_t *list);
int is_range_index(object_t *obj);
object_t *get_list_range(object_t *index,object_t *list);
object_t *get_list_slice(int a,int b,object_t *list);
int count_list(object_t *list);
object_t *count_proc(object_t *arguments);
object_t *get_list_proc(object_t *arguments);
object_t *get_hash_proc(object_t *arguments);
int subst_node_tree(object_t *exnode,object_t *newnode);
int append_node_tree(object_t *exnode,object_t *newnode);
int add_node_last(object_t *lastnode,object_t *newnode);
object_t *make_hash_node(object_t *key,object_t *val);
object_t *set_list_proc(object_t *arguments);
object_t *set_hash_proc(object_t *arguments);
object_t *enclosing_environment(object_t *env);
object_t *first_frame(object_t *env);
object_t *make_frame(object_t *variables, object_t *values);
object_t *frame_variables(object_t *frame);
object_t *frame_values(object_t *frame);
void add_binding_to_frame(object_t *var, object_t *val, object_t *frame);
object_t *extend_environment(object_t *vars, object_t *vals,object_t *base_env);
object_t *lookup_variable_value(object_t *var, object_t *env);
void set_variable_value(object_t *var, object_t *val, object_t *env);
void define_variable(object_t *var, object_t *val, object_t *env);
object_t *setup_environment(void);
void populate_environment(object_t *env);
object_t *make_environment(void);
void init_env(void);

#endif
